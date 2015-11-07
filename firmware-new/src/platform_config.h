
#ifndef _PLATFORM_CONFIG_H_
#define _PLATFORM_CONFIG_H_

#include <pinout.h>

#define DEBUG_UART huart1
#define RS232_UART huart2

#define MAX_TIMEOUT 0xffff

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

#endif
