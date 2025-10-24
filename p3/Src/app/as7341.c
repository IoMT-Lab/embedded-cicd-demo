#include "app/as7341.h"

#define ADDR PD_DEVICE_ADDR
#define REG_ENABLE 0x80
#define REG_CONFIG 0x70
#define REG_CFG0 0xA9
#define REG_LED 0x74
#define REG_AUXID 0x90
#define REG_REVID 0x91
#define REG_ID 0x92
#define REG_INTENAB 0xF9
#define REG_STAT 0x71
#define REG_STATUS 0x93
#define REG_STATUS2 0xA3
#define REG_STATUS3 0xA4
#define REG_STATUS5 0xA6
#define REG_STATUS6 0xA7
#define REG_FD_STATUS 0xDB
#define REG_CFG3 0xAC
#define REG_CH0_DATA_L 0x95
#define REG_SP_TH_L_LSB 0x84
#define REG_SP_TH_H_LSB 0x86
#define REG_CONTROL 0xFA

I2C_HandleTypeDef *hi2c;

void write_register(uint16_t reg, uint8_t value)
{
    HAL_I2C_Mem_Write(hi2c, ADDR, reg, I2C_MEMADD_SIZE_8BIT, &value, 1, 100);
}

uint8_t read_register(uint16_t reg)
{
    uint8_t data = 0x00;
    HAL_I2C_Mem_Read(hi2c, ADDR, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
    return data;
}

void write_double_register(uint16_t reg, uint16_t value)
{
    uint8_t data[2];
    data[0] = value & 0xFF;
    data[1] = (value >> 8) & 0xFF;
    HAL_I2C_Mem_Write(hi2c, ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 2, 100);
}

uint16_t read_double_register(uint16_t reg)
{
    uint8_t data[2];
    HAL_I2C_Mem_Read(hi2c, ADDR, reg, I2C_MEMADD_SIZE_8BIT, &data, 2, 100);
    return (data[1] << 8) | data[0];
}

void PD_I2c_Init(I2C_HandleTypeDef *i2c)
{
    hi2c = i2c;
}

void PD_Power_On()
{
    write_register(REG_ENABLE, 1);
}

void PD_Power_Off()
{
    write_register(REG_ENABLE, 0);
}

void PD_Measurement_Enable()
{
    write_register(REG_ENABLE, 0b00000011);
}

void PD_Measurement_Disable()
{
    write_register(REG_ENABLE, 0b00000001);
}

BOOL PD_Measurement_Enabled()
{
    return (read_register(REG_ENABLE) & 0b00000010) >> 1;
}

void PD_Initialize_LED()
{
    write_register(REG_CFG0, 0b00010000);
    write_register(REG_CONFIG, 0b00001000);
    write_register(REG_CFG0, 0b00000000);
}

void PD_LED_On()
{
    write_register(REG_CFG0, 0b00010000);
    write_register(REG_LED, 0b10000100);
    write_register(REG_CFG0, 0b00000000);
}

void PD_LED_Off()
{
    write_register(REG_CFG0, 0b00010000);
    write_register(REG_LED, 0b00000100);
    write_register(REG_CFG0, 0b00000000);
}

uint8_t PD_Get_AuxID()
{
    return read_register(REG_AUXID) & 0b00001111;
}

uint8_t PD_Get_RevID()
{
    return read_register(REG_REVID) & 0b00000111;
}

uint8_t PD_Get_ID()
{
    return (read_register(REG_ID) & 0b11111100) >> 2;
}

BOOL PD_Get_IntBusy()
{
    return (read_register(REG_STATUS6) & 0x1);
}

BOOL PD_Get_Ready()
{
    return read_register(REG_STAT) & 0x1;
}

BOOL PD_Get_Valid()
{
    return (read_register(REG_STATUS2) & 0b01000000) >> 6;
}

void PD_Enable_Interrupt()
{
    write_register(REG_INTENAB, 0b00001000);
}

void PD_Disable_Interrupt()
{
    write_register(REG_INTENAB, 0b00000000);
}

BOOL PD_Interrupt_Enabled()
{
    return (read_register(REG_INTENAB) & 0b00001000) >> 3;
}

BOOL PD_Get_LowInterrupt()
{
    return (read_register(REG_STATUS3) & 0b00010000) >> 4;
}

BOOL PD_Get_HighInterrupt()
{
    return (read_register(REG_STATUS3) & 0b00100000) >> 5;
}

void PD_Set_LowThreshold(uint16_t threshold)
{
    write_double_register(REG_SP_TH_L_LSB, threshold);
}

void PD_Set_HighThreshold(uint16_t threshold)
{
    write_double_register(REG_SP_TH_H_LSB, threshold);
}

uint16_t PD_Get_LowThreshold()
{
    return read_double_register(REG_SP_TH_L_LSB);
}

uint16_t PD_Get_HighThreshold()
{
    return read_double_register(REG_SP_TH_H_LSB);
}

uint16_t PD_Get_Value()
{
    return read_double_register(REG_CH0_DATA_L);
}

void PD_enable_sleep_after_interrupt()
{
    write_register(REG_CFG0, 0b00010000);
    write_register(REG_CFG3, 0b00010000);
    write_register(REG_CFG0, 0b00000000);
}

void PD_disable_sleep_after_interrupt()
{
    write_register(REG_CFG0, 0b00010000);
    write_register(REG_CFG3, 0b00000000);
    write_register(REG_CFG0, 0b00000000);
}

void PD_clear_interrupts()
{
    write_register(REG_CFG0, 0b00010000);
    read_register(REG_STATUS);
    read_register(REG_STATUS2);
    read_register(REG_STATUS3);
    read_register(REG_STATUS5);
    read_register(REG_STATUS6);
    read_register(REG_FD_STATUS);
    write_register(REG_CONTROL, 0b00000001);
    write_register(REG_CFG0, 0b00000000);
}