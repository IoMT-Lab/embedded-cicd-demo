#ifndef __APP_H
#define __APP_H

#include "stm32wb0x.h"

BOOL check_identifiers(void);
void device_initialization(BOOL enable_waiting, uint16_t low_threshold, uint16_t high_threshold);
void pulse(void);
BOOL get_measurement(uint16_t *data);
void wait_for_measurement(void);
void reset(void);
void App(void);

#endif