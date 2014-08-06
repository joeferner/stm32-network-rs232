
#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <stdint.h>
#include "enc28j60.h"

extern uint8_t IP_ADDRESS[4];
extern uint8_t GATEWAY_ADDRESS[4];
extern uint8_t NETMASK_ADDRESS[4];
extern uint8_t MAC_ADDRESS[6];

void network_setup();
void network_tick();

#endif // _NETWORK_H_
