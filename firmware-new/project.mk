USER_CFLAGS = -Ilibs/contiki/core -Ilibs/contiki/core/net/
SRCS = \
	src/stm32-network-rs232.c \
	src/rs232.c \
	src/network.c \
	src/interrupts.c \
	libs/contiki/core/sys/process.c \
	libs/contiki/core/sys/etimer.c \
	libs/contiki/core/sys/timer.c \
	libs/contiki/core/net/ip/tcpip.c \
	libs/contiki/core/net/ipv4/uip.c \
	libs/contiki/core/net/ipv4/uip_arp.c \
	libs/contiki/dev/enc28j60/enc28j60.c
SSRCS =