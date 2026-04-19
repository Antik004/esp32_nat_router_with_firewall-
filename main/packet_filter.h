#ifndef PACKET_FILTER_H
#define PACKET_FILTER_H

#include <stdint.h>
#include <stdbool.h>
#include "lwip/ip.h"
#include "lwip/ip4.h"
#include "router_globals.h"

// Initialize packet filter
void init_packet_filter(void);

// Core filtering logic
bool is_packet_allowed(uint8_t* src_mac, uint8_t protocol, uint16_t src_port, uint16_t dst_port);

#endif