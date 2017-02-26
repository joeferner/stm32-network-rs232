
#ifndef _NETWORK_H_
#define _NETWORK_H_

void network_setup();
void network_tick();
void network_dhcpRenew();
void network_ifconfig();
void network_sendToAllTelnetConnections(const char* line);

extern void network_handleTelnetReceive(const char* line);

#endif
