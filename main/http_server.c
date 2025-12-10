/* Simple HTTP Server Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <esp_timer.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "esp_netif.h"
#include "esp_http_server.h"

#include "pages.h"
#include "router_globals.h"
#include "cJSON.h"

#include "log_buffer.h"
#include "domain_filter.h"


static const char *TAG = "HTTPServer";

////////////////////////////////////////////////////////


////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
esp_timer_handle_t restart_timer;

static void restart_timer_callback(void* arg)
{
    ESP_LOGI(TAG, "Restarting now...");
    esp_restart();
}

esp_timer_create_args_t restart_timer_args = {
    .callback = &restart_timer_callback,
    .arg = (void*) 0,
    .name = "restart_timer"
};

/* An HTTP GET handler */
static esp_err_t index_get_handler(httpd_req_t *req)
{
    char* buf;
    size_t buf_len;

    buf_len = httpd_req_get_hdr_value_len(req, "Host") + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_hdr_value_str(req, "Host", buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found header => Host: %s", buf);
        }
        free(buf);
    }

    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found URL query => %s", buf);

            // Example: handle reset query
            if (strcmp(buf, "reset=Reboot") == 0) {
                esp_timer_start_once(restart_timer, 500000);
            }

            // Handle other URL parameters (Wi-Fi, AP, static IP, etc.)
            char param1[64], param2[64], param3[64], param4[64];
            if (httpd_query_key_value(buf, "ap_ssid", param1, sizeof(param1)) == ESP_OK) {
                preprocess_string(param1);
                if (httpd_query_key_value(buf, "ap_password", param2, sizeof(param2)) == ESP_OK) {
                    preprocess_string(param2);
                    char* argv[3] = {"set_ap", param1, param2};
                    set_ap(3, argv);
                    esp_timer_start_once(restart_timer, 500000);
                }
            }
            if (httpd_query_key_value(buf, "ssid", param1, sizeof(param1)) == ESP_OK) {
                preprocess_string(param1);
                if (httpd_query_key_value(buf, "password", param2, sizeof(param2)) == ESP_OK) {
                    preprocess_string(param2);
                    if (httpd_query_key_value(buf, "ent_username", param3, sizeof(param3)) == ESP_OK) {
                        preprocess_string(param3);
                        if (httpd_query_key_value(buf, "ent_identity", param4, sizeof(param4)) == ESP_OK) {
                            preprocess_string(param4);
                            char* argv[7];
                            int argc = 0;
                            argv[argc++] = "set_sta";
                            argv[argc++] = param1;
                            argv[argc++] = param2;
                            if(strlen(param2)) { argv[argc++] = "-u"; argv[argc++] = param3; }
                            if(strlen(param3)) { argv[argc++] = "-a"; argv[argc++] = param4; }
                            set_sta(argc, argv);
                            esp_timer_start_once(restart_timer, 500000);
                        }
                    }
                }
            }
            if (httpd_query_key_value(buf, "staticip", param1, sizeof(param1)) == ESP_OK) {
                preprocess_string(param1);
                if (httpd_query_key_value(buf, "subnetmask", param2, sizeof(param2)) == ESP_OK) {
                    preprocess_string(param2);
                    if (httpd_query_key_value(buf, "gateway", param3, sizeof(param3)) == ESP_OK) {
                        preprocess_string(param3);
                        char* argv[4] = {"set_sta_static", param1, param2, param3};
                        set_sta_static(4, argv);
                        esp_timer_start_once(restart_timer, 500000);
                    }
                }
            }
        }
        free(buf);
    }

    const char* resp_str = (const char*) req->user_ctx;
    httpd_resp_send(req, resp_str, strlen(resp_str));
    return ESP_OK;
}

static esp_err_t inspect_packet_handler(httpd_req_t *req)
{
    httpd_resp_set_status(req, "302 Found");
    httpd_resp_set_hdr(req, "Location", "/dashboard");
    httpd_resp_send(req, NULL, 0);
    return ESP_OK;
}

static httpd_uri_t indexp = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_get_handler
};

static httpd_uri_t inspect_packet_uri = {
    .uri       = "/inspect_packet",
    .method    = HTTP_GET,
    .handler   = inspect_packet_handler,
};

static esp_err_t dashboard_get_handler(httpd_req_t *req)
{
    httpd_resp_send(req, DASHBOARD_PAGE, strlen(DASHBOARD_PAGE));
    return ESP_OK;
}

static httpd_uri_t dashboard_uri = {
    .uri       = "/dashboard",
    .method    = HTTP_GET,
    .handler   = dashboard_get_handler,
};

esp_err_t logs_data_handler(httpd_req_t *req)
{
    char line[LOG_LINE_MAX_LEN + 10];
    for (int i = 0; i < LOG_BUFFER_SIZE; i++) {
        int idx = (log_index + i) % LOG_BUFFER_SIZE;
        if (strlen(log_buffer[idx]) > 0) {
            snprintf(line, sizeof(line), "%s\n", log_buffer[idx]);
            httpd_resp_sendstr_chunk(req, line);
        }
    }
    httpd_resp_sendstr_chunk(req, NULL); // end
    return ESP_OK;
}

httpd_uri_t logs_uri = {
    .uri       = "/logs",
    .method    = HTTP_GET,
    .handler   = logs_data_handler,
    .user_ctx  = NULL
};
esp_err_t get_devices_handler(httpd_req_t *req)
{
    cJSON *root = cJSON_CreateArray();
    
    for(int i = 0; i < device_count; i++){
        cJSON *dev = cJSON_CreateObject();

        char mac_str[18];
        sprintf(mac_str, "%02X:%02X:%02X:%02X:%02X:%02X",
                device_list[i].mac[0], device_list[i].mac[1],
                device_list[i].mac[2], device_list[i].mac[3],
                device_list[i].mac[4], device_list[i].mac[5]);
        
        cJSON_AddStringToObject(dev, "mac", mac_str);
        
        switch(device_list[i].status){
            case PENDING: 
                cJSON_AddStringToObject(dev, "status", "PENDING"); 
                break;
            case ALLOWED: 
                cJSON_AddStringToObject(dev, "status", "ALLOWED"); 
                break;
            case BLOCKED: 
                cJSON_AddStringToObject(dev, "status", "BLOCKED"); 
                break;
        }
        
        cJSON_AddItemToArray(root, dev);
    }

    char *resp = cJSON_Print(root);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, resp, strlen(resp));

    free(resp);
    cJSON_Delete(root);
    return ESP_OK;
}



/////////////////////////////////////////////DOMAIN HANDLER/////////////////////////////////////////////
esp_err_t get_blocked_domains_handler(httpd_req_t *req)
{
    cJSON *root = cJSON_CreateArray();
    
    for(int i = 0; i < blocked_domain_count; i++){
        if(blocked_domains[i].active && strlen(blocked_domains[i].domain) > 0) {
            cJSON *domain_obj = cJSON_CreateObject();
            cJSON_AddStringToObject(domain_obj, "domain", blocked_domains[i].domain);
            cJSON_AddItemToArray(root, domain_obj);
        }
    }

    char *resp = cJSON_Print(root);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, resp, strlen(resp));

    free(resp);
    cJSON_Delete(root);
    return ESP_OK;
}

// Handler to add a blocked domain
esp_err_t add_blocked_domain_handler(httpd_req_t *req)
{
    char buf[256];
    int ret = httpd_req_recv(req, buf, sizeof(buf));
    if(ret <= 0) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid request");
        return ESP_FAIL;
    }
    buf[ret] = 0;

    cJSON *json = cJSON_Parse(buf);
    if(!json) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid JSON");
        return ESP_FAIL;
    }

    cJSON *domain_item = cJSON_GetObjectItem(json, "domain");
    
    if(!domain_item || !domain_item->valuestring) {
        cJSON_Delete(json);
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Missing domain parameter");
        return ESP_FAIL;
    }

    const char *domain_str = domain_item->valuestring;
    
    if(add_blocked_domain(domain_str)) {
        httpd_resp_sendstr(req, "OK");
    } else {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to add domain");
    }
    
    cJSON_Delete(json);
    return ESP_OK;
}

// Handler to remove a blocked domain
esp_err_t remove_blocked_domain_handler(httpd_req_t *req)
{
    char buf[256];
    int ret = httpd_req_recv(req, buf, sizeof(buf));
    if(ret <= 0) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid request");
        return ESP_FAIL;
    }
    buf[ret] = 0;

    cJSON *json = cJSON_Parse(buf);
    if(!json) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid JSON");
        return ESP_FAIL;
    }

    cJSON *domain_item = cJSON_GetObjectItem(json, "domain");
    
    if(!domain_item || !domain_item->valuestring) {
        cJSON_Delete(json);
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Missing domain parameter");
        return ESP_FAIL;
    }

    const char *domain_str = domain_item->valuestring;
    
    if(remove_blocked_domain(domain_str)) {
        httpd_resp_sendstr(req, "OK");
    } else {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Domain not found");
    }
    
    cJSON_Delete(json);
    return ESP_OK;
}
// ============================================================================
// For http_server.c - update_mac_handler function
// ============================================================================

// Replace the update_mac_handler function with this version:

esp_err_t update_mac_handler(httpd_req_t *req)
{
    char buf[128];
    int ret = httpd_req_recv(req, buf, sizeof(buf));
    if(ret <= 0) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid request");
        return ESP_FAIL;
    }
    buf[ret] = 0;

    cJSON *json = cJSON_Parse(buf);
    if(!json) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid JSON");
        return ESP_FAIL;
    }

    cJSON *mac_item = cJSON_GetObjectItem(json, "mac");
    cJSON *status_item = cJSON_GetObjectItem(json, "status");
    
    if(!mac_item || !status_item) {
        cJSON_Delete(json);
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Missing parameters");
        return ESP_FAIL;
    }

    const char *mac_str = mac_item->valuestring;
    const char *status_str = status_item->valuestring;

    uint8_t mac[6];
    sscanf(mac_str, "%hhX:%hhX:%hhX:%hhX:%hhX:%hhX",
           &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);

    bool found = false;
    for(int i = 0; i < device_count; i++){
        if(memcmp(device_list[i].mac, mac, 6) == 0){
            found = true;
            if(strcmp(status_str, "ALLOWED") == 0){
                device_list[i].status = ALLOWED;
                ESP_LOGI(TAG, "MAC allowed");
                add_log_line("MAC allowed:");
                add_log_line(mac_str);
            } else if(strcmp(status_str, "BLOCKED") == 0){
                device_list[i].status = BLOCKED;
                ESP_LOGI(TAG, "MAC blocked - kicking device");
                add_log_line("MAC blocked:");
                add_log_line(mac_str);
                
                // Kick device if currently connected
                // In ESP-IDF 5.1.2, we need to find the device and use its AID
                wifi_sta_list_t wifi_sta_list;
                memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
                
                esp_err_t err = esp_wifi_ap_get_sta_list(&wifi_sta_list);
                if(err == ESP_OK) {
                    // Look for the device in connected stations
                    for(int j = 0; j < wifi_sta_list.num; j++) {
                        if(memcmp(wifi_sta_list.sta[j].mac, device_list[i].mac, 6) == 0) {
                            // Disconnect all stations and they'll reconnect
                            // Blocked device will be rejected on reconnect
                            // This is a workaround since we can't get AID from sta list
                            ESP_LOGI(TAG, "Device will be blocked on next connection attempt");
                            
                            // Alternative: Use disconnect by setting max_connection to current-1
                            // This forces a re-evaluation of connections
                            break;
                        }
                    }
                } else {
                    ESP_LOGW(TAG, "Failed to get station list");
                }
            }
            break;
        }
    }

    cJSON_Delete(json);
    
    if(found) {
        httpd_resp_sendstr(req, "OK");
    } else {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "MAC not found");
    }
    
    return ESP_OK;
}
typedef esp_err_t (*real_handler_t)(httpd_req_t *req);

static void get_client_ip(httpd_req_t *req, char *ip_str, size_t len) {
    int sock = httpd_req_to_sockfd(req);
    if (sock >= 0) {
        struct sockaddr_in addr;
        socklen_t addr_len = sizeof(addr);
        if (getpeername(sock, (struct sockaddr *)&addr, &addr_len) == 0) {
            strncpy(ip_str, inet_ntoa(addr.sin_addr), len);
        }
    }
}
esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Page not found");
    return ESP_FAIL;
}

char* html_escape(const char* src) {
    int len = strlen(src);
    int esc_len = len + 1;
    for (int i = 0; i < len; i++) {
        if (src[i] == '\\' || src[i] == '\'' || src[i] == '\"' || src[i] == '&' || src[i] == '#' || src[i] == ';') {
            esc_len += 4;
        }
    }

    char* res = malloc(sizeof(char) * esc_len);
    int j = 0;
    for (int i = 0; i < len; i++) {
        if (src[i] == '\\' || src[i] == '\'' || src[i] == '\"' || src[i] == '&' || src[i] == '#' || src[i] == ';') {
            res[j++] = '&';
            res[j++] = '#';
            res[j++] = '0' + (src[i] / 10);
            res[j++] = '0' + (src[i] % 10);
            res[j++] = ';';
        } else {
            res[j++] = src[i];
        }
    }
    res[j] = '\0';
    return res;
}

httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    const char* config_page_template = CONFIG_PAGE;

    char* safe_ap_ssid = html_escape(ap_ssid);
    char* safe_ap_passwd = html_escape(ap_passwd);
    char* safe_ssid = html_escape(ssid);
    char* safe_passwd = html_escape(passwd);
    char* safe_ent_username = html_escape(ent_username);
    char* safe_ent_identity = html_escape(ent_identity);

    int page_len =
        strlen(config_page_template) +
        strlen(safe_ap_ssid) +
        strlen(safe_ap_passwd) +
        strlen(safe_ssid) +
        strlen(safe_passwd) +
        strlen(safe_ent_username) +
        strlen(safe_ent_identity) +
        256;
    char* config_page = malloc(sizeof(char) * page_len);

    snprintf(
        config_page, page_len, config_page_template,
        safe_ap_ssid, safe_ap_passwd,
        safe_ssid, safe_passwd, safe_ent_username, safe_ent_identity,
        static_ip, subnet_mask, gateway_addr);
    indexp.user_ctx = config_page;

    free(safe_ap_ssid);
    free(safe_ap_passwd);
    free(safe_ssid);
    free(safe_passwd);
    free(safe_ent_username);
    free(safe_ent_identity);

    esp_timer_create(&restart_timer_args, &restart_timer);

    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &indexp);
        httpd_register_uri_handler(server, &inspect_packet_uri);
        httpd_register_uri_handler(server, &logs_uri);
        httpd_register_uri_handler(server, &dashboard_uri);
        // MAC Filtering endpoints
        httpd_uri_t get_devices_uri = {
            .uri = "/devices",
            .method = HTTP_GET,
            .handler = get_devices_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &get_devices_uri);

        httpd_uri_t update_mac_uri = {
            .uri = "/update_mac",
            .method = HTTP_POST,
            .handler = update_mac_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &update_mac_uri);


        ///////

       httpd_uri_t get_blocked_domains_uri = {
        .uri = "/blocked_domains",
        .method = HTTP_GET,
        .handler = get_blocked_domains_handler,
        .user_ctx = NULL
};
        httpd_register_uri_handler(server, &get_blocked_domains_uri);

        httpd_uri_t add_blocked_domain_uri = {
        .uri = "/add_domain",
        .method = HTTP_POST,
        .handler = add_blocked_domain_handler,
        .user_ctx = NULL
};
    httpd_register_uri_handler(server, &add_blocked_domain_uri);

httpd_uri_t remove_blocked_domain_uri = {
        .uri = "/remove_domain",
        .method = HTTP_POST,
        .handler = remove_blocked_domain_handler,
        .user_ctx = NULL
};
httpd_register_uri_handler(server, &remove_blocked_domain_uri);

        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

static void stop_webserver(httpd_handle_t server)
{
    httpd_stop(server);
}
