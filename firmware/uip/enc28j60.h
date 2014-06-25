
#ifndef _ENC28J60_H_
#define _ENC28J60_H_

#include "uip.h"

extern void enc28j60_spi_assert();
extern void enc28j60_spi_deassert();
extern uint8_t enc28j60_spi_transfer(uint8_t d);

void enc28j60_setup(uint8_t* macAddress);

#endif // _ENC28J60_H_
