
#include "rs232.h"
#include <utils/utils.h>
#include <stdio.h>
#include <string.h>
#include "platform_config.h"
#include <utils/ringbufferdma.h>

#ifdef RS232_DEBUG
#define RS232_DEBUG_OUT(format, ...) printf("%s:%d: rs232: " format, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define RS232_DEBUG_OUT(format, ...)
#endif

#define RS232_RX_BUFFER_SIZE 32
RingBufferDmaU8 rs232RxRing;
uint8_t rs232RxBuffer[RS232_RX_BUFFER_SIZE];

void rs232_setup() {
  RS232_DEBUG_OUT("rs232_setup\n");
  RingBufferDmaU8_initUSARTRx(&rs232RxRing, &RS232_UART, rs232RxBuffer, RS232_RX_BUFFER_SIZE);
}

void rs232_tx(const char* str) {
  RS232_DEBUG_OUT("rs232tx: \"%s\"\n", str);
  HAL_StatusTypeDef r = HAL_UART_Transmit(&RS232_UART, (uint8_t*)str, strlen(str), MAX_TIMEOUT);
  if (r != HAL_OK) {
    RS232_DEBUG_OUT("rs232tx: fail 0x%02x\n", r);
  }
}

void rs232_tick() {
  char line[30];
  if (RingBufferDmaU8_readLine(&rs232RxRing, line, sizeof(line)) > 0) {
    strTrimRight(line);
    if (strlen(line) > 0) {
      rs232_processLine(line);
    }
  }
}
