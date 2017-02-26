USER_CFLAGS = 
SRCS = \
	src/stm32-network-rs232.c \
	src/rs232.c \
	src/network.c \
	src/interrupts.c \
	libs/enc28j60/enc28j60.c \
	libs/utils/debug.c \
	libs/utils/time.c \
	libs/utils/timer.c \
	libs/utils/syscalls.c \
	libs/utils/ringbufferdma.c \
	libs/utils/ringbuffer.c \
	libs/utils/utils.c \
	libs/network/dhcp.c \
	libs/network/interface.c \
	libs/network/ipv4.c \
	libs/network/udp.c \
	libs/network/tcp.c \
	libs/network/mac.c \
	libs/network/arp.c \
	libs/network/icmp.c \
	libs/network/checksum.c
SSRCS =
