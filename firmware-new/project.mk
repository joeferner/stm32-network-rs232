USER_CFLAGS = -Ilibs/contiki/core
SRCS = \
	src/stm32-network-rs232.c \
	src/rs232.c \
	src/interrupts.c \
	libs/contiki/core/sys/process.c \
	libs/contiki/core/sys/etimer.c \
	libs/contiki/core/sys/timer.c
SSRCS =