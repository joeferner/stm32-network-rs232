
#include "platform_config.h"
#include "rs232.h"
#include <stm32lib/ringbuffer.h>
#include <stm32lib/usart.h>
#include <string.h>

#define RS232_WRITE_BUFFER_SIZE 100
#define RS232_READ_BUFFER_SIZE 100

uint8_t _rs232_readBuffer[RS232_READ_BUFFER_SIZE];
RingBufferU8 _rs232_readRingBuffer;

void rs232_setup() {
  USART_InitParams usart;

  printf("?BEGIN rs232_setup\n");

  RingBufferU8_init(&_rs232_readRingBuffer, _rs232_readBuffer, RS232_READ_BUFFER_SIZE);

  USART_initParamsInit(&usart);
  usart.txPort = RS232USART_TX_PORT;
  usart.txPin = RS232USART_TX_PIN;
  usart.rxPort = RS232USART_RX_PORT;
  usart.rxPin = RS232USART_RX_PIN;
  usart.halUsartInitParams.instance = RS232USART_USART;
  usart.halUsartInitParams.baudRate = 9600;
  usart.halUsartInitParams.wordLength = USART_WordLength_8b;
  usart.halUsartInitParams.parity = USART_Parity_no;
  usart.halUsartInitParams.stopBits = USART_StopBits_1;
  usart.halUsartInitParams.hardwareFlowControl = USART_HardwareFlowControl_none;
  usart.halUsartInitParams.mode = USART_Mode_rx | USART_Mode_tx;
  USART_init(&usart);

  USART_interruptReceive(RS232USART_USART, ENABLE);
  USART_interruptsEnable(RS232USART_USART);

  USART_enable(RS232USART_USART);

  printf("?END rs232_setup\n");
  IWDG_RESET;
}

void rs232_writeString(const char *str) {
  USART_txString(RS232USART_USART, str);
}

void rs232_usartIrq() {
  uint8_t b;
  
  while (USART_getFlagStatus(RS232USART_USART, USART_Flag_RXNE)) {
    USART_clearFlag(RS232USART_USART, USART_Flag_RXNE);
    b = USART_rx(RS232USART_USART);
    RingBufferU8_writeByte(&_rs232_readRingBuffer, b);
  }
}

uint16_t rs232_readLine(char *buffer, uint16_t size) {
  return RingBufferU8_readLine(&_rs232_readRingBuffer, buffer, size);
}

void rs232_clearBuffer() {
  RingBufferU8_clear(&_rs232_readRingBuffer);
}

