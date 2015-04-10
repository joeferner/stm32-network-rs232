
#include "platform_config.h"
#include <stdio.h>

static void setup();
static void loop();

int main(void) {
  setup();
  while (1) {
    loop();
  }
  return 0;
}

static void setup() {
  debug_setup();
  printf("setup complete!\n");
}

static void loop() {

}

void assert_failed(uint8_t *file, uint32_t line) {
  while (1) {
    printf("assert_failed: %s:%lu\n", file, line);
  }
}
