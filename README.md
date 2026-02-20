This project extends the open-source ESP32 NAT router implementation of Martinger's ESP 32 NAT router by adding a custom firewall layer, real-time device monitoring, and domain filtering.

DNS-Based Domain Blocking
Intercepts DNS queries
Matches against blocked domain list
Returns 0.0.0.0 for blocked domains
Supports wildcard patterns (e.g., *.youtube.com)
 //ignore commit messages, domain blocking has already been implemented



Note:-The current version of the orignal code is also available with a firewall now, this version of mine works on an older espidf version and has an extra functionality of domain filtering
however this can be bypassed as of now if DoH(DNS over HTTPS) is used and the MAC filtering is also bypassable if the client uses MAC spoofing.


This is a built for a test and project purpose, not meant for usage in practical scenario as of now as it has some critical vulnerabilities as stated later in the above note.
