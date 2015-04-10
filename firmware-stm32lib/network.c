
#include "network.h"
#include <sys/etimer.h>
#include <net/ip/uip.h>
#include <net/ipv4/uip_arp.h>
#include <net/ip/dhcpc.h>
#include <net/ip/resolv.h>
#include <stm32lib/device/enc28j60/enc28j60.h>

PROCESS(dhcp_process, "DHCP");

uint8_t _network_request_dhcp = 0;
ENC28J60 enc28j60;

void network_setup() {
  printf("?BEGIN network_setup\n");

  uip_ipaddr_t ipaddr;
  uip_ipaddr_t gatewayAddr;
  uip_ipaddr_t netmaskAddr;

  uip_lladdr.addr[0] = MAC_ADDRESS[0];
  uip_lladdr.addr[1] = MAC_ADDRESS[1];
  uip_lladdr.addr[2] = MAC_ADDRESS[2];
  uip_lladdr.addr[3] = MAC_ADDRESS[3];
  uip_lladdr.addr[4] = MAC_ADDRESS[4];
  uip_lladdr.addr[5] = MAC_ADDRESS[5];

  uip_ipaddr(&ipaddr, 0, 0, 0, 0);
  uip_sethostaddr(&ipaddr);

  uip_ipaddr(&gatewayAddr, 0, 0, 0, 0);
  uip_setdraddr(&gatewayAddr);

  uip_ipaddr(&netmaskAddr, 0, 0, 0, 0);
  uip_setnetmask(&netmaskAddr);

  memcpy(enc28j60.macAddress, uip_lladdr.addr, 6);
  enc28j60.spi = ENC28J60_SPI;
  enc28j60.csPort = ENC28J60_CS_PORT;
  enc28j60.csPin = ENC28J60_CS_PIN;
  enc28j60.resetPort = ENC28J60_RESET_PORT;
  enc28j60.resetPin = ENC28J60_RESET_PIN;
  enc28j60_setup(&enc28j60);

  uip_init();
  uip_arp_init();

  printf("?Start DHCP Process\n");
  _network_request_dhcp = 1;
  process_start(&dhcp_process, NULL);

  printf("?END network_setup\n");
}

void network_tick() {
  enc28j60_tick(&enc28j60);
}

uint8_t enc28j60_tcp_output() {
  uip_arp_out();
  enc28j60_send(&enc28j60);
  return 0;
}

PROCESS_THREAD(dhcp_process, ev, data) {
  PROCESS_BEGIN();
  dhcpc_init(uip_lladdr.addr, sizeof(uip_lladdr.addr));

  while (1) {
    PROCESS_WAIT_EVENT();
    if (_network_request_dhcp) {
      _network_request_dhcp = 0;
      dhcpc_request();
    } else if (ev == PROCESS_EVENT_EXIT) {
      process_exit(&dhcp_process);
      LOADER_UNLOAD();
    } else {
      dhcpc_appcall(ev, data);
    }
  }

  PROCESS_END();
}

void dhcpc_configured(const struct dhcpc_state *s) {
  printf("?dhcpc_configured\n");
  printf("?ipaddr: %d.%d.%d.%d\n", s->ipaddr.u8[0], s->ipaddr.u8[1], s->ipaddr.u8[2], s->ipaddr.u8[3]);
  printf("?default_router: %d.%d.%d.%d\n", s->default_router.u8[0], s->default_router.u8[1], s->default_router.u8[2], s->default_router.u8[3]);
  printf("?netmask: %d.%d.%d.%d\n", s->netmask.u8[0], s->netmask.u8[1], s->netmask.u8[2], s->netmask.u8[3]);

  uip_sethostaddr(&s->ipaddr);
  uip_setdraddr(&s->default_router);
  uip_setnetmask(&s->netmask);

  process_start(&resolv_process, NULL);
}

void dhcpc_unconfigured(const struct dhcpc_state *s) {
  printf("?dhcpc_unconfigured\n");
}
