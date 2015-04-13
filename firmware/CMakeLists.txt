PROJECT(stm32-network-rs232)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
ENABLE_LANGUAGE(ASM)

SET(STM32LIB_DIR "../../stm32lib/lib")
SET(CONTIKI_DIR "../../contiki")

INCLUDE_DIRECTORIES(
  ${STM32LIB_DIR}
  ${CONTIKI_DIR}/core
  ${CONTIKI_DIR}/core/net
  .
)

SET(STM32_CHIP "STM32F103RBT6")
SET(STM32_CHIP_DEF "STM32F10X_MD")
SET(STM32_FLASH_SIZE "131072")
SET(STM32_RAM_SIZE "20480")

SET(PROJECT_SOURCES
  ${STM32LIB_DIR}/stm32lib/hal/rcc.c
  ${STM32LIB_DIR}/stm32lib/hal/gpio.c
  ${STM32LIB_DIR}/stm32lib/hal/spi.c
  ${STM32LIB_DIR}/stm32lib/hal/usart.c
  ${STM32LIB_DIR}/stm32lib/hal/exti.c
  ${STM32LIB_DIR}/stm32lib/hal/iwdg.c
  ${STM32LIB_DIR}/stm32lib/hal/nvic.c
  ${STM32LIB_DIR}/stm32lib/usart.c
  ${STM32LIB_DIR}/stm32lib/rcc.c
  ${STM32LIB_DIR}/stm32lib/spi.c
  ${STM32LIB_DIR}/stm32lib/time.c
  ${STM32LIB_DIR}/stm32lib/exti.c
  ${STM32LIB_DIR}/stm32lib/utils.c
  ${STM32LIB_DIR}/stm32lib/debug.c
  ${STM32LIB_DIR}/stm32lib/newlib.c
  ${STM32LIB_DIR}/stm32lib/base64.c
  ${STM32LIB_DIR}/stm32lib/sha1.c
  ${STM32LIB_DIR}/stm32lib/ringbuffer.c
  ${STM32LIB_DIR}/stm32lib/iwdg.c
  ${STM32LIB_DIR}/stm32lib/device/enc28j60/enc28j60.c
  ${STM32LIB_DIR}/stm32lib/contiki/httpd.c

  ${CONTIKI_DIR}/core/net/ipv4/uip.c
  ${CONTIKI_DIR}/core/net/ipv4/uip_arp.c
  ${CONTIKI_DIR}/core/net/ip/psock.c
  ${CONTIKI_DIR}/core/net/ip/dhcpc.c
  ${CONTIKI_DIR}/core/net/ip/tcpip.c
  ${CONTIKI_DIR}/core/net/ip/resolv.c
  ${CONTIKI_DIR}/core/net/ip/uip-udp-packet.c
  ${CONTIKI_DIR}/core/net/ip/uip-nameserver.c
  ${CONTIKI_DIR}/core/sys/timer.c
  ${CONTIKI_DIR}/core/sys/etimer.c
  ${CONTIKI_DIR}/core/sys/process.c
  ${CONTIKI_DIR}/core/sys/procinit.c
  ${CONTIKI_DIR}/core/lib/random.c
  
  system_stm32f10x.c
  main.c
  interrupts.c
  network.c
  rs232.c
)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_FULL_ASSERT -D${STM32_CHIP_DEF}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_FULL_ASSERT -D${STM32_CHIP_DEF}")
SET(CMAKE_EXE_LINKER_FLAGS "-T${CMAKE_CURRENT_BINARY_DIR}/../stm32f10x_md_flash.ld ${CMAKE_EXE_LINKER_FLAGS}")

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}.elf ${PROJECT_SOURCES} stm32f10x_md_flash.ld startup_stm32f10x_md.s)
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME}.elf)

ADD_CUSTOM_TARGET(${CMAKE_PROJECT_NAME}.hex DEPENDS ${CMAKE_PROJECT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Oihex ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.hex)
ADD_CUSTOM_TARGET(${CMAKE_PROJECT_NAME}.bin DEPENDS ${CMAKE_PROJECT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Obinary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin)
ADD_CUSTOM_TARGET(${CMAKE_PROJECT_NAME}.list DEPENDS ${CMAKE_PROJECT_NAME}.elf COMMAND ${CMAKE_OBJDUMP} -x -S ${CMAKE_PROJECT_NAME}.elf > ${CMAKE_PROJECT_NAME}.list)

