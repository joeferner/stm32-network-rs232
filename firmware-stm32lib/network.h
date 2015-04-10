
#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "platform_config.h"
#include <sys/process.h>

PROCESS_NAME(dhcp_process);

void network_setup();
void network_tick();

#endif
