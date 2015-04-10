
#include "platform_config.h"
#include <stdio.h>
#include <sys/etimer.h>
#include <net/ip/tcpip.h>
#include <net/ip/resolv.h>
#include "network.h"

uint8_t MAC_ADDRESS[6] = {0xa4, 0x6f, 0xa7, 0xa5, 0x25, 0xd2};

static void setup();
static void setup_spi();
static void loop();

PROCINIT(
  &etimer_process,
  &tcpip_process,
  &resolv_process,

  &dhcp_process
);

int main(void) {
  setup();
  while (1) {
    loop();
  }
  return 0;
}

static void setup() {
  SWJTAG_setup(SWJTAG_State_sw);
  debug_setup();
  time_setup();

  process_init();
  process_start(&etimer_process, NULL);

  setup_spi();
  network_setup();
  printf("setup complete!\n");
}

static void setup_spi() {
  SPI_InitParams spiInit;

  SPI_initParamsInit(&spiInit);
  spiInit.halSpiInitParams.instance = ENC28J60_SPI;
  spiInit.halSpiInitParams.cpol = SPI_CPOL_low;
  spiInit.halSpiInitParams.cpha = SPI_CPHA_1Edge;
  spiInit.halSpiInitParams.baudRatePrescaler = SPI_BaudRatePrescaler_8;
  spiInit.halSpiInitParams.firstBit = SPI_FirstBit_msb;
  spiInit.mosiPort = ENC28J60_MOSI_PORT;
  spiInit.mosiPin = ENC28J60_MOSI_PIN;
  spiInit.misoPort = ENC28J60_MISO_PORT;
  spiInit.misoPin = ENC28J60_MISO_PIN;
  spiInit.sckPort = ENC28J60_SCK_PORT;
  spiInit.sckPin = ENC28J60_SCK_PIN;
  SPI_init(&spiInit);
  SPI_enable(ENC28J60_SPI);

  printf("setup spi complete!\n");
}

static void loop() {
  network_tick();
  process_run();
  etimer_request_poll();
}

void uip_log(char *msg) {
  printf("contiki: %s\n", msg);
}

CCIF unsigned long clock_seconds() {
  return time_ms() / 1000;
}

clock_time_t clock_time(void) {
  return time_ms();
}

void assert_failed(uint8_t *file, uint32_t line) {
  while (1) {
    printf("assert_failed: %s:%lu\n", file, line);
  }
}
