#ifndef autocorrelator_h
#define autocorrelator_h

#include <stdint.h>
#include "utils.h"

int autocorrelate(int period, uint8_t *restrict hard, int len);

#endif
