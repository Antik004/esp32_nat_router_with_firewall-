#include "domain_filter.h"
#include <string.h>
#include <ctype.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "router_globals.h"
#include "log_buffer.h"

static const char *TAG = "DomainFilter";

blocked_domain_t blocked_domains[MAX_BLOCKED_DOMAINS];
int blocked_domain_count = 0;

// Helper function to convert domain to lowercase
static void to_lowercase(char* str) {
    for(int i = 0; str[i]; i++){
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Check if a domain matches (supports wildcards like *.youtube.com)
static bool domain_matches(const char* query, const char* pattern) {
    // Convert to lowercase for comparison
    char query_lower[MAX_DOMAIN_LENGTH];
    char pattern_lower[MAX_DOMAIN_LENGTH];
    
    strncpy(query_lower, query, MAX_DOMAIN_LENGTH - 1);
    strncpy(pattern_lower, pattern, MAX_DOMAIN_LENGTH - 1);
    query_lower[MAX_DOMAIN_LENGTH - 1] = '\0';
    pattern_lower[MAX_DOMAIN_LENGTH - 1] = '\0';
    
    to_lowercase(query_lower);
    to_lowercase(pattern_lower);
    
    // Exact match
    if(strcmp(query_lower, pattern_lower) == 0) {
        return true;
    }
    
    // Wildcard match (*.example.com)
    if(pattern_lower[0] == '*' && pattern_lower[1] == '.') {
        const char* pattern_suffix = pattern_lower + 2;
        size_t query_len = strlen(query_lower);
        size_t suffix_len = strlen(pattern_suffix);
        
        if(query_len >= suffix_len) {
            // Check if query ends with pattern suffix
            if(strcmp(query_lower + (query_len - suffix_len), pattern_suffix) == 0) {
                // Make sure there's a dot before the suffix or it's exact match
                if(query_len == suffix_len || query_lower[query_len - suffix_len - 1] == '.') {
                    return true;
                }
            }
        }
    }
    
    // Check if query is subdomain of pattern
    size_t query_len = strlen(query_lower);
    size_t pattern_len = strlen(pattern_lower);
    
    if(query_len > pattern_len) {
        if(strcmp(query_lower + (query_len - pattern_len), pattern_lower) == 0) {
            if(query_lower[query_len - pattern_len - 1] == '.') {
                return true;
            }
        }
    }
    
    return false;
}

bool is_domain_blocked(const char* domain) {
    if(!domain || strlen(domain) == 0) {
        return false;
    }
    
    for(int i = 0; i < blocked_domain_count; i++) {
        if(blocked_domains[i].active && domain_matches(domain, blocked_domains[i].domain)) {
            ESP_LOGI(TAG, "Domain blocked: %s (matches %s)", domain, blocked_domains[i].domain);
            return true;
        }
    }
    
    return false;
}

bool add_blocked_domain(const char* domain) {
    if(!domain || strlen(domain) == 0 || strlen(domain) >= MAX_DOMAIN_LENGTH) {
        return false;
    }
    
    // Check if already exists
    for(int i = 0; i < blocked_domain_count; i++) {
        if(strcasecmp(blocked_domains[i].domain, domain) == 0) {
            blocked_domains[i].active = true;
            return true;
        }
    }
    
    // Add new domain
    if(blocked_domain_count < MAX_BLOCKED_DOMAINS) {
        strncpy(blocked_domains[blocked_domain_count].domain, domain, MAX_DOMAIN_LENGTH - 1);
        blocked_domains[blocked_domain_count].domain[MAX_DOMAIN_LENGTH - 1] = '\0';
        blocked_domains[blocked_domain_count].active = true;
        blocked_domain_count++;
        
        ESP_LOGI(TAG, "Added blocked domain: %s", domain);
        add_log_line("Blocked domain added:");
        add_log_line(domain);
        
        // Save to NVS
        nvs_handle_t nvs;
        esp_err_t err = nvs_open(PARAM_NAMESPACE, NVS_READWRITE, &nvs);
        if(err == ESP_OK) {
            err = nvs_set_blob(nvs, "blocked_domains", blocked_domains, 
                              sizeof(blocked_domain_t) * MAX_BLOCKED_DOMAINS);
            if(err == ESP_OK) {
                nvs_commit(nvs);
            }
            nvs_close(nvs);
        }
        
        return true;
    }
    
    ESP_LOGW(TAG, "Cannot add domain: list full");
    return false;
}

bool remove_blocked_domain(const char* domain) {
    if(!domain || strlen(domain) == 0) {
        return false;
    }
    
    for(int i = 0; i < blocked_domain_count; i++) {
        if(strcasecmp(blocked_domains[i].domain, domain) == 0) {
            blocked_domains[i].active = false;
            
            ESP_LOGI(TAG, "Removed blocked domain: %s", domain);
            add_log_line("Blocked domain removed:");
            add_log_line(domain);
            
            // Save to NVS
            nvs_handle_t nvs;
            esp_err_t err = nvs_open(PARAM_NAMESPACE, NVS_READWRITE, &nvs);
            if(err == ESP_OK) {
                err = nvs_set_blob(nvs, "blocked_domains", blocked_domains, 
                                  sizeof(blocked_domain_t) * MAX_BLOCKED_DOMAINS);
                if(err == ESP_OK) {
                    nvs_commit(nvs);
                }
                nvs_close(nvs);
            }
            
            return true;
        }
    }
    
    return false;
}

void init_domain_filter(void) {
    memset(blocked_domains, 0, sizeof(blocked_domains));
    blocked_domain_count = 0;
    
    // Load from NVS
    nvs_handle_t nvs;
    esp_err_t err = nvs_open(PARAM_NAMESPACE, NVS_READONLY, &nvs);
    if(err == ESP_OK) {
        size_t len = sizeof(blocked_domain_t) * MAX_BLOCKED_DOMAINS;
        err = nvs_get_blob(nvs, "blocked_domains", blocked_domains, &len);
        if(err == ESP_OK) {
            // Count active domains
            blocked_domain_count = 0;
            for(int i = 0; i < MAX_BLOCKED_DOMAINS; i++) {
                if(blocked_domains[i].active && strlen(blocked_domains[i].domain) > 0) {
                    blocked_domain_count = i + 1;
                }
            }
            ESP_LOGI(TAG, "Loaded %d blocked domains from NVS", blocked_domain_count);
        }
        nvs_close(nvs);
    }
}

int get_blocked_domains_count(void) {
    int count = 0;
    for(int i = 0; i < blocked_domain_count; i++) {
        if(blocked_domains[i].active) {
            count++;
        }
    }
    return count;
}