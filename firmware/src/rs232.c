
#include "rs232.h"
#include <utils/utils.h>
#include <stdio.h>
#include <string.h>
#include "platform_config.h"
#include <utils/ringbufferdma.h>
#include <contiki/core/sys/process.h>

PROCESS(rs232_process, "RS232");

#define RS232_RX_BUFFER_SIZE 32
RingBufferDmaU8 rs232RxRing;
uint8_t rs232RxBuffer[RS232_RX_BUFFER_SIZE];

void rs232_setup() {
  printf("rs232_setup\n");
  RingBufferDmaU8_initUSARTRx(&rs232RxRing, &RS232_UART, rs232RxBuffer, RS232_RX_BUFFER_SIZE);
  process_start(&rs232_process, NULL);
}

void rs232_tx(const char* str) {
  printf("rs232tx: \"%s\"\n", str);
  HAL_UART_Transmit(&RS232_UART, (uint8_t *)str, strlen(str), MAX_TIMEOUT);
}

PROCESS_THREAD(rs232_process, ev, data) {
  char line[30];

  PROCESS_BEGIN();
  while(1) {
    PROCESS_PAUSE();
    if(RingBufferDmaU8_readLine(&rs232RxRing, line, sizeof(line)) > 0) {
      strTrimRight(line);
      if(strlen(line) > 0) {
	rs232_processLine(line);
      }
    }
  }
  PROCESS_END();
}
