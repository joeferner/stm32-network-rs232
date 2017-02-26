
#ifndef _RS232_H_
#define _RS232_H_

void rs232_setup();
void rs232_tx(const char* str);
void rs232_tick();
extern void rs232_processLine(const char* line);

#endif
