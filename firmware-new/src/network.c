
#include <stdio.h>
#include <contiki/core/sys/process.h>
#include <contiki/core/net/ip/uip.h>
#include <contiki/core/net/ipv4/uip_arp.h>
#include <contiki/dev/enc28j60/enc28j60.h>
#include <contiki/core/net/ip/uip.h>
#include <contiki/core/net/ip/uip-udp-packet.h>
#include <utils/time.h>
#include "platform_config.h"
#include "rs232.h"

PROCESS(network_process, "network");
PROCESS(rs232udp_process, "RS232UDP server");

#define UDP_HDR      ((struct uip_udpip_hdr *)&uip_buf[UIP_LLH_LEN])

static struct uip_udp_conn *_network_rs232udp_server_conn;
static uip_ipaddr_t _network_rs232udp_last_ip;
uint16_t _network_rs232udp_last_port;

uint8_t enc28j60_tcp_output();

void network_setup() {
  printf("network_setup\n");
  
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

  tcpip_set_outputfunc(enc28j60_tcp_output);
  enc28j60_init(uip_lladdr.addr);
  
  uip_init();

  uip_arp_init();
  
  printf("Start RS232UDP Process\n");
  process_start(&rs232udp_process, NULL);

  printf("Start network Process\n");
  process_start(&network_process, NULL);

  printf("END network_setup\n");
}

uint8_t enc28j60_tcp_output() {
  uip_arp_out();
  enc28j60_send(uip_buf, uip_len + sizeof(struct uip_eth_hdr));
  return 0;
}

void network_txLastAddr(const char* str) {
  uip_udp_packet_sendto(_network_rs232udp_server_conn, str, strlen(str), &_network_rs232udp_last_ip, UIP_HTONS(_network_rs232udp_last_port));
}

PROCESS_THREAD(network_process, ev, data) {
  PROCESS_BEGIN();
  
  while(1) {
    PROCESS_PAUSE();
    uip_len = enc28j60_read(uip_buf, UIP_BUFSIZE);
    if (uip_len > 0) {
      struct uip_eth_hdr *header = ((struct uip_eth_hdr *)&uip_buf[0]);
      uint16_t packetType = header->type;

      #ifdef ENC28J60_DEBUG
	printf("?receivePacket: len: %d, dest: %02x:%02x:%02x:%02x:%02x:%02x, src: %02x:%02x:%02x:%02x:%02x:%02x, type: %d\n",
	      uip_len,
	      header->dest.addr[0], header->dest.addr[1], header->dest.addr[2], header->dest.addr[3], header->dest.addr[4], header->dest.addr[5],
	      header->src.addr[0], header->src.addr[1], header->src.addr[2], header->src.addr[3], header->src.addr[4], header->src.addr[5],
	      packetType
	      );
	for (int i = 0; i < uip_len; i++) {
	  printf("%02x ", uip_buf[i]);
	}
	printf("\n");
      #endif

      if (packetType == UIP_HTONS(UIP_ETHTYPE_IP)) {
	#ifdef ENC28J60_DEBUG
	  printf("?readPacket type IP\n");
	#endif
	uip_arp_ipin();
	uip_input();
	if (uip_len > 0) {
	  uip_arp_out();
	  enc28j60_send(uip_buf, uip_len + sizeof(struct uip_eth_hdr));
	}
      } else if (packetType == UIP_HTONS(UIP_ETHTYPE_ARP)) {
	#ifdef ENC28J60_DEBUG
	  printf("?readPacket type ARP\n");
	#endif
	uip_arp_arpin();
	if (uip_len > 0) {
	  enc28j60_send(uip_buf, uip_len + sizeof(struct uip_eth_hdr));
	}
      }
    }
  }
  
  PROCESS_END();
}

PROCESS_THREAD(rs232udp_process, ev, data) {
  PROCESS_BEGIN();

  _network_rs232udp_server_conn = udp_new(NULL, UIP_HTONS(0), NULL);
  udp_bind(_network_rs232udp_server_conn, UIP_HTONS(UDP_PORT));
  
  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
    if(uip_newdata()) {
      uip_ipaddr_copy(&_network_rs232udp_last_ip, &UDP_HDR->srcipaddr);
      _network_rs232udp_last_port = UIP_HTONS(UDP_HDR->srcport);
      
      ((char *)uip_appdata)[uip_datalen()] = 0;
      printf("rs232udp %d.%d.%d.%d:%d rx: '%s'\n",
	     _network_rs232udp_last_ip.u8[0],
	     _network_rs232udp_last_ip.u8[1],
	     _network_rs232udp_last_ip.u8[2],
	     _network_rs232udp_last_ip.u8[3],
	     _network_rs232udp_last_port,
	     (char *)uip_appdata);
      rs232_tx((char *)uip_appdata);
    }
  }

  PROCESS_END();
}

void enc28j60_arch_spi_init(void) {
  enc28j60_arch_spi_deselect();
  HAL_GPIO_WritePin(PIN_ENC28J60_RESET_PORT, PIN_ENC28J60_RESET_PIN, GPIO_PIN_RESET);
  sleep_ms(50);
  HAL_GPIO_WritePin(PIN_ENC28J60_RESET_PORT, PIN_ENC28J60_RESET_PIN, GPIO_PIN_SET);
  sleep_ms(1000);
}

uint8_t enc28j60_arch_spi_write(uint8_t data) {
  uint8_t tx = data;
  uint8_t rx;
  HAL_StatusTypeDef status;
  if((status = HAL_SPI_TransmitReceive(&ENC28J60_SPI, &tx, &rx, 1, MAX_TIMEOUT)) != HAL_OK) {
    printf("spi write error %d\n", status);
  }
  return rx;
}

uint8_t enc28j60_arch_spi_read(void) {
  return enc28j60_arch_spi_write(0);
}

void enc28j60_arch_spi_select(void) {
  HAL_GPIO_WritePin(PIN_ENC28J60_CS_PORT, PIN_ENC28J60_CS_PIN, GPIO_PIN_RESET);
}

void enc28j60_arch_spi_deselect(void) {
  HAL_GPIO_WritePin(PIN_ENC28J60_CS_PORT, PIN_ENC28J60_CS_PIN, GPIO_PIN_SET);
}
