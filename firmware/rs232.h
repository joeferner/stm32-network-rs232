
#ifndef RS232_H
#define RS232_H

#include <stdint.h>
#include "ring_buffer.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define RS232_INPUT_BUFFER_SIZE 100
extern uint8_t g_rs232UsartInputBuffer[RS232_INPUT_BUFFER_SIZE];
extern dma_ring_buffer g_rs232UsartDmaInputRingBuffer;

void rs232_setup();
void rs232_write(uint8_t b);

#ifdef	__cplusplus
}
#endif

#endif // RS232_H
