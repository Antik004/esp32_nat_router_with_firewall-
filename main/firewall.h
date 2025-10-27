#ifndef FIREWALL_H
#define FIREWALL_H

#include "lwip/opt.h"
#include "lwip/ip.h"
#include "lwip/pbuf.h"
#include "lwip/netif.h"

#define MAX_BLOCKED_IPS 16

// Array to store blocked IPs
extern uint32_t blocked_ips[MAX_BLOCKED_IPS];
extern uint8_t blocked_count;

// Add or remove IP from blocklist
bool add_blocked_ip(uint32_t ip);
bool remove_blocked_ip(uint32_t ip);
bool is_ip_blocked(uint32_t ip);

// Packet filter hook
err_t firewall_hook(void *arg, struct pbuf *p, struct netif *inp);

#endif // FIREWALL_H
