#ifndef SERVICE_FILTER_H
#define SERVICE_FILTER_H

#include <stdint.h>
#include <stdbool.h>

// -----------------------------------------------------------------------
// Limits
// -----------------------------------------------------------------------
#define MAX_CUSTOM_PORT_RULES  20
#define MAX_SERVICE_NAME_LEN   24

// Protocol tag stored in rules (matches lwIP IP_PROTO_* values)
#define SVC_PROTO_TCP   6
#define SVC_PROTO_UDP   17
#define SVC_PROTO_BOTH  0   // match both TCP and UDP

// -----------------------------------------------------------------------
// Built-in service IDs (index into g_services[])
// -----------------------------------------------------------------------
typedef enum {
    SVC_HTTP     = 0,
    SVC_HTTPS    = 1,
    SVC_DNS      = 2,
    SVC_SSH      = 3,
    SVC_FTP      = 4,
    SVC_FTP_DATA = 5,
    SVC_SMTP     = 6,
    SVC_SMTPS    = 7,
    SVC_POP3     = 8,
    SVC_IMAP     = 9,
    SVC_IMAPS    = 10,
    SVC_RDP      = 11,
    SVC_NTP      = 12,
    SVC_TELNET   = 13,
    SVC_MQTT     = 14,
    SVC_MQTTS    = 15,
    SVC_BUILTIN_COUNT
} service_id_t;

// -----------------------------------------------------------------------
// Rule structs
// -----------------------------------------------------------------------
typedef struct {
    char     name[MAX_SERVICE_NAME_LEN];
    uint16_t port;
    uint8_t  proto;    // SVC_PROTO_TCP / UDP / BOTH
    bool     blocked;  // true = DROP packets to this port
    bool     active;   // always true for built-ins
} service_rule_t;

typedef struct {
    char     name[MAX_SERVICE_NAME_LEN];
    uint16_t port;
    uint8_t  proto;
    bool     blocked;
    bool     active;   // false = empty slot
} custom_port_rule_t;

// -----------------------------------------------------------------------
// Global tables (defined in service_filter.c)
// -----------------------------------------------------------------------
extern service_rule_t     g_services[SVC_BUILTIN_COUNT];
extern custom_port_rule_t g_custom_rules[MAX_CUSTOM_PORT_RULES];
extern int                g_custom_rule_count;

// -----------------------------------------------------------------------
// API
// -----------------------------------------------------------------------

/** Call once at startup after nvs_flash_init(). */
void         init_service_filter(void);

/** Persist current state to NVS. */
void         save_service_rules(void);

/** Block / unblock a built-in service by enum ID. */
void         service_set_blocked(service_id_t id, bool blocked);

/** Find a built-in service by name (case-insensitive). Returns SVC_BUILTIN_COUNT if not found. */
service_id_t service_find_by_name(const char *name);

/** Add or update a custom rule. Returns false if the table is full. */
bool         service_add_custom(const char *name, uint16_t port, uint8_t proto, bool blocked);

/** Remove a custom rule by port+proto. Returns false if not found. */
bool         service_remove_custom(uint16_t port, uint8_t proto);

/**
 * Core decision for a single packet:
 *   true  = ALLOW
 *   false = BLOCK
 *
 * Precedence:
 *   1. Custom rules  (highest priority)
 *   2. Built-in service rules
 *   3. Default: ALLOW  (unknown ports pass through)
 */
bool service_is_port_allowed(uint8_t proto, uint16_t dst_port);

#endif // SERVICE_FILTER_H