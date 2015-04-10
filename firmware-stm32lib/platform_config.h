
#ifndef PLATFORM_CONFIG_H_INCLUDED
#define PLATFORM_CONFIG_H_INCLUDED

#include <stdint.h>
#include "contiki-conf.h"

extern uint8_t MAC_ADDRESS[6];

#define ENC28J60_DEBUG

#define DEBUG_USART   USART1
#define DEBUG_BAUD    9600
#define DEBUG_TX_PORT GPIOA
#define DEBUG_TX_PIN  GPIO_Pin_9
#define DEBUG_RX_PORT GPIOA
#define DEBUG_RX_PIN  GPIO_Pin_10

#define ENC28J60_SPI        SPI1
#define ENC28J60_MOSI_PORT  GPIOA
#define ENC28J60_MOSI_PIN   GPIO_Pin_7
#define ENC28J60_MISO_PORT  GPIOA
#define ENC28J60_MISO_PIN   GPIO_Pin_6
#define ENC28J60_SCK_PORT   GPIOA
#define ENC28J60_SCK_PIN    GPIO_Pin_5
#define ENC28J60_CS_PORT    GPIOB
#define ENC28J60_CS_PIN     GPIO_Pin_0
#define ENC28J60_RESET_PORT GPIOB
#define ENC28J60_RESET_PIN  GPIO_Pin_1

#include <stm32lib/utils.h>
#include <stm32lib/hal/rcc.h>
#include <stm32lib/hal/gpio.h>
#include <stm32lib/debug.h>
#include <stm32lib/rcc.h>
#include <stm32lib/spi.h>
#include <stm32lib/time.h>

#endif /* PLATFORM_CONFIG_H_INCLUDED */
