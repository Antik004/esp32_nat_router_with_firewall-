#include "firewall.h"
#include <string.h>
#include <stdio.h>

uint32_t blocked_ips[MAX_BLOCKED_IPS];
uint8_t blocked_count = 0;

bool add_blocked_ip(uint32_t ip) {
    if (blocked_count >= MAX_BLOCKED_IPS) return false;
    for (int i = 0; i < blocked_count; i++) {
        if (blocked_ips[i] == ip) return false; // Already blocked
    }
    blocked_ips[blocked_count++] = ip;
    printf("Blocked IP added: %u.%u.%u.%u\n", 
           (ip & 0xFF), (ip>>8 & 0xFF), (ip>>16 & 0xFF), (ip>>24 & 0xFF));
    return true;
}

bool remove_blocked_ip(uint32_t ip) {
    for (int i = 0; i < blocked_count; i++) {
        if (blocked_ips[i] == ip) {
            for (int j = i; j < blocked_count - 1; j++) {
                blocked_ips[j] = blocked_ips[j+1];
            }
            blocked_count--;
            printf("Blocked IP removed: %u.%u.%u.%u\n", 
                   (ip & 0xFF), (ip>>8 & 0xFF), (ip>>16 & 0xFF), (ip>>24 & 0xFF));
            return true;
        }
    }
    return false;
}

bool is_ip_blocked(uint32_t ip) {
    for (int i = 0; i < blocked_count; i++) {
        if (blocked_ips[i] == ip) return true;
    }
    return false;
}

// Hook into lwIP to drop blocked packets
err_t firewall_hook(void *arg, struct pbuf *p, struct netif *inp) {
    if (!p) return ERR_OK;

    struct ip_hdr *iphdr = (struct ip_hdr *)p->payload;
    if (!iphdr) return ERR_OK;

    uint32_t src_ip = iphdr->src.addr;
    uint32_t dst_ip = iphdr->dest.addr;

    // Drop packets if source or destination IP is blocked
    if (is_ip_blocked(src_ip) || is_ip_blocked(dst_ip)) {
        printf("Firewall blocked packet: %u.%u.%u.%u -> %u.%u.%u.%u\n",
               (src_ip & 0xFF), (src_ip>>8 & 0xFF), (src_ip>>16 & 0xFF), (src_ip>>24 & 0xFF),
               (dst_ip & 0xFF), (dst_ip>>8 & 0xFF), (dst_ip>>16 & 0xFF), (dst_ip>>24 & 0xFF));
        return ERR_ABRT; // Drop packet
    }

    return ERR_OK; // Allow packet
}
