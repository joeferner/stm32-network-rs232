
#include <version.h>
#include <stdio.h>
#include <string.h>
#include "platform_config.h"
#include <utils/debug.h>
#include <contiki/core/sys/process.h>
#include "rs232.h"

void setup() {
  printf("setup\n");
  process_init();
  debug_setup();
  rs232_setup();
  printf("setup complete\n");
  printf("> ");
}

void loop() {
  process_run();
}

void debug_processLine(const char* line) {
  if (strlen(line) == 0) {
  } else if(strncmp(line, "tx", 2) == 0) {
    strcat((char*)line, "\r\n");
    rs232_tx(line + 3);
  } else {
    printf("invalid debug command: %s\n", line);
  }
  printf("> ");
}

void rs232_processLine(const char* line) {
  printf("rs232rx: \"%s\"\n", line);
}

