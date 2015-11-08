
#include <version.h>
#include <stdio.h>
#include <string.h>
#include "platform_config.h"
#include <utils/debug.h>
#include <contiki/core/sys/process.h>
#include <contiki/core/sys/etimer.h>
#include <contiki/core/net/ip/tcpip.h>
#include "rs232.h"
#include "network.h"

uint8_t MAC_ADDRESS[6] = {0xa4, 0x6f, 0xa7, 0xa5, 0x25, 0xd2};

void setup() {
  printf("setup\n");

  process_init();
  process_start(&etimer_process, NULL);
  process_start(&tcpip_process, NULL);

  debug_setup();
  rs232_setup();
  network_setup();
  printf("setup complete\n");
  printf("> ");
}

void loop() {
  process_run();
}

void uip_log(char *msg) {
  printf("contiki: %s\n", msg);
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

