
#ifndef _ENC28J60_H_
#define _ENC28J60_H_

#include "uip/uip.h"

void enc28j60_setup(uint8_t* macAddress, uip_ipaddr_t* ipaddr, uip_ipaddr_t* gatewayAddr, uip_ipaddr_t* netmaskAddr);

#endif // _ENC28J60_H_
