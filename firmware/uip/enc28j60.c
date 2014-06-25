#include "enc28j60.h"
#include "enc28j60_constants.h"
#include "delay.h"
#include "debug.h"
#include "uip_arp.h"
#include "timer.h"

uint8_t _enc28j60_send();
uint16_t _enc28j60_receivePacket(uint8_t* buffer, uint16_t bufferLength);

void _enc28j60_setERXRDPT();

void _enc28j60_writeOp(uint8_t op, uint8_t address, uint8_t data);
void _enc28j60_writeRegPair(uint8_t address, uint16_t data);
void _enc28j60_writeReg(uint8_t address, uint8_t data);
void _enc28j60_phyWrite(uint8_t address, uint16_t data);
void _enc28j60_setBank(uint8_t address);
uint8_t _enc28j60_readReg(uint8_t address);
uint8_t _enc28j60_readOp(uint8_t op, uint8_t address);

struct timer _enc28j60_periodicTimer;
uint16_t _enc28j60_nextPacketPtr;
uint8_t _enc28j60_bank = 0xff;

void enc28j60_setup(uint8_t* macAddress) {
  timer_set(&_enc28j60_periodicTimer, CLOCK_SECOND / 4);

  // perform system reset
  _enc28j60_writeOp(ENC28J60_SOFT_RESET, 0, ENC28J60_SOFT_RESET);
  delay_ms(50);

  // check CLKRDY bit to see if reset is complete
  // The CLKRDY does not work. See Rev. B4 Silicon Errata point. Just wait.
  //while(!(readReg(ESTAT) & ESTAT_CLKRDY));

  // do bank 0 stuff
  // initialize receive buffer
  // 16-bit transfers, must write low byte first
  // set receive buffer start address
  _enc28j60_nextPacketPtr = RXSTART_INIT;

  // Rx start
  _enc28j60_writeRegPair(ERXSTL, RXSTART_INIT);

  // set receive pointer address
  _enc28j60_writeRegPair(ERXRDPTL, RXSTART_INIT);

  // RX end
  _enc28j60_writeRegPair(ERXNDL, RXSTOP_INIT);

  // TX start
  //_enc28j60_writeRegPair(ETXSTL, TXSTART_INIT);

  // TX end
  //_enc28j60_writeRegPair(ETXNDL, TXSTOP_INIT);

  // do bank 1 stuff, packet filter:
  // For broadcast packets we allow only ARP packtets
  // All other packets should be unicast only for our mac (MAADR)
  //
  // The pattern to match on is therefore
  // Type     ETH.DST
  // ARP      BROADCAST
  // 06 08 -- ff ff ff ff ff ff -> ip checksum for theses bytes=f7f9
  // in binary these poitions are:11 0000 0011 1111
  // This is hex 303F->EPMM0=0x3f,EPMM1=0x30
  //TODO define specific pattern to receive dhcp-broadcast packages instead of setting ERFCON_BCEN!
  _enc28j60_writeReg(ERXFCON, ERXFCON_UCEN|ERXFCON_CRCEN|ERXFCON_PMEN|ERXFCON_BCEN);
  _enc28j60_writeRegPair(EPMM0, 0x303f);
  _enc28j60_writeRegPair(EPMCSL, 0xf7f9);

  // do bank 2 stuff
  // enable MAC receive
  // and bring MAC out of reset (writes 0x00 to MACON2)
  _enc28j60_writeRegPair(MACON1, MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS);

  // enable automatic padding to 60bytes and CRC operations
  _enc28j60_writeOp(ENC28J60_BIT_FIELD_SET, MACON3, MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN);

  // set inter-frame gap (non-back-to-back)
  _enc28j60_writeRegPair(MAIPGL, 0x0C12);

  // set inter-frame gap (back-to-back)
  _enc28j60_writeReg(MABBIPG, 0x12);

  // Set the maximum packet size which the controller will accept
  // Do not send packets longer than MAX_FRAMELEN:
  _enc28j60_writeRegPair(MAMXFLL, MAX_FRAMELEN);

  // do bank 3 stuff
  // write MAC address
  // NOTE: MAC address in ENC28J60 is byte-backward
  _enc28j60_writeReg(MAADR5, macAddress[0]);
  _enc28j60_writeReg(MAADR4, macAddress[1]);
  _enc28j60_writeReg(MAADR3, macAddress[2]);
  _enc28j60_writeReg(MAADR2, macAddress[3]);
  _enc28j60_writeReg(MAADR1, macAddress[4]);
  _enc28j60_writeReg(MAADR0, macAddress[5]);

  // no loopback of transmitted frames
  _enc28j60_phyWrite(PHCON2, PHCON2_HDLDIS);

  // switch to bank 0
  _enc28j60_setBank(ECON1);

  // enable interrutps
  _enc28j60_writeOp(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE|EIE_PKTIE);

  // enable packet reception
  _enc28j60_writeOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);

  //Configure leds
  _enc28j60_phyWrite(PHLCON,0x476);
}

void enc28j60_tick() {
  uip_len = _enc28j60_receivePacket(((uint8_t*)uip_buf), UIP_BUFSIZE);
  if(uip_len > 0) {
    debug_write("?receivePacket: ");
    debug_write_u16(uip_len, 10);
    debug_write_line("");

    uint16_t packetType = ((struct uip_eth_hdr *)&uip_buf[0])->type;
    if (packetType == HTONS(UIP_ETHTYPE_IP)) {
      debug_write_line("?readPacket type IP");
      uip_arp_ipin();
      uip_input();
      if (uip_len > 0) {
        uip_arp_out();
        _enc28j60_send();
      }
    } else if (packetType == HTONS(UIP_ETHTYPE_ARP)) {
      debug_write_line("?readPacket type ARP");
      uip_arp_arpin();
      if (uip_len > 0) {
        _enc28j60_send();
      }
    }
  }

  if (timer_expired(&_enc28j60_periodicTimer)) {
    timer_restart(&_enc28j60_periodicTimer);
    for (int i = 0; i < UIP_CONNS; i++) {
      uip_periodic(i);
      // If the above function invocation resulted in data that
      // should be sent out on the Enc28J60Network, the global variable
      // uip_len is set to a value > 0.
      if (uip_len > 0) {
        uip_arp_out();
        _enc28j60_send();
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

uint16_t _enc28j60_receivePacket(uint8_t* buffer, uint16_t bufferLength) {
  uint16_t rxstat;
  uint16_t len;

  // check if a packet has been received and buffered
  //if( !(_enc28j60_readReg(EIR) & EIR_PKTIF) ){
  // The above does not work. See Rev. B4 Silicon Errata point 6.
  if (_enc28j60_readReg(EPKTCNT) == 0) {
    return;
  }

  uint16_t readPtr = _enc28j60_nextPacketPtr + 6;

  // Set the read pointer to the start of the received packet
  _enc28j60_writeRegPair(ERDPTL, _enc28j60_nextPacketPtr);

  // read the next packet pointer
  _enc28j60_nextPacketPtr = _enc28j60_readOp(ENC28J60_READ_BUF_MEM, 0);
  _enc28j60_nextPacketPtr |= _enc28j60_readOp(ENC28J60_READ_BUF_MEM, 0) << 8;

  // read the packet length (see datasheet page 43)
  len = _enc28j60_readOp(ENC28J60_READ_BUF_MEM, 0);
  len |= _enc28j60_readOp(ENC28J60_READ_BUF_MEM, 0) << 8;
  len -= 4; //remove the CRC count

  // read the receive status (see datasheet page 43)
  rxstat = _enc28j60_readOp(ENC28J60_READ_BUF_MEM, 0);
  rxstat |= _enc28j60_readOp(ENC28J60_READ_BUF_MEM, 0) << 8;

  readBuffer;

  // Move the RX read pointer to the start of the next received packet
  // This frees the memory we just read out
  _enc28j60_setERXRDPT();

  // decrement the packet counter indicate we are done with this packet
  _enc28j60_writeOp(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);

  return len;
}

uint8_t _enc28j60_send() {
  if (_network_packetState & UIPETHERNET_SENDPACKET) {
    debug_write("?Enc28J60Network_send uip_packet: ");
    debug_write(uip_packet);
    debug_write(", hdrlen: ");
    debug_write(_network_uip_headerLength);
    debug_write_line("");
    _enc28j60_writePacket(uip_packet, 0, uip_buf, _network_uip_headerLength);
    goto sendandfree;
  }
  uip_packet = _enc28j60_allocBlock(uip_len);
  if (uip_packet != NOBLOCK) {
#ifdef UIPETHERNET_DEBUG
    Serial.print(F("Enc28J60Network_send uip_buf (uip_len): "));
    Serial.print(uip_len);
    Serial.print(F(", packet: "));
    Serial.println(uip_packet);
#endif
    _enc28j60_writePacket(uip_packet, 0, uip_buf, uip_len);
    goto sendandfree;
  }
  return 0;
sendandfree:
  _enc28j60_sendPacket(uip_packet);
  _enc28j60_freeBlock(uip_packet);
  uip_packet = NOBLOCK;
  return 1;
}

void _enc28j60_setERXRDPT() {
  _enc28j60_writeRegPair(ERXRDPTL, nextPacketPtr == RXSTART_INIT ? RXSTOP_INIT : nextPacketPtr - 1);
}

void _enc28j60_writeReg(uint8_t address, uint8_t data) {
  // set the bank
  _enc28j60_setBank(address);

  // do the write
  _enc28j60_writeOp(ENC28J60_WRITE_CTRL_REG, address, data);
}

void _enc28j60_writeRegPair(uint8_t address, uint16_t data) {
  // set the bank
  _enc28j60_setBank(address);

  // do the write
  _enc28j60_writeOp(ENC28J60_WRITE_CTRL_REG, address, (data & 0xFF));
  _enc28j60_writeOp(ENC28J60_WRITE_CTRL_REG, address + 1, (data) >> 8);
}

void _enc28j60_phyWrite(uint8_t address, uint16_t data) {
  // set the PHY register address
  _enc28j60_writeReg(MIREGADR, address);

  // write the PHY data
  _enc28j60_writeRegPair(MIWRL, data);

  // wait until the PHY write completes
  while(_enc28j60_readReg(MISTAT) & MISTAT_BUSY) {
    delay_us(15);
  }
}

void _enc28j60_setBank(uint8_t address) {
  // set the bank (if needed)
  if((address & BANK_MASK) != _enc28j60_bank) {
    // set the bank
    _enc28j60_writeOp(ENC28J60_BIT_FIELD_CLR, ECON1, (ECON1_BSEL1|ECON1_BSEL0));
    _enc28j60_writeOp(ENC28J60_BIT_FIELD_SET, ECON1, (address & BANK_MASK)>>5);
    _enc28j60_bank = (address & BANK_MASK);
  }
}

uint8_t _enc28j60_readReg(uint8_t address) {
  // set the bank
  _enc28j60_setBank(address);

  // do the read
  return _enc28j60_readOp(ENC28J60_READ_CTRL_REG, address);
}

uint8_t _enc28j60_readOp(uint8_t op, uint8_t address) {
  uint8_t  result;

  enc28j60_spi_assert();
  enc28j60_spi_transfer(op | (address & ADDR_MASK)); // issue read command
  result = enc28j60_spi_transfer(0x00);

  // do dummy read if needed (for mac and mii, see datasheet page 29)
  if(address & 0x80) {
    result = enc28j60_spi_transfer(0x00);
  }

  enc28j60_spi_deassert();
  return result;
}

void _enc28j60_writeOp(uint8_t op, uint8_t address, uint8_t data) {
  enc28j60_spi_assert();
  enc28j60_spi_transfer(op | (address & ADDR_MASK));
  enc28j60_spi_transfer(data);
  enc28j60_spi_deassert();
}

