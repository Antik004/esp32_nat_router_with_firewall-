#include "service_filter.h"
#include "router_globals.h"
#include "log_buffer.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <string.h>
#include <ctype.h>
#include <inttypes.h>

static const char *TAG = "ServiceFilter";

// ---------------------------------------------------------------------------
// Built-in service table  (all unblocked by default)
// ---------------------------------------------------------------------------
service_rule_t g_services[SVC_BUILTIN_COUNT] = {
    [SVC_HTTP]     = { "HTTP",     80,   SVC_PROTO_TCP,  false, true },
    [SVC_HTTPS]    = { "HTTPS",    443,  SVC_PROTO_TCP,  false, true },
    [SVC_DNS]      = { "DNS",      53,   SVC_PROTO_UDP,  false, true },
    [SVC_SSH]      = { "SSH",      22,   SVC_PROTO_TCP,  false, true },
    [SVC_FTP]      = { "FTP",      21,   SVC_PROTO_TCP,  false, true },
    [SVC_FTP_DATA] = { "FTP-DATA", 20,   SVC_PROTO_TCP,  false, true },
    [SVC_SMTP]     = { "SMTP",     25,   SVC_PROTO_TCP,  false, true },
    [SVC_SMTPS]    = { "SMTPS",    465,  SVC_PROTO_TCP,  false, true },
    [SVC_POP3]     = { "POP3",     110,  SVC_PROTO_TCP,  false, true },
    [SVC_IMAP]     = { "IMAP",     143,  SVC_PROTO_TCP,  false, true },
    [SVC_IMAPS]    = { "IMAPS",    993,  SVC_PROTO_TCP,  false, true },
    [SVC_RDP]      = { "RDP",      3389, SVC_PROTO_TCP,  false, true },
    [SVC_NTP]      = { "NTP",      123,  SVC_PROTO_UDP,  false, true },
    [SVC_TELNET]   = { "TELNET",   23,   SVC_PROTO_TCP,  false, true },
    [SVC_MQTT]     = { "MQTT",     1883, SVC_PROTO_TCP,  false, true },
    [SVC_MQTTS]    = { "MQTTS",    8883, SVC_PROTO_TCP,  false, true },
};

// ---------------------------------------------------------------------------
// Custom port rules
// ---------------------------------------------------------------------------
custom_port_rule_t g_custom_rules[MAX_CUSTOM_PORT_RULES];
int                g_custom_rule_count = 0;

// ---------------------------------------------------------------------------
// NVS keys
// ---------------------------------------------------------------------------
#define NVS_KEY_SVC_BLOCKED  "svc_blocked"   // uint32 bitmask
#define NVS_KEY_CUSTOM_RULES "svc_custom"    // blob

// ---------------------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------------------
static bool proto_matches(uint8_t rule_proto, uint8_t pkt_proto) {
    if (rule_proto == SVC_PROTO_BOTH) return true;
    return rule_proto == pkt_proto;
}

static void to_upper(const char *src, char *dst, size_t max) {
    size_t i = 0;
    for (; i < max - 1 && src[i]; i++) {
        dst[i] = (char)toupper((unsigned char)src[i]);
    }
    dst[i] = '\0';
}

// ---------------------------------------------------------------------------
// NVS persist / load
// ---------------------------------------------------------------------------
void save_service_rules(void) {
    nvs_handle_t nvs;
    esp_err_t err = nvs_open(PARAM_NAMESPACE, NVS_READWRITE, &nvs);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS open failed: %s", esp_err_to_name(err));
        return;
    }

    // Pack blocked flags into a 32-bit bitmask (bit i = g_services[i].blocked)
    uint32_t mask = 0;
    for (int i = 0; i < SVC_BUILTIN_COUNT; i++) {
        if (g_services[i].blocked) mask |= (1u << i);
    }
    nvs_set_u32(nvs, NVS_KEY_SVC_BLOCKED, mask);
    nvs_set_blob(nvs, NVS_KEY_CUSTOM_RULES, g_custom_rules,
                 sizeof(custom_port_rule_t) * MAX_CUSTOM_PORT_RULES);
    nvs_commit(nvs);
    nvs_close(nvs);

    ESP_LOGI(TAG, "Service rules saved (mask=0x%08" PRIx32 ", custom=%d)",
             mask, g_custom_rule_count);
    add_log_line("Service rules saved");
}

static void load_service_rules(void) {
    nvs_handle_t nvs;
    esp_err_t err = nvs_open(PARAM_NAMESPACE, NVS_READONLY, &nvs);
    if (err != ESP_OK) {
        ESP_LOGI(TAG, "No saved service rules – using defaults");
        return;
    }

    uint32_t mask = 0;
    if (nvs_get_u32(nvs, NVS_KEY_SVC_BLOCKED, &mask) == ESP_OK) {
        for (int i = 0; i < SVC_BUILTIN_COUNT; i++) {
            g_services[i].blocked = (bool)((mask >> i) & 1u);
        }
        ESP_LOGI(TAG, "Loaded service block mask: 0x%08" PRIx32, mask);
    }

    size_t len = sizeof(custom_port_rule_t) * MAX_CUSTOM_PORT_RULES;
    if (nvs_get_blob(nvs, NVS_KEY_CUSTOM_RULES, g_custom_rules, &len) == ESP_OK) {
        g_custom_rule_count = 0;
        for (int i = MAX_CUSTOM_PORT_RULES - 1; i >= 0; i--) {
            if (g_custom_rules[i].active) {
                g_custom_rule_count = i + 1;
                break;
            }
        }
        ESP_LOGI(TAG, "Loaded %d custom port rules", g_custom_rule_count);
    }

    nvs_close(nvs);
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------
void init_service_filter(void) {
    memset(g_custom_rules, 0, sizeof(g_custom_rules));
    g_custom_rule_count = 0;
    load_service_rules();

    ESP_LOGI(TAG, "Service filter ready:");
    for (int i = 0; i < SVC_BUILTIN_COUNT; i++) {
        ESP_LOGI(TAG, "  %-10s  port=%-5u  %-4s  %s",
                 g_services[i].name,
                 g_services[i].port,
                 g_services[i].proto == SVC_PROTO_TCP  ? "TCP"  :
                 g_services[i].proto == SVC_PROTO_UDP  ? "UDP"  : "BOTH",
                 g_services[i].blocked ? "BLOCKED" : "allowed");
    }
    add_log_line("Service filter initialised (%d built-ins)", SVC_BUILTIN_COUNT);
}

void service_set_blocked(service_id_t id, bool blocked) {
    if ((int)id >= SVC_BUILTIN_COUNT) {
        ESP_LOGW(TAG, "service_set_blocked: invalid id %d", id);
        return;
    }
    g_services[id].blocked = blocked;
    save_service_rules();
    ESP_LOGI(TAG, "Service %s -> %s", g_services[id].name, blocked ? "BLOCKED" : "ALLOWED");
    add_log_line("Service %s: %s", g_services[id].name, blocked ? "BLOCKED" : "ALLOWED");
}

service_id_t service_find_by_name(const char *name) {
    if (!name) return SVC_BUILTIN_COUNT;
    char up_in[MAX_SERVICE_NAME_LEN];
    to_upper(name, up_in, sizeof(up_in));
    for (int i = 0; i < SVC_BUILTIN_COUNT; i++) {
        char up_svc[MAX_SERVICE_NAME_LEN];
        to_upper(g_services[i].name, up_svc, sizeof(up_svc));
        if (strcmp(up_in, up_svc) == 0) return (service_id_t)i;
    }
    return SVC_BUILTIN_COUNT;
}

bool service_add_custom(const char *name, uint16_t port, uint8_t proto, bool blocked) {
    if (!name || port == 0) return false;

    // Update if same port+proto already exists
    for (int i = 0; i < MAX_CUSTOM_PORT_RULES; i++) {
        if (!g_custom_rules[i].active) continue;
        if (g_custom_rules[i].port == port &&
            (g_custom_rules[i].proto == proto ||
             proto == SVC_PROTO_BOTH ||
             g_custom_rules[i].proto == SVC_PROTO_BOTH)) {
            g_custom_rules[i].blocked = blocked;
            g_custom_rules[i].proto   = proto;
            strncpy(g_custom_rules[i].name, name, MAX_SERVICE_NAME_LEN - 1);
            g_custom_rules[i].name[MAX_SERVICE_NAME_LEN - 1] = '\0';
            save_service_rules();
            ESP_LOGI(TAG, "Updated custom rule: %s port=%u", name, port);
            return true;
        }
    }

    // Find empty slot
    for (int i = 0; i < MAX_CUSTOM_PORT_RULES; i++) {
        if (g_custom_rules[i].active) continue;
        strncpy(g_custom_rules[i].name, name, MAX_SERVICE_NAME_LEN - 1);
        g_custom_rules[i].name[MAX_SERVICE_NAME_LEN - 1] = '\0';
        g_custom_rules[i].port    = port;
        g_custom_rules[i].proto   = proto;
        g_custom_rules[i].blocked = blocked;
        g_custom_rules[i].active  = true;
        if (i + 1 > g_custom_rule_count) g_custom_rule_count = i + 1;
        save_service_rules();
        ESP_LOGI(TAG, "Added custom rule: %s port=%u blocked=%d", name, port, blocked);
        add_log_line("Custom port rule added: %s port=%u", name, port);
        return true;
    }

    ESP_LOGW(TAG, "Custom rule table full");
    return false;
}

bool service_remove_custom(uint16_t port, uint8_t proto) {
    for (int i = 0; i < MAX_CUSTOM_PORT_RULES; i++) {
        if (!g_custom_rules[i].active) continue;
        if (g_custom_rules[i].port != port) continue;
        if (proto != SVC_PROTO_BOTH &&
            g_custom_rules[i].proto != proto &&
            g_custom_rules[i].proto != SVC_PROTO_BOTH) continue;

        ESP_LOGI(TAG, "Removing custom rule: %s port=%u", g_custom_rules[i].name, port);
        add_log_line("Custom port rule removed: %s port=%u", g_custom_rules[i].name, port);
        memset(&g_custom_rules[i], 0, sizeof(custom_port_rule_t));

        // Recalculate count
        g_custom_rule_count = 0;
        for (int j = MAX_CUSTOM_PORT_RULES - 1; j >= 0; j--) {
            if (g_custom_rules[j].active) { g_custom_rule_count = j + 1; break; }
        }
        save_service_rules();
        return true;
    }
    return false;
}

bool service_is_port_allowed(uint8_t proto, uint16_t dst_port) {
    // 1. Custom rules – highest priority
    for (int i = 0; i < g_custom_rule_count; i++) {
        if (!g_custom_rules[i].active) continue;
        if (g_custom_rules[i].port == dst_port &&
            proto_matches(g_custom_rules[i].proto, proto)) {
            bool allow = !g_custom_rules[i].blocked;
            ESP_LOGD(TAG, "Custom rule [%s] port=%u -> %s",
                     g_custom_rules[i].name, dst_port, allow ? "ALLOW" : "BLOCK");
            return allow;
        }
    }

    // 2. Built-in service rules
    for (int i = 0; i < SVC_BUILTIN_COUNT; i++) {
        if (!g_services[i].active) continue;
        if (g_services[i].port == dst_port &&
            proto_matches(g_services[i].proto, proto)) {
            bool allow = !g_services[i].blocked;
            ESP_LOGD(TAG, "Service rule [%s] port=%u -> %s",
                     g_services[i].name, dst_port, allow ? "ALLOW" : "BLOCK");
            return allow;
        }
    }

    // 3. Default: allow unknown ports
    return true;
}