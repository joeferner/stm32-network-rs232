
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include <misc.h>
#include <string.h>
#include "debug.h"
#include "delay.h"
#include "time.h"
#include "ring_buffer.h"
#include "platform_config.h"

void setup();
void loop();

#define MAX_LINE_LENGTH 100
#define INPUT_BUFFER_SIZE 100
uint8_t g_usartInputBuffer[INPUT_BUFFER_SIZE];
ring_buffer_u8 g_usartInputRingBuffer;

int main(void) {
  setup();
  while (1) {
    loop();
  }
  return 0;
}

void setup() {
  // Configure the NVIC Preemption Priority Bits
  // 2 bit for pre-emption priority, 2 bits for subpriority
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  debug_setup();
  debug_led_set(1);
  debug_write_line("?BEGIN setup");

  ring_buffer_u8_init(&g_usartInputRingBuffer, g_usartInputBuffer, INPUT_BUFFER_SIZE);

  time_setup();

  debug_led_set(0);
  debug_write_line("?END setup");
}

void loop() {
  delay_ms(1000);
  debug_led_set(0);
  delay_ms(1000);
  debug_led_set(1);
}

void assert_failed(uint8_t* file, uint32_t line) {
  debug_write("-assert_failed: file ");
  debug_write((const char*) file);
  debug_write(" on line ");
  debug_write_u32(line, 10);
  debug_write_line("");

  /* Infinite loop */
  while (1) {
  }
}

void on_usart1_irq() {
  char line[MAX_LINE_LENGTH];

  if (USART_GetITStatus(DEBUG_USART, USART_IT_RXNE) != RESET) {
    uint8_t data[1];
    data[0] = USART_ReceiveData(DEBUG_USART);

    ring_buffer_u8_write(&g_usartInputRingBuffer, data, 1);
    while (ring_buffer_u8_readline(&g_usartInputRingBuffer, line, MAX_LINE_LENGTH) > 0) {
      if(strcmp(line, "!CONNECT\n") == 0) {
        debug_write_line("+OK");
        debug_write_line("!clear");
        debug_write_line("!set name,stm32-network-rs232");
        debug_write_line("!set description,'STM32 Network RS232'");
      } else {
        debug_write("?Unknown command: ");
        debug_write_line(line);
      }
    }
  }
}
