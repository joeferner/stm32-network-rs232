#include "clock-arch.h"
#include "time.h"

clock_time_t clock_time(void) {
  return (clock_time_t) time_ms();
}
