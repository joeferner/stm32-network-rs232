
#include "network.h"
#include "rs232.h"
#include <sys/etimer.h>
#include <net/ip/uip.h>
#include <net/ipv4/uip_arp.h>
#include <net/ipv4/uip-fw.h>
#include <net/ip/resolv.h>
#include <net/ip/uip-udp-packet.h>
#include <enc28j60/enc28j60.h>

#define UDP_HDR      ((struct uip_udpip_hdr *)&uip_buf[UIP_LLH_LEN])

PROCESS(rs232udp_process, "RS232UDP server process");

static struct uip_udp_conn *_network_rs232udp_server_conn;
static uip_ipaddr_t _network_rs232udp_last_ip;
uint16_t _network_rs232udp_last_port;

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

  enc28j60_init(uip_lladdr.addr);
  
  uip_init();
  uip_fw_init();
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
  
  if((result = rs232_readLine(buffer, sizeof(buffer))) > 0) {
    uip_udp_packet_sendto(_network_rs232udp_server_conn, buffer, result, &_network_rs232udp_last_ip, UIP_HTONS(_network_rs232udp_last_port));
  }
  
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
  
  IWDG_RESET;
}

uint8_t enc28j60_tcp_output() {
  uip_arp_out();
  enc28j60_send(uip_buf, uip_len + sizeof(struct uip_eth_hdr));
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

void enc28j60_arch_spi_init(void) {
  GPIO_InitParams gpio;

  RCC_peripheralClockEnableForPort(ENC28J60_CS_PORT);
  RCC_peripheralClockEnableForPort(ENC28J60_RESET_PORT);

  GPIO_initParamsInit(&gpio);
  gpio.mode = GPIO_Mode_output;
  gpio.outputType = GPIO_OutputType_pushPull;
  gpio.speed = GPIO_Speed_high;

  gpio.port = ENC28J60_CS_PORT;
  gpio.pin = ENC28J60_CS_PIN;
  GPIO_init(&gpio);
  enc28j60_arch_spi_deselect();

  gpio.port = ENC28J60_RESET_PORT;
  gpio.pin = ENC28J60_RESET_PIN;
  GPIO_init(&gpio);
  GPIO_setBits(ENC28J60_RESET_PORT, ENC28J60_RESET_PIN);
}

uint8_t enc28j60_arch_spi_write(uint8_t data) {
  return SPI_transfer(ENC28J60_SPI, data);
}

uint8_t enc28j60_arch_spi_read(void) {
  return enc28j60_arch_spi_write(0);
}

void enc28j60_arch_spi_select(void) {
  GPIO_resetBits(ENC28J60_CS_PORT, ENC28J60_CS_PIN);
}

void enc28j60_arch_spi_deselect(void) {
  GPIO_setBits(ENC28J60_CS_PORT, ENC28J60_CS_PIN);
}

