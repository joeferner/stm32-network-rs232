
#include "debug.h"
#include <utils/ringbuffer.h>
#include <utils/utils.h>
#include <stdio.h>
#include <string.h>
#include "platform_config.h"

RingBufferU8 debugRing;
uint8_t debugRingBuffer[30];

void _debug_processLine(const char* line);

void debug_setup() {
  printf("debug_setup\n");
  RingBufferU8_init(&debugRing, debugRingBuffer, sizeof(debugRingBuffer));
}

void debug_tick() {
  uint8_t b;
  char line[30];
  while(HAL_UART_Receive(&DEBUG_UART, &b, 1, 0) == HAL_OK) {
    RingBufferU8_writeByte(&debugRing, b);
    if(RingBufferU8_readLine(&debugRing, line, sizeof(line)) > 0) {
      strTrimRight(line);
      _debug_processLine(line);
    }
  }
}

void _debug_processLine(const char* line) {
  if (strlen(line) == 0) {
  } else {
    printf("invalid debug command: %s\n", line);
  }
  printf("> ");
}
