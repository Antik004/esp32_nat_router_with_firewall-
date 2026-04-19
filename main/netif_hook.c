#include <string.h>
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_netif_net_stack.h"

#include "lwip/netif.h"
#include "lwip/ip.h"
#include "lwip/pbuf.h"
#include "lwip/prot/tcp.h"
#include "lwip/prot/udp.h"

#include "packet_filter.h"
static const char *TAG = "NETIF_HOOK";

static netif_input_fn original_ap_input = NULL;

// 🔥 Packet interception (Martin-style)
static err_t ap_netif_input_hook(struct pbuf *p, struct netif *inp)
{
    if (!p || p->len < 34) {
        return original_ap_input(p, inp);
    }

    uint8_t *frame = (uint8_t *)p->payload;

    // Ethernet header
    uint8_t *src_mac = frame + 6;

    // Only IPv4
    if (frame[12] != 0x08 || frame[13] != 0x00) {
        return original_ap_input(p, inp);
    }

    struct ip_hdr *iphdr = (struct ip_hdr *)(frame + 14);

    uint8_t proto = IPH_PROTO(iphdr);
    uint16_t src_port = 0, dst_port = 0;

    uint8_t ip_hl = IPH_HL(iphdr);

    if (proto == IP_PROTO_TCP) {
        struct tcp_hdr *tcphdr = (struct tcp_hdr *)((uint8_t*)iphdr + ip_hl*4);
        src_port = ntohs(tcphdr->src);
        dst_port = ntohs(tcphdr->dest);
    } 
    else if (proto == IP_PROTO_UDP) {
        struct udp_hdr *udphdr = (struct udp_hdr *)((uint8_t*)iphdr + ip_hl*4);
        src_port = ntohs(udphdr->src);
        dst_port = ntohs(udphdr->dest);
    }

    ESP_LOGI(TAG, "proto=%d port=%d", proto, dst_port);

    // 🔥 Your firewall logic
    if (!is_packet_allowed(src_mac, proto, src_port, dst_port)) {
        ESP_LOGW(TAG, "BLOCKED port=%d", dst_port);
        pbuf_free(p);
        return ERR_OK;
    }

    return original_ap_input(p, inp);
}

// 🔥 THIS is the esp_netif part you asked for
void init_netif_firewall(void)
{
    // Get AP interface
    esp_netif_t *ap_netif_handle = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");

    if (!ap_netif_handle) {
        ESP_LOGE(TAG, "AP netif handle not found");
        return;
    }

    // Get lwIP netif from esp_netif
    struct netif *ap_netif = esp_netif_get_netif_impl(ap_netif_handle);

    if (!ap_netif) {
        ESP_LOGE(TAG, "Failed to get lwIP netif");
        return;
    }

    // Hook it (THIS IS THE MAGIC LINE)
    original_ap_input = ap_netif->input;
    ap_netif->input = ap_netif_input_hook;

    ESP_LOGI(TAG, "🔥 NETIF firewall installed");
}