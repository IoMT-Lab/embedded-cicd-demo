#ifndef __AS7341_H
#define __AS7341_H

#include "stm32wb0x_hal.h"

#define PD_DEVICE_ADDR 0x39 << 1
#define EXPECTED_AUX_ID 0x00
#define EXPECTED_REV_ID 0x00
#define EXPECTED_ID 0x09

void PD_I2c_Init(I2C_HandleTypeDef *i2c);
void PD_Power_On();
void PD_Power_Off();

void PD_Measurement_Enable();
void PD_Measurement_Disable();
BOOL PD_Measurement_Enabled();

void PD_Initialize_LED();
void PD_LED_On();
void PD_LED_Off();

uint8_t PD_Get_AuxID();
uint8_t PD_Get_RevID();
uint8_t PD_Get_ID();

BOOL PD_Get_IntBusy();
BOOL PD_Get_Ready();
BOOL PD_Get_Valid();

void PD_Enable_Interrupt();
void PD_Disable_Interrupt();
BOOL PD_Interrupt_Enabled();
BOOL PD_Get_LowInterrupt();
BOOL PD_Get_HighInterrupt();

void PD_Set_LowThreshold(uint16_t threshold);
void PD_Set_HighThreshold(uint16_t threshold);
uint16_t PD_Get_LowThreshold();
uint16_t PD_Get_HighThreshold();

uint16_t PD_Get_Value();
#endif