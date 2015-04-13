
#include "platform_config.h"
#include <stm32lib/debug.h>
#include "rs232.h"

void SysTick_Handler() {
  time_SysTick_Handler();
}

void USART1_IRQHandler() {
  debug_usartIrq();
}

void USART2_IRQHandler() {
  rs232_usartIrq();
}
