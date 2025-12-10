#ifndef DOMAIN_FILTER_H
#define DOMAIN_FILTER_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_BLOCKED_DOMAINS 50
#define MAX_DOMAIN_LENGTH 128

typedef struct {
    char domain[MAX_DOMAIN_LENGTH];
    bool active;
} blocked_domain_t;

extern blocked_domain_t blocked_domains[MAX_BLOCKED_DOMAINS];
extern int blocked_domain_count;

// Function declarations
bool is_domain_blocked(const char* domain);
bool add_blocked_domain(const char* domain);
bool remove_blocked_domain(const char* domain);
void init_domain_filter(void);
int get_blocked_domains_count(void);

#endif // DOMAIN_FILTER_H