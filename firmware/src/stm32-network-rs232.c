
#include <version.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <utils/debug.h>
#include <utils/time.h>
#include "platform_config.h"
#include "rs232.h"
#include "network.h"

void setup() {
  printf("setup\n");

  HAL_IWDG_Start(&hiwdg);

  debug_setup();
  rs232_setup();
  network_setup();
  printf("setup complete\n");
  printf("> ");
}

void loop() {
  HAL_IWDG_Refresh(&hiwdg);
  debug_tick();
  rs232_tick();
  network_tick();
}

void debug_processLine(const char* line) {
  if (strlen(line) == 0) {
  } else if (strncmp(line, "tx", 2) == 0) {
    strcat((char*)line, "\r\n");
    rs232_tx(line + 3);
  } else if (strncmp(line, "send", 4) == 0) {
    network_sendToAllTelnetConnections(line + 5);
  } else if (strcmp(line, "testiwdg") == 0) {
    printf("testing IWDG\n");
    while (1) {
      sleep_ms(1000);
      printf("waiting for IWDG\n");
    }
  } else if (strcmp(line, "dhcp renew") == 0) {
    printf("DHCP renew\n");
    network_dhcpRenew();
  } else if (strcmp(line, "ifconfig") == 0) {
    network_ifconfig();
  } else {
    printf("invalid debug command: %s\n", line);
  }
  printf("> ");
}

void rs232_processLine(const char* line) {
  printf("rs232rx: \"%s\"\n", line);
  network_sendToAllTelnetConnections(line);
}

void network_handleTelnetReceive(const char* line) {
  printf("networkRx: \"%s\"\n", line);
  rs232_tx(line);
}
