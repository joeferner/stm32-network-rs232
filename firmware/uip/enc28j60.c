#include "enc28j60.h"
#include "enc28j60_constants.h"
#include "delay.h"

void _enc28j60_writeOp(uint8_t op, uint8_t address, uint8_t data);
void _enc28j60_writeRegPair(uint8_t address, uint16_t data);
void _enc28j60_writeReg(uint8_t address, uint8_t data);
void _enc28j60_phyWrite(uint8_t address, uint16_t data);
void _enc28j60_setBank(uint8_t address);
uint8_t _enc28j60_readReg(uint8_t address);
uint8_t _enc28j60_readOp(uint8_t op, uint8_t address);

uint16_t _enc28j60_nextPacketPtr;
uint8_t _enc28j60_bank = 0xff;

void enc28j60_setup(uint8_t* macAddress) {
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

