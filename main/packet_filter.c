#include "packet_filter.h"
#include "service_filter.h"
#include "router_globals.h"
#include "esp_log.h"
#include "log_buffer.h"
#include <string.h>

static const char *TAG = "PacketFilter";

void init_packet_filter(void) {
    ESP_LOGI(TAG, "Initializing packet filter...");
    add_log_line("Packet filter initialized");
}

bool is_packet_allowed(uint8_t* src_mac, uint8_t protocol, uint16_t src_port, uint16_t dst_port) {

    // ------------------------------------------------------------------
    // Layer 1: Admin MAC bypass – full access
    // ------------------------------------------------------------------
    uint8_t admin_mac[6] = {0x8C, 0x1D, 0x96, 0x5F, 0x7D, 0xA5};
    if(memcmp(src_mac, admin_mac, 6) == 0) {
        return true;
    }

    // ------------------------------------------------------------------
    // Layer 2: Device status lookup
    // ------------------------------------------------------------------
    mac_status_t status = PENDING;
    for(int i = 0; i < device_count; i++) {
        if(memcmp(device_list[i].mac, src_mac, 6) == 0) {
            status = device_list[i].status;
            break;
        }
    }

    // ------------------------------------------------------------------
    // Layer 3: BLOCKED device → drop everything
    // ------------------------------------------------------------------
    if(status == BLOCKED) {
        ESP_LOGD(TAG, "BLOCKED device – drop proto=%d port=%d", protocol, dst_port);
        return false;
    }

    // ------------------------------------------------------------------
    // Layer 4: Service-based filtering (MAIN FIREWALL)
    // ------------------------------------------------------------------
    if(protocol == IP_PROTO_TCP || protocol == IP_PROTO_UDP) {
        if(!service_is_port_allowed(protocol, dst_port)) {
            ESP_LOGD(TAG, "Service BLOCK proto=%d port=%d", protocol, dst_port);
            add_log_line("Service BLOCK proto=%d port=%d", protocol, dst_port);
            return false;
        }
    }

    // ------------------------------------------------------------------
    // ALLOWED or PENDING → permit if passed service rules
    // ------------------------------------------------------------------
    return true;
}