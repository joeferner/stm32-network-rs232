
#include <version.h>
#include <stdio.h>
#include <string.h>
#include "platform_config.h"
#include <utils/debug.h>
#include <contiki/core/sys/process.h>
#include <contiki/core/sys/etimer.h>

void setup() {
  printf("setup\n");
  process_init();
  debug_setup();
  printf("setup complete\n");
  printf("> ");
}

void loop() {
  process_run();
}

void debug_processLine(const char* line) {
  if (strlen(line) == 0) {
  } else {
    printf("invalid debug command: %s\n", line);
  }
  printf("> ");
}

void HAL_SYSTICK_Callback() {
  etimer_request_poll();
}

CCIF unsigned long clock_seconds() {
  return HAL_GetTick() / 1000;
}

clock_time_t clock_time(void) {
  return HAL_GetTick();
}
