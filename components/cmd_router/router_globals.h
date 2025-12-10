/* Various global declarations for the esp32_nat_router

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define PARAM_NAMESPACE "esp32_nat"

#define PROTO_TCP 6
#define PROTO_UDP 17

extern char* ssid;
extern char* ent_username;
extern char* ent_identity;
extern char* passwd;
extern char* static_ip;
extern char* subnet_mask;
extern char* gateway_addr;
extern char* ap_ssid;
extern char* ap_passwd;

extern uint16_t connect_count;
extern bool ap_connect;

extern uint32_t my_ip;
extern uint32_t my_ap_ip;

void preprocess_string(char* str);
int set_sta(int argc, char **argv);
int set_sta_static(int argc, char **argv);
int set_ap(int argc, char **argv);

esp_err_t get_config_param_blob(char* name, uint8_t** blob, size_t blob_len);
esp_err_t get_config_param_int(char* name, int* param);
esp_err_t get_config_param_str(char* name, char** param);

void print_portmap_tab();
esp_err_t add_portmap(uint8_t proto, uint16_t mport, uint32_t daddr, uint16_t dport);
esp_err_t del_portmap(uint8_t proto, uint16_t mport);

#define MAX_DEVICES 20

typedef enum {
    PENDING, 
    ALLOWED, 
    BLOCKED
} mac_status_t;

typedef struct {
    uint8_t mac[6];
    mac_status_t status;
} device_t;

extern device_t device_list[MAX_DEVICES];
extern int device_count;

void add_device(uint8_t mac[6]);
bool mac_exists(uint8_t mac[6]);

// DNS Server function
void start_dns_server(void);

#ifdef __cplusplus
}
#endif
