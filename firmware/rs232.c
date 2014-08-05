#include "rs232.h"
#include "platform_config.h"
#include "debug.h"
#include <stm32f10x_usart.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <misc.h>

uint8_t g_rs232UsartInputBuffer[RS232_INPUT_BUFFER_SIZE];
dma_ring_buffer g_rs232UsartDmaInputRingBuffer;

uint8_t _rs232UsartOutputBuffer[RS232_OUTPUT_BUFFER_SIZE];
ring_buffer_u8 _rs232UsartOutputRingBuffer;

void rs232_setup() {
  USART_InitTypeDef usartInitStructure;
  GPIO_InitTypeDef gpioInitStructure;
  DMA_InitTypeDef dmaInitStructure;

  debug_write_line("?BEGIN rs232_setup");

  dma_ring_buffer_init(&g_rs232UsartDmaInputRingBuffer, RS232_USART_RX_DMA_CH, g_rs232UsartInputBuffer, RS232_INPUT_BUFFER_SIZE);
  ring_buffer_u8_init(&_rs232UsartOutputRingBuffer, _rs232UsartOutputBuffer, RS232_OUTPUT_BUFFER_SIZE);

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  // Enable clocks
  RCC_APB1PeriphClockCmd(RS232_USART_RCC1, ENABLE);
  RCC_APB2PeriphClockCmd(RS232_USART_RCC2, ENABLE);

  // Configure USART Tx as alternate function push-pull
  gpioInitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  gpioInitStructure.GPIO_Pin = RS232_USART_TX_PIN;
  gpioInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RS232_USART_TX, &gpioInitStructure);

  // Configure USART Rx as input floating
  gpioInitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  gpioInitStructure.GPIO_Pin = RS232_USART_RX_PIN;
  GPIO_Init(RS232_USART_RX, &gpioInitStructure);

  // Configure DMA - RX
  DMA_StructInit(&dmaInitStructure);
  DMA_DeInit(RS232_USART_RX_DMA_CH);
  dmaInitStructure.DMA_PeripheralBaseAddr = (uint32_t)RS232_USART_DR_BASE;
  dmaInitStructure.DMA_MemoryBaseAddr = (uint32_t)g_rs232UsartInputBuffer;
  dmaInitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  dmaInitStructure.DMA_BufferSize = RS232_INPUT_BUFFER_SIZE;
  dmaInitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  dmaInitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  dmaInitStructure.DMA_Mode = DMA_Mode_Circular;
  dmaInitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  dmaInitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  dmaInitStructure.DMA_Priority = DMA_Priority_Medium;
  dmaInitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(RS232_USART_RX_DMA_CH, &dmaInitStructure);

  // USART configuration
  usartInitStructure.USART_BaudRate = RS232_USART_BAUD;
  usartInitStructure.USART_WordLength = USART_WordLength_8b;
  usartInitStructure.USART_Parity = USART_Parity_No;
  usartInitStructure.USART_StopBits = USART_StopBits_1;
  usartInitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  usartInitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(RS232_USART, &usartInitStructure);

  // Enable DMA
  USART_DMACmd(RS232_USART, USART_DMAReq_Rx, ENABLE);
  DMA_Cmd(RS232_USART_RX_DMA_CH, ENABLE);

  // Enable USART
  USART_Cmd(RS232_USART, ENABLE);

  debug_write_line("?END rs232_setup");
}

void rs232_write(const uint8_t* buffer, uint16_t len) {
  ring_buffer_u8_write(&_rs232UsartOutputRingBuffer, buffer, len);
}

void rs232_tick() {
  char line[MAX_LINE_LENGTH];
  while(dma_ring_buffer_readline(&g_rs232UsartDmaInputRingBuffer, line, MAX_LINE_LENGTH)) {
    debug_write("+RECV:");
    debug_write_line(line);
  }

  if(ring_buffer_u8_available(&_rs232UsartOutputRingBuffer)) {
    uint8_t b = ring_buffer_u8_read_byte(&_rs232UsartOutputRingBuffer);

    while (USART_GetFlagStatus(RS232_USART, USART_FLAG_TXE) == RESET);
    USART_SendData(RS232_USART, b);
  }
}
