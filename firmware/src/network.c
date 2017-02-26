
#include <stdio.h>
#include <string.h>
#include <utils/time.h>
#include <enc28j60/enc28j60.h>
#include <network/interface.h>
#include <network/dhcp.h>
#include <network/mac.h>
#include <network/tcp.h>
#include <utils/utils.h>
#include <utils/ringbuffer.h>
#include "network.h"
#include "platform_config.h"

#ifdef NETWORK_DEBUG
#define NETWORK_DEBUG_OUT(format, ...) printf("%s:%d: network: " format, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define NETWORK_DEBUG_OUT(format, ...)
#endif

#define TELNET_PORT 23
#define TELNET_CONNECTION_COUNT 5

uint8_t MAC_ADDRESS[6] = {0xa4, 0x6f, 0xa7, 0xa5, 0x25, 0xd2};
const char* HOST_NAME = "epson-projector";

typedef enum {
  NETWORK_STATE_INIT,
  NETWORK_STATE_WAITING_FOR_DHCP_RESPONSE,
  NETWORK_STATE_DHCP_COMPLETE
} NetworkState;

NetworkInterface _network_nic;
ENC28J60 _network_enc28j60;
NetworkState _network_state;
uint32_t _network_lastStateChange;
TCP_Server _network_telnetServer;
TCP_Connection _network_telnetServerConnections[TELNET_CONNECTION_COUNT];

#define NETWORK_RX_BUFFER_SIZE 32
RingBufferU8 networkRxRing;
uint8_t networkRxBuffer[NETWORK_RX_BUFFER_SIZE];

uint32_t _network_receive(NetworkInterface* nic, uint8_t* buffer, uint32_t len);
uint32_t _network_send(NetworkInterface* nic, uint8_t* buffer, uint32_t len);
void _network_setState(NetworkState newState);
void _network_stateInit();
void _network_stateWaitingForDhcpResponse();
void _network_telnetConnect(NetworkInterface* nic, TCP_Connection* conn);
void _network_telnetDisconnect(NetworkInterface* nic, TCP_Connection* conn);
void _network_telnetReceive(NetworkInterface* nic, TCP_Connection* conn, uint8_t* data, uint32_t dataLength);

void network_setup() {
  NETWORK_DEBUG_OUT("network_setup\n");

  RingBufferU8_init(&networkRxRing, networkRxBuffer, NETWORK_RX_BUFFER_SIZE);
  
  _network_setState(NETWORK_STATE_INIT);

  NetworkInterface_init(&_network_nic, MAC_ADDRESS, _network_receive, _network_send);
  _network_nic.hostName = HOST_NAME;

  _network_enc28j60.spi = &ENC28J60_SPI;
  _network_enc28j60.csPort = ENC28J60_CS_PORT;
  _network_enc28j60.csPin = ENC28J60_CS_PIN;
  _network_enc28j60.resetPort = ENC28J60_RESET_PORT;
  _network_enc28j60.resetPin = ENC28J60_RESET_PIN;
  memcpy(_network_enc28j60.macAddress, MAC_ADDRESS, MAC_ADDRESS_LENGTH);
  HAL_StatusTypeDef status = ENC28J60_setup(&_network_enc28j60);
  if (status != HAL_OK) {
    printf("error with enc28j60 setup\n");
    while (1);
  }
  sleep_ms(100);

  TCP_listen(
    &_network_nic,
    &_network_telnetServer,
    TELNET_PORT,
    _network_telnetServerConnections,
    TELNET_CONNECTION_COUNT,
    _network_telnetConnect,
    _network_telnetDisconnect,
    _network_telnetReceive
  );
}

uint32_t _network_receive(NetworkInterface* nic, uint8_t* buffer, uint32_t len) {
  return ENC28J60_receive(&_network_enc28j60, buffer, len);
}

uint32_t _network_send(NetworkInterface* nic, uint8_t* buffer, uint32_t len) {
  return ENC28J60_send(&_network_enc28j60, buffer, len);
}

void network_tick() {
  NetworkInterface_tick(&_network_nic);
  ENC28J60_tick(&_network_enc28j60);
  switch (_network_state) {
  case NETWORK_STATE_INIT:
    _network_stateInit();
    break;
  case NETWORK_STATE_WAITING_FOR_DHCP_RESPONSE:
    _network_stateWaitingForDhcpResponse();
    break;
  case NETWORK_STATE_DHCP_COMPLETE:
    break;
  }
}

void _network_setState(NetworkState newState) {
  _network_state = newState;
  _network_lastStateChange = HAL_GetTick();
  NETWORK_DEBUG_OUT("state %d\n", _network_state);
}

void network_dhcpRenew() {
  DHCP_sendDiscover(&_network_nic);
  _network_setState(NETWORK_STATE_WAITING_FOR_DHCP_RESPONSE);
}

void network_ifconfig() {
  char buffer[100];

  MAC_macToString(buffer, _network_nic.macAddress);
  printf("MAC Address: %s\n", buffer);
  printf("  Host name: %s\n", _network_nic.hostName);
  IPV4_ipToString(buffer, _network_nic.ipAddress);
  printf(" IP Address: %s\n", buffer);
  IPV4_ipToString(buffer, _network_nic.subnetMask);
  printf("Subnet Mask: %s\n", buffer);
  IPV4_ipToString(buffer, _network_nic.router);
  printf("    Gateway: %s\n", buffer);
  for (int i = 0; i < NETWORK_INTERFACE_MAX_DNS_COUNT; i++) {
    if (_network_nic.dnsServers[i] != 0) {
      IPV4_ipToString(buffer, _network_nic.dnsServers[i]);
      printf(" DNS Server: %s\n", buffer);
    }
  }
}

void _network_stateInit() {
  network_dhcpRenew();
}

void _network_stateWaitingForDhcpResponse() {
  if (_network_nic.ipAddress != 0 ) {
    _network_setState(NETWORK_STATE_DHCP_COMPLETE);
  } else if (HAL_GetTick() - _network_lastStateChange > 5000) {
    _network_setState(NETWORK_STATE_INIT);
  }
}

void _network_telnetConnect(NetworkInterface* nic, TCP_Connection* conn) {
#ifdef NETWORK_DEBUG
  char macAddressString[25];
  char ipAddressString[25];
  MAC_macToString(macAddressString, conn->remoteMac);
  IPV4_ipToString(ipAddressString, conn->remoteIpAddress);
  NETWORK_DEBUG_OUT("telnet connect %s %s:%d\n", macAddressString, ipAddressString, conn->remotePort);
#endif
}

void _network_telnetDisconnect(NetworkInterface* nic, TCP_Connection* conn) {
  NETWORK_DEBUG_OUT("telnet disconnect\n");
}

void network_sendToAllTelnetConnections(const char* line) {
  for (int i = 0; i < TELNET_CONNECTION_COUNT; i++) {
    TCP_Connection* conn = &_network_telnetServerConnections[i];
    if (TCP_isConnected(conn)) {
      NETWORK_DEBUG_OUT("sending to %d: %s\n", i, line);
      TCP_send(&_network_nic, conn, (uint8_t*)line, strlen(line));
    }
  }
}

void _network_telnetReceive(NetworkInterface* nic, TCP_Connection* conn, uint8_t* data, uint32_t dataLength) {
  NETWORK_DEBUG_OUT("telnet receive\n");
#ifdef NETWORK_DEBUG
  printMemory(data, dataLength);
#endif
  if (dataLength > 0 && data[0] == 0xff) {
    NETWORK_DEBUG_OUT("telnet setup (skipping)\n");
    return;
  }
  RingBufferU8_write(&networkRxRing, data, dataLength);
  
  char line[NETWORK_RX_BUFFER_SIZE];
  if (RingBufferU8_readLine(&networkRxRing, line, NETWORK_RX_BUFFER_SIZE) > 0) {
    network_handleTelnetReceive(line);
  }
}
