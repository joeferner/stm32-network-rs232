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
#include "rs232.h"

#define USART_FLAG_ERRORS (USART_FLAG_ORE | USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE)

void setup();
void loop();
void spi_setup();
void network_setup();
void debug_tick();

void enc28j60_spi_assert();
void enc28j60_spi_deassert();
uint8_t enc28j60_spi_transfer(uint8_t d);

dma_ring_buffer g_debugUsartDmaInputRingBuffer;

uint8_t IP_ADDRESS[4] = {192, 168, 1, 101};
uint8_t GATEWAY_ADDRESS[4] = {192, 168, 1, 1};
uint8_t NETMASK_ADDRESS[4] = {255, 255, 255, 0};
uint8_t MAC_ADDRESS[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};

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

  dma_ring_buffer_init(&g_debugUsartDmaInputRingBuffer, DEBUG_USART_RX_DMA_CH, g_debugUsartRxBuffer, DEBUG_USART_RX_BUFFER_SIZE);

  debug_setup();
  debug_led_set(1);
  debug_write_line("?BEGIN setup");

  rs232_setup();

  spi_setup();
  network_setup();

  time_setup();

  debug_led_set(0);
  debug_write_line("?END setup");
}

void loop() {
  network_tick();
  debug_tick();
  rs232_tick();
  //delay_ms(1000);
  //debug_led_set(0);
  //delay_ms(1000);
  //debug_led_set(1);
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

void debug_tick() {
  char line[MAX_LINE_LENGTH];

  while(dma_ring_buffer_readline(&g_debugUsartDmaInputRingBuffer, line, MAX_LINE_LENGTH)) {
    if(strcmp(line, "!CONNECT\n") == 0) {
      debug_write_line("+OK");
      debug_write_line("!clear");
      debug_write_line("!set name,stm32-network-rs232");
      debug_write_line("!set description,'STM32 Network RS232'");
    } else if(strncmp(line, "!TX", 3) == 0) {
      int len = strlen(line);
      rs232_write((const uint8_t*)(line + 3), len - 3);
      debug_write_line("+OK");
    } else {
      debug_write("?Unknown command: ");
      debug_write_line(line);
    }
  }
}

void spi_setup() {
  debug_write_line("?spi_setup");

  SPI_InitTypeDef spiInitStruct;
  GPIO_InitTypeDef gpioConfig;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

  enc28j60_reset_deassert();
  RCC_APB2PeriphClockCmd(ENC28J60_RESET_RCC, ENABLE);
  gpioConfig.GPIO_Pin = ENC28J60_RESET_PIN;
  gpioConfig.GPIO_Mode = GPIO_Mode_Out_PP;
  gpioConfig.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ENC28J60_RESET_PORT, &gpioConfig);
  enc28j60_reset_deassert();

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
  spiInitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  spiInitStruct.SPI_FirstBit = SPI_FirstBit_MSB;

  // Mode 0 (CPOL = 0, CPHA = 0)
  spiInitStruct.SPI_CPOL = SPI_CPOL_Low;
  spiInitStruct.SPI_CPHA = SPI_CPHA_1Edge;

  SPI_Init(SPI1, &spiInitStruct);

  SPI_Cmd(SPI1, ENABLE);
}

