
#ifndef RS232_H
#define RS232_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

void rs232_setup();
void rs232_write(uint8_t b);

#ifdef	__cplusplus
}
#endif

#endif // RS232_H
