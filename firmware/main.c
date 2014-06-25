#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_rcc.h>
#include <misc.h>
#include <string.h>
#include "debug.h"
#include "delay.h"
#include "time.h"
#include "ring_buffer.h"
#include "platform_config.h"
#include "network.h"

void setup();
void loop();
void spi_setup();
void network_setup();

void enc28j60_spi_assert();
void enc28j60_spi_deassert();
uint8_t enc28j60_spi_transfer(uint8_t d);

#define MAX_LINE_LENGTH 100
#define INPUT_BUFFER_SIZE 100
uint8_t g_usartInputBuffer[INPUT_BUFFER_SIZE];
ring_buffer_u8 g_usartInputRingBuffer;

uint8_t IP_ADDRESS[4] = {192, 168, 1, 101};
uint8_t GATEWAY_ADDRESS[4] = {192, 168, 1, 1};
uint8_t NETMASK_ADDRESS[4] = {255, 255, 255, 0};
uint8_t MAC_ADDRESS[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};

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

  spi_setup();
  network_setup();

  time_setup();

  debug_led_set(0);
  debug_write_line("?END setup");
}

void loop() {
  network_tick();
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

void spi_setup() {
  debug_write_line("?spi_setup");

  SPI_InitTypeDef spiInitStruct;
  GPIO_InitTypeDef gpioConfig;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

  enc28j60_spi_deassert(); // set pin high before initializing as output pin to not false trigger CS
  RCC_APB2PeriphClockCmd(ENC28J60_CS_RCC, ENABLE);
  gpioConfig.GPIO_Pin = ENC28J60_CS_PIN;
  gpioConfig.GPIO_Mode = GPIO_Mode_Out_PP;
  gpioConfig.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ENC28J60_CS_PORT, &gpioConfig);
  enc28j60_spi_deassert();

  // Configure SPI1 pins: SCK (pin 5) and MOSI (pin 7)
  gpioConfig.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
  gpioConfig.GPIO_Speed = GPIO_Speed_50MHz;
  gpioConfig.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &gpioConfig);

  // Configure SPI1 pins: MISO (pin 6)
  gpioConfig.GPIO_Pin = GPIO_Pin_6;
  gpioConfig.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &gpioConfig);

  // init SPI
  SPI_StructInit(&spiInitStruct);
  spiInitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  spiInitStruct.SPI_Mode = SPI_Mode_Master;
  spiInitStruct.SPI_DataSize = SPI_DataSize_8b;
  spiInitStruct.SPI_NSS = SPI_NSS_Soft;
  spiInitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
  spiInitStruct.SPI_FirstBit = SPI_FirstBit_MSB;

  // Mode 1 (CPOL = 0, CPHA = 1)
  spiInitStruct.SPI_CPOL = SPI_CPOL_Low;
  spiInitStruct.SPI_CPHA = SPI_CPHA_2Edge;

  SPI_Init(SPI1, &spiInitStruct);

  SPI_Cmd(SPI1, ENABLE);
}

