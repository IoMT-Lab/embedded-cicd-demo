#ifndef __SERIAL_H
#define __SERIAL_H

#include "stm32wb0x_hal.h"
#define DEFAULT_BUFFER_SIZE 16

void Serial_Init(UART_HandleTypeDef *huart);
void Ready_Wait(void);
void Send(const char *msg);
void Send_Line(const char *msg);
void Send_Raw(const char *msg, uint8_t len);
uint8_t Receive(char *msg);
uint8_t Receive_Line(char *msg);
void Receive_Raw(char *msg, uint8_t len);

#endif