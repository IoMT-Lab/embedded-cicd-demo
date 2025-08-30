#ifndef __GLOBALS_H
#define __GLOBALS_H

#include <stm32wb0x_hal_i2c.h>
#include <stm32wb0x_hal_uart.h>

extern I2C_HandleTypeDef *get_i2c(void);
extern UART_HandleTypeDef *get_uart(void);
extern void Error_Handler(void);

#endif