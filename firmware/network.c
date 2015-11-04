
#include "network.h"
#include "rs232.h"
#include <sys/etimer.h>
#include <net/ip/uip.h>
#include <net/ipv4/uip_arp.h>
#include <net/ip/resolv.h>
#include <net/ip/uip-udp-packet.h>
#include <stm32lib/device/enc28j60/enc28j60.h>

#define UDP_HDR      ((struct uip_udpip_hdr *)&uip_buf[UIP_LLH_LEN])

PROCESS(rs232udp_process, "RS232UDP server process");

static struct uip_udp_conn *_network_rs232udp_server_conn;
static uip_ipaddr_t _network_rs232udp_last_ip;
uint16_t _network_rs232udp_last_port;
ENC28J60 enc28j60;

void rs232udp_handler();

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

  uip_ipaddr(&ipaddr, 192, 168, 0, 101);
  uip_sethostaddr(&ipaddr);

  uip_ipaddr(&gatewayAddr, 192, 168, 0, 1);
  uip_setdraddr(&gatewayAddr);

  uip_ipaddr(&netmaskAddr, 255, 255, 255, 0);
  uip_setnetmask(&netmaskAddr);

  memcpy(enc28j60.macAddress, uip_lladdr.addr, 6);
  enc28j60.spi = ENC28J60_SPI;
  enc28j60.csPort = ENC28J60_CS_PORT;
  enc28j60.csPin = ENC28J60_CS_PIN;
  enc28j60.resetPort = ENC28J60_RESET_PORT;
  enc28j60.resetPin = ENC28J60_RESET_PIN;
  enc28j60_setup(&enc28j60);

  uip_init();
  IWDG_RESET;

  uip_arp_init();
  IWDG_RESET;

#ifdef DEBUG_NETWORK_ENABLE
  debug_networkSetup();
#endif

  printf("?Start RESOLV Process\n");
  process_start(&resolv_process, NULL);
  
  printf("?Start RS232UDP Process\n");
  process_start(&rs232udp_process, NULL);
  IWDG_RESET;

  printf("?END network_setup\n");
  IWDG_RESET;
}

void network_tick() {
  char buffer[100];
  uint16_t result;
  
  enc28j60_tick(&enc28j60);
  
  if((result = rs232_readLine(buffer, sizeof(buffer))) > 0) {
    uip_udp_packet_sendto(_network_rs232udp_server_conn, buffer, result, &_network_rs232udp_last_ip, UIP_HTONS(_network_rs232udp_last_port));
  }
  
  IWDG_RESET;
}

uint8_t enc28j60_tcp_output() {
  uip_arp_out();
  enc28j60_send(&enc28j60);
  return 0;
}


PROCESS_THREAD(rs232udp_process, ev, data) {
  PROCESS_BEGIN();

  _network_rs232udp_server_conn = udp_new(NULL, UIP_HTONS(0), NULL);
  udp_bind(_network_rs232udp_server_conn, UIP_HTONS(UDP_PORT));
  
  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
    rs232udp_handler();
  }

  PROCESS_END();
}

void rs232udp_handler() {
  if(uip_newdata()) {
    uip_ipaddr_copy(&_network_rs232udp_last_ip, &UDP_HDR->srcipaddr);
    _network_rs232udp_last_port = UIP_HTONS(UDP_HDR->srcport);
    
    ((char *)uip_appdata)[uip_datalen()] = 0;
    printf("rs232udp rx: '%s'\n", (char *)uip_appdata);
    rs232_clearBuffer();
    rs232_writeString(urlDecode((char *)uip_appdata));
  }
}
