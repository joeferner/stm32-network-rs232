#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include "network.h"
#include "uip/timer.h"
#include "uip/uip_arp.h"
#include "platform_config.h"
#include "debug.h"

uint8_t _network_send();

struct timer _network_periodicTimer;
uint8_t _network_uip_headerLength = 0;
uint8_t _network_packetState = 0;

void network_setup() {
  debug_write_line("?BEGIN network_setup");

  uip_ipaddr_t ipaddr;
  uip_ipaddr_t gatewayAddr;
  uip_ipaddr_t netmaskAddr;

  timer_set(&_network_periodicTimer, CLOCK_SECOND / 4);

  uip_ethaddr.addr[0] = MAC_ADDRESS[0];
  uip_ethaddr.addr[1] = MAC_ADDRESS[1];
  uip_ethaddr.addr[2] = MAC_ADDRESS[2];
  uip_ethaddr.addr[3] = MAC_ADDRESS[3];
  uip_ethaddr.addr[4] = MAC_ADDRESS[4];
  uip_ethaddr.addr[5] = MAC_ADDRESS[5];

  uip_ipaddr(&ipaddr, IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
  uip_sethostaddr(ipaddr);

  uip_ipaddr(&gatewayAddr, GATEWAY_ADDRESS[0], GATEWAY_ADDRESS[1], GATEWAY_ADDRESS[2], GATEWAY_ADDRESS[3]);
  uip_setdraddr(gatewayAddr);

  uip_ipaddr(&netmaskAddr, NETMASK_ADDRESS[0], NETMASK_ADDRESS[1], NETMASK_ADDRESS[2], NETMASK_ADDRESS[3]);
  uip_setnetmask(netmaskAddr);

  enc28j60_setup(MAC_ADDRESS);

  uip_init();
  uip_arp_init();

  debug_write_line("?END network_setup");
}

void network_tick() {
  if (in_packet == NOBLOCK) {
    in_packet = enc28j60_receivePacket();
    if (in_packet != NOBLOCK) {
      debug_write("?receivePacket: ");
      debug_write_u8(in_packet, 10);
      debug_write_line("");
    }
  }
  if (in_packet != NOBLOCK) {
    _network_packetState = UIPETHERNET_FREEPACKET;
    uip_len = enc28j60_blockSize(in_packet);
    if (uip_len > 0) {
      enc28j60_readPacket(in_packet, 0, (uint8_t*)uip_buf, UIP_BUFSIZE);
      if (ETH_HDR->type == HTONS(UIP_ETHTYPE_IP)) {
        uip_packet = in_packet;
        debug_write("?readPacket type IP, uip_len: ");
        debug_write_u16(uip_len, 10);
        debug_write_line("");
        uip_arp_ipin();
        uip_input();
        if (uip_len > 0) {
          uip_arp_out();
          _network_send();
        }
      } else if (ETH_HDR ->type == HTONS(UIP_ETHTYPE_ARP)) {
        debug_write("?readPacket type ARP, uip_len: ");
        debug_write_u16(uip_len, 10);
        debug_write_line("");
        uip_arp_arpin();
        if (uip_len > 0) {
          _network_send();
        }
      }
    }
    if (in_packet != NOBLOCK && (_network_packetState & UIPETHERNET_FREEPACKET)) {
      debug_write_line("?freeing packet");
      enc28j60_freePacket();
      in_packet = NOBLOCK;
    }
  }

  if (timer_expired(&_network_periodicTimer)) {
    timer_restart(&_network_periodicTimer);
    for (int i = 0; i < UIP_CONNS; i++) {
      uip_periodic(i);
      // If the above function invocation resulted in data that
      // should be sent out on the Enc28J60Network, the global variable
      // uip_len is set to a value > 0.
      if (uip_len > 0) {
        uip_arp_out();
        _network_send();
      }
    }

#if UIP_UDP
    for (int i = 0; i < UIP_UDP_CONNS; i++) {
      uip_udp_periodic(i);
      // If the above function invocation resulted in data that
      // should be sent out on the Enc28J60Network, the global variable
      // uip_len is set to a value > 0. */
      if (uip_len > 0) {
        UIPUDP::_send((uip_udp_userdata_t *)(uip_udp_conns[i].appstate));
      }
    }
#endif /* UIP_UDP */
  }
}

void enc28j60_spi_assert() {
  GPIO_ResetBits(ENC28J60_CS_PORT, ENC28J60_CS_PIN);
}

void enc28j60_spi_deassert() {
  GPIO_SetBits(ENC28J60_CS_PORT, ENC28J60_CS_PIN);
}

uint8_t enc28j60_spi_transfer(uint8_t d) {
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  SPI_I2S_SendData(SPI1, d);
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  return SPI_I2S_ReceiveData(SPI1);
}

uint8_t _network_send() {
  if (_network_packetState & UIPETHERNET_SENDPACKET) {
    debug_write("?Enc28J60Network_send uip_packet: ");
    debug_write(uip_packet);
    debug_write(", hdrlen: ");
    debug_write(_network_uip_headerLength);
    debug_write_line("");
    enc28j60_writePacket(uip_packet, 0, uip_buf, _network_uip_headerLength);
    goto sendandfree;
  }
  uip_packet = enc28j60_allocBlock(uip_len);
  if (uip_packet != NOBLOCK) {
#ifdef UIPETHERNET_DEBUG
    Serial.print(F("Enc28J60Network_send uip_buf (uip_len): "));
    Serial.print(uip_len);
    Serial.print(F(", packet: "));
    Serial.println(uip_packet);
#endif
    enc28j60_writePacket(uip_packet, 0, uip_buf, uip_len);
    goto sendandfree;
  }
  return 0;
sendandfree:
  enc28j60_sendPacket(uip_packet);
  enc28j60_freeBlock(uip_packet);
  uip_packet = NOBLOCK;
  return 1;
}
