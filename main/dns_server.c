#include <string.h>
#include <sys/socket.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "domain_filter.h"
#include "log_buffer.h"

#define DNS_PORT 53
#define DNS_MAX_PACKET_SIZE 512

static const char *TAG = "DNS_Server";

// DNS Header structure
typedef struct {
    uint16_t id;
    uint16_t flags;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;
} __attribute__((packed)) dns_header_t;

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
    header->flags = htons(0x8183); // Standard query response, no error
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

// DNS server task
void dns_server_task(void *pvParameters) {
    int sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    uint8_t rx_buffer[DNS_MAX_PACKET_SIZE];
    uint8_t tx_buffer[DNS_MAX_PACKET_SIZE];
    
    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sock < 0) {
        ESP_LOGE(TAG, "Failed to create socket");
        vTaskDelete(NULL);
        return;
    }
    
    // Bind to DNS port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(DNS_PORT);
    
    if(bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        ESP_LOGE(TAG, "Failed to bind socket");
        close(sock);
        vTaskDelete(NULL);
        return;
    }
    
    ESP_LOGI(TAG, "DNS server started on port %d", DNS_PORT);
    add_log_line("DNS filter server started");
    
    while(1) {
        int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer), 0, 
                          (struct sockaddr*)&client_addr, &client_addr_len);
        
        if(len < sizeof(dns_header_t)) {
            continue;
        }
        
        dns_header_t* header = (dns_header_t*)rx_buffer;
        
        // Only process queries
        if((ntohs(header->flags) & 0x8000) == 0 && ntohs(header->qdcount) > 0) {
            char domain[256];
            int offset = sizeof(dns_header_t);
            
            // Parse the domain name
            int new_offset = parse_domain_name(rx_buffer, offset, domain, sizeof(domain));
            
            if(new_offset > 0 && strlen(domain) > 0) {
                ESP_LOGI(TAG, "DNS query for: %s", domain);
                
                // Check if domain is blocked
                if(is_domain_blocked(domain)) {
                    ESP_LOGW(TAG, "Blocking domain: %s", domain);
                    
                    char log_msg[300];
                    snprintf(log_msg, sizeof(log_msg), "BLOCKED: %s", domain);
                    add_log_line(log_msg);
                    
                    // Create blocked response
                    int response_len = create_blocked_response(rx_buffer, len, tx_buffer);
                    
                    // Send response
                    sendto(sock, tx_buffer, response_len, 0, 
                          (struct sockaddr*)&client_addr, client_addr_len);
                } else {
                    // Forward to upstream DNS server
                    // This is a simplified version - you might want to implement proper DNS forwarding
                    ESP_LOGI(TAG, "Allowing domain: %s", domain);
                }
            }
        }
    }
    
    close(sock);
    vTaskDelete(NULL);
}

void start_dns_server(void) {
    xTaskCreate(dns_server_task, "dns_server", 4096, NULL, 5, NULL);
}