#ifndef __UIP_CONF_H__
#define __UIP_CONF_H__

#include <inttypes.h>

/* for TCP */
#define UIP_SOCKET_NUMPACKETS    5
#define UIP_CONF_MAX_CONNECTIONS 4

/* for UDP */
#define UIP_CONF_UDP             1
#define UIP_CONF_BROADCAST       1
#define UIP_CONF_UDP_CONNS       4

/* number of attempts on write before returning number of bytes sent so far
 * set to -1 to block until connection is closed by timeout */
#define UIP_ATTEMPTS_ON_WRITE    -1

/* timeout after which UIPClient::connect gives up. The timeout is specified in seconds.
 * if set to a number <= 0 connect will timeout when uIP does (which might be longer than you expect...) */
#define UIP_CONNECT_TIMEOUT      -1

/**
 * 8 bit datatype
 *
 * This typedef defines the 8-bit type used throughout uIP.
 *
 * \hideinitializer
 */
typedef uint8_t u8_t;

/**
 * 16 bit datatype
 *
 * This typedef defines the 16-bit type used throughout uIP.
 *
 * \hideinitializer
 */
typedef uint16_t u16_t;

/**
 * Statistics datatype
 *
 * This typedef defines the dataype used for keeping statistics in
 * uIP.
 *
 * \hideinitializer
 */
typedef unsigned short uip_stats_t;

/**
 * Maximum number of TCP connections.
 * (see uipethernet-conf.h)
 * \hideinitializer
 *
 * #define UIP_CONF_MAX_CONNECTIONS 4
 */

/**
 * Maximum number of listening TCP ports.
 *
 * \hideinitializer
 */
#define UIP_CONF_MAX_LISTENPORTS 4

/**
 * uIP buffer size.
 *
 * \hideinitializer
 */
#define UIP_CONF_BUFFER_SIZE     98
//#define UIP_CONF_BUFFER_SIZE     118

/**
 * The TCP maximum segment size.
 *
 * This is should not be to set to more than
 * UIP_BUFSIZE - UIP_LLH_LEN - UIP_TCPIP_HLEN.
 */

#define UIP_CONF_TCP_MSS 512

/**
 * The size of the advertised receiver's window.
 *
 * Should be set low (i.e., to the size of the uip_buf buffer) is the
 * application is slow to process incoming data, or high (32768 bytes)
 * if the application processes data quickly.
 *
 * \hideinitializer
 */
#define UIP_CONF_RECEIVE_WINDOW 512

/**
 * CPU byte order.
 *
 * \hideinitializer
 */
#define UIP_CONF_BYTE_ORDER      LITTLE_ENDIAN

/**
 * Logging on or off
 *
 * \hideinitializer
 */
#define UIP_CONF_LOGGING         0

/**
 * UDP support on or off
 * (see uipethernet-conf.h)
 * \hideinitializer
 *
 * #define UIP_CONF_UDP             1
 *
 * #define UIP_CONF_UDP_CONNS       4
 */

/**
 * UDP checksums on or off
 *
 * \hideinitializer
 */
#define UIP_CONF_UDP_CHECKSUMS   1

/**
 * UDP Broadcast (receive) on or off
 * (see uipethernet-conf.h)
 * \hideinitializer
 * #define UIP_CONF_BROADCAST    1
 */


/**
 * uIP statistics on or off
 *
 * \hideinitializer
 */
#define UIP_CONF_STATISTICS      0

// SLIP
//#define UIP_CONF_LLH_LEN 0

#include "psock.h"

typedef struct network_uip_state {
  struct psock p;
  char inputbuffer[10];
  char name[40];
} uip_tcp_appstate_t;

void uip_appcall(void);

#define UIP_APPCALL uip_appcall

typedef void* uip_udp_appstate_t;

void uip_udp_appcall(void);

#define UIP_UDP_APPCALL uip_udp_appcall

#define CC_REGISTER_ARG register

//#define UIP_ARCH_CHKSUM 1

#endif /* __UIP_CONF_H__ */
