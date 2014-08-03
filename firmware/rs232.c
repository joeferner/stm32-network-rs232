#include "rs232.h"
#include "platform_config.h"
#include "debug.h"
#include <stm32f10x_usart.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <misc.h>

void rs232_setup() {
  USART_InitTypeDef usartInitStructure;
  GPIO_InitTypeDef gpioInitStructure;
  NVIC_InitTypeDef nvicInitStructure;

  debug_write_line("?BEGIN rs232_setup");

  /* Enable clocks */
  RCC_APB1PeriphClockCmd(RS232_USART_RCC1, ENABLE);
  RCC_APB2PeriphClockCmd(RS232_USART_RCC2, ENABLE);

  /* Enable the USART2 Interrupt */
  nvicInitStructure.NVIC_IRQChannel = RS232_USART_IRQ;
  nvicInitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  nvicInitStructure.NVIC_IRQChannelSubPriority = 4;
  nvicInitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvicInitStructure);

  /* Configure USART Tx as alternate function push-pull */
  gpioInitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  gpioInitStructure.GPIO_Pin = RS232_USART_TX_PIN;
  gpioInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RS232_USART_TX, &gpioInitStructure);

  /* Configure USART Rx as input floating */
  gpioInitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  gpioInitStructure.GPIO_Pin = RS232_USART_RX_PIN;
  GPIO_Init(RS232_USART_RX, &gpioInitStructure);

  /* USART configuration */
  usartInitStructure.USART_BaudRate = RS232_USART_BAUD;
  usartInitStructure.USART_WordLength = USART_WordLength_8b;
  usartInitStructure.USART_Parity = USART_Parity_No;
  usartInitStructure.USART_StopBits = USART_StopBits_1;
  usartInitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  usartInitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(RS232_USART, &usartInitStructure);

  /* Enable USART */
  USART_Cmd(RS232_USART, ENABLE);

  /* Enable the USART interrupts */
  USART_ITConfig(RS232_USART, USART_IT_RXNE, ENABLE);

  debug_write_line("?END rs232_setup");
}

void rs232_write(uint8_t b) {
  while (USART_GetFlagStatus(RS232_USART, USART_FLAG_TXE) == RESET);
  USART_SendData(RS232_USART, b);
}

