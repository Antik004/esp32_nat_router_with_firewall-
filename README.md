This project extends the open-source ESP32 NAT router implementation of Martinger's ESP 32 NAT router by adding a custom firewall layer, real-time device monitoring, and domain filtering.

DNS-Based Domain Blocking
Intercepts DNS queries
Matches against blocked domain list
Returns 0.0.0.0 for blocked domains
Supports wildcard patterns (e.g., *.youtube.com)
 //ignore commit messages, domain blocking has already been implemented
