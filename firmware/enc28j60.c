#include "enc28j60.h"

void enc28j60_setup(uint8_t* macAddress, uip_ipaddr_t* ipaddr, uip_ipaddr_t* gatewayAddr, uip_ipaddr_t* netmaskAddr) {
  uip_sethostaddr(ipaddr);
  uip_setdraddr(gatewayAddr);
  uip_setnetmask(netmaskAddr);
}
