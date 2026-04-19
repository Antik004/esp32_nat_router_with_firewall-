
#include <string.h>
#include <sys/socket.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "domain_filter.h"
#include "log_buffer.h"

#define DNS_PORT 53
#define DNS_MAX_PACKET_SIZE 512
#define UPSTREAM_DNS "8.8.8.8"  // Google DNS
#define UPSTREAM_DNS_PORT 53

static const char *TAG = "DNS_Server";
#include "esp_timer.h"  // for timestamp

#define MAX_CLIENTS 20
#define MAX_REQUESTS_PER_SECOND 5

typedef struct {
    uint8_t mac[6];
    uint32_t last_reset; // in seconds
    int count;
} mac_rate_t;

static mac_rate_t clients[MAX_CLIENTS];
static int num_clients = 0;

// DNS Header structure
typedef struct {
    uint16_t id;
    uint16_t flags;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;
} __attribute__((packed)) dns_header_t;
static int check_rate_limit_mac(uint8_t* mac) {
    uint32_t now = esp_timer_get_time() / 1000000; // current seconds

    for(int i = 0; i < num_clients; i++) {
        if(memcmp(clients[i].mac, mac, 6) == 0) {
            if(now - clients[i].last_reset >= 1) {
                clients[i].count = 0;
                clients[i].last_reset = now;
            }
            clients[i].count++;
            if(clients[i].count > MAX_REQUESTS_PER_SECOND) {
                return 0; // rate limit exceeded
            }
            return 1; // allowed
        }
    }

    // New client
    if(num_clients < MAX_CLIENTS) {
        memcpy(clients[num_clients].mac, mac, 6);
        clients[num_clients].count = 1;
        clients[num_clients].last_reset = now;
        num_clients++;
        return 1;
    }

    return 0; // too many clients tracked
}

// Parse domain name from DNS query
static int parse_domain_name(uint8_t* buf, int offset, char* domain, int max_len) {
    int i = 0;
    int len;
    int jumped = 0;
    int jump_offset = -1;
    
    while((len = buf[offset]) != 0) {
        // Check for compression pointer
        if((len & 0xC0) == 0xC0) {
            if(!jumped) {
                jump_offset = offset + 2;
            }
            offset = ((len & 0x3F) << 8) | buf[offset + 1];
            jumped = 1;
            continue;
        }
        
        offset++;
        
        if(i + len + 1 >= max_len) {
            return -1;
        }
        
        if(i > 0) {
            domain[i++] = '.';
        }
        
        memcpy(domain + i, buf + offset, len);
        i += len;
        offset += len;
    }
    
    domain[i] = '\0';
    
    if(jumped) {
        return jump_offset;
    }
    
    return offset + 1;
}

// Create DNS response with blocked IP (0.0.0.0)
static int create_blocked_response(uint8_t* query, int query_len, uint8_t* response) {
    memcpy(response, query, query_len);
    
    dns_header_t* header = (dns_header_t*)response;
    
    // Set response flags
    header->flags = htons(0x8180); // Standard query response, no error
    header->ancount = htons(1);    // One answer
    header->nscount = 0;
    header->arcount = 0;
    
    int offset = query_len;
    
    // Answer section
    // Name (pointer to question)
    response[offset++] = 0xC0;
    response[offset++] = 0x0C;
    
    // Type A
    response[offset++] = 0x00;
    response[offset++] = 0x01;
    
    // Class IN
    response[offset++] = 0x00;
    response[offset++] = 0x01;
    
    // TTL (60 seconds)
    response[offset++] = 0x00;
    response[offset++] = 0x00;
    response[offset++] = 0x00;
    response[offset++] = 0x3C;
    
    // Data length (4 bytes for IPv4)
    response[offset++] = 0x00;
    response[offset++] = 0x04;
    
    // IP address (0.0.0.0 - blocked)
    response[offset++] = 0x00;
    response[offset++] = 0x00;
    response[offset++] = 0x00;
    response[offset++] = 0x00;
    
    return offset;
}

// Forward DNS query to upstream server
static int forward_dns_query(uint8_t* query, int query_len, uint8_t* response, int max_response_len) {
    int upstream_sock;
    struct sockaddr_in upstream_addr;
    int response_len = 0;
    
    // Create socket for upstream DNS
    upstream_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(upstream_sock < 0) {
        ESP_LOGE(TAG, "Failed to create upstream socket");
        return -1;
    }
    
    // Set timeout
    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    setsockopt(upstream_sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    
    // Setup upstream DNS server address
    memset(&upstream_addr, 0, sizeof(upstream_addr));
    upstream_addr.sin_family = AF_INET;
    upstream_addr.sin_port = htons(UPSTREAM_DNS_PORT);
    inet_pton(AF_INET, UPSTREAM_DNS, &upstream_addr.sin_addr);
    
    // Send query to upstream DNS
    int sent = sendto(upstream_sock, query, query_len, 0, 
                     (struct sockaddr*)&upstream_addr, sizeof(upstream_addr));
    
    if(sent < 0) {
        ESP_LOGE(TAG, "Failed to send to upstream DNS");
        close(upstream_sock);
        return -1;
    }
    
    // Receive response from upstream DNS
    socklen_t addr_len = sizeof(upstream_addr);
    response_len = recvfrom(upstream_sock, response, max_response_len, 0,
                           (struct sockaddr*)&upstream_addr, &addr_len);
    
    close(upstream_sock);
    
    if(response_len < 0) {
        ESP_LOGE(TAG, "Failed to receive from upstream DNS");
        return -1;
    }
    
    return response_len;
}


void dns_server_task(void *pvParameters) {
    ESP_LOGI(TAG, "DNS server task started!");
    
    int sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    uint8_t rx_buffer[DNS_MAX_PACKET_SIZE];
    uint8_t tx_buffer[DNS_MAX_PACKET_SIZE];
    
    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sock < 0) {
        ESP_LOGE(TAG, "Failed to create socket, errno: %d", errno);
        add_log_line("DNS ERROR: Failed to create socket");
        vTaskDelete(NULL);
        return;
    }
    ESP_LOGI(TAG, "DNS socket created: %d", sock);
    
    // Allow reuse of address
    int reuse = 1;
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        ESP_LOGW(TAG, "Failed to set SO_REUSEADDR");
    }
    
    // Bind to DNS port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(DNS_PORT);
    
    ESP_LOGI(TAG, "Attempting to bind to port %d...", DNS_PORT);
    
    if(bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        ESP_LOGE(TAG, "Failed to bind socket to port %d, errno: %d", DNS_PORT, errno);
        add_log_line("DNS ERROR: Failed to bind port 53");
        close(sock);
        vTaskDelete(NULL);
        return;
    }
    
    ESP_LOGI(TAG, "✓ DNS server successfully bound to port %d", DNS_PORT);
    ESP_LOGI(TAG, "✓ Upstream DNS: %s", UPSTREAM_DNS);
    add_log_line("DNS server running on port 53");
    
    int query_count = 0;
    
    while(1) {
        memset(&client_addr, 0, sizeof(client_addr));
        client_addr_len = sizeof(client_addr);
        
        int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer), 0, 
                          (struct sockaddr*)&client_addr, &client_addr_len);
        
        if(len < 0) {
            if(errno == EAGAIN || errno == EWOULDBLOCK) {
                vTaskDelay(10 / portTICK_PERIOD_MS);
                continue;
            }
            ESP_LOGE(TAG, "recvfrom error: %d", errno);
            continue;
        }
        
        if(len < sizeof(dns_header_t)) {
            continue;
        }
        
        query_count++;
        
        dns_header_t* header = (dns_header_t*)rx_buffer;
        
        // Only process queries (not responses)
        if((ntohs(header->flags) & 0x8000) == 0 && ntohs(header->qdcount) > 0) {
            char domain[256];
            int offset = sizeof(dns_header_t);
            
            // Parse the domain name
            int new_offset = parse_domain_name(rx_buffer, offset, domain, sizeof(domain));
            
            if(new_offset > 0 && strlen(domain) > 0) {
                char client_ip[16];
                inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
                
                ESP_LOGI(TAG, "Query #%d from %s: %s", query_count, client_ip, domain);
                
                // Check if domain is blocked
                if(is_domain_blocked(domain)) {
                    ESP_LOGW(TAG, ">>> BLOCKING: %s <<<", domain);
                    
                    char log_msg[300];
                    snprintf(log_msg, sizeof(log_msg), "BLOCKED: %s", domain);
                    add_log_line(log_msg);
                    
                    // Create blocked response (0.0.0.0)
                    int response_len = create_blocked_response(rx_buffer, len, tx_buffer);
                    
                    // Send blocked response back to client
                    int sent = sendto(sock, tx_buffer, response_len, 0, 
                          (struct sockaddr*)&client_addr, client_addr_len);
                    
                    if(sent < 0) {
                        ESP_LOGE(TAG, "Failed to send blocked response");
                    } else {
                        ESP_LOGI(TAG, "Sent blocked response (%d bytes)", sent);
                    }
                } else {
                    // Forward to upstream DNS server
                    ESP_LOGI(TAG, "Forwarding: %s", domain);
                    
                    int response_len = forward_dns_query(rx_buffer, len, tx_buffer, sizeof(tx_buffer));
                    
                    if(response_len > 0) {
                        // Forward the response back to the client
                        int sent = sendto(sock, tx_buffer, response_len, 0, 
                              (struct sockaddr*)&client_addr, client_addr_len);
                        
                        if(sent < 0) {
                            ESP_LOGE(TAG, "Failed to send forwarded response");
                        } else {
                            ESP_LOGI(TAG, "Forwarded response (%d bytes)", sent);
                        }
                    } else {
                        ESP_LOGW(TAG, "Failed to get response from upstream DNS for: %s", domain);
                    }
                }
            }
        }
    }
    
    close(sock);
    vTaskDelete(NULL);
}

void start_dns_server(void) {
    ESP_LOGI(TAG, "Creating DNS server task...");
    
    TaskHandle_t dns_task_handle = NULL;
    BaseType_t result = xTaskCreate(
        dns_server_task, 
        "dns_server", 
        8192,  // Increased stack size
        NULL, 
        5,     // Priority
        &dns_task_handle
    );
    
    if(result == pdPASS && dns_task_handle != NULL) {
        ESP_LOGI(TAG, "DNS server task created successfully, handle: %p", dns_task_handle);
    } else {
        ESP_LOGE(TAG, "FAILED to create DNS server task! Error code: %d", result);
    }
}
