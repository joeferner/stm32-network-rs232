
#include <version.h>
#include <stdio.h>
#include <string.h>
#include "platform_config.h"
#include "debug.h"

void setup() {
  printf("setup\n");
  debug_setup();
  printf("setup complete\n");
  printf("> ");
}

void loop() {
  debug_tick();
}


#ifdef __GNUC__
#  define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#  define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#  define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#  define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE {
  HAL_UART_Transmit(&DEBUG_UART, (uint8_t *)&ch, 1, MAX_TIMEOUT);
  return ch;
}

GETCHAR_PROTOTYPE {
  char ch;
  HAL_UART_Receive(&DEBUG_UART, (uint8_t *)&ch, 1, MAX_TIMEOUT);
  return ch;
}

