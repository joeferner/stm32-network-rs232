
#ifndef _PLATFORM_CONFIG_H_
#define _PLATFORM_CONFIG_H_

#include <pinout.h>

extern uint8_t MAC_ADDRESS[6];
#define UDP_PORT            9000

#define DEBUG_UART   huart1
#define RS232_UART   huart2
#define ENC28J60_SPI hspi1

#define MAX_TIMEOUT 0xffff

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern SPI_HandleTypeDef hspi1;
extern IWDG_HandleTypeDef hiwdg;

#endif
