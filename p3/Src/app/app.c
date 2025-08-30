#include "app/app.h"
#include "app/as7341.h"

#include "globals.h"
#include "stm32wb0x_nucleo.h"
#include "utils/serial.h"

#define POLL_WAIT_MS 5000
#define INTERRUPT_WAIT_MS 1000
#define LOW_THRESHOLD 1000
#define HIGH_THRESHOLD 10000
#define ENABLE_WAITING TRUE

BOOL check_identifiers()
{
    uint8_t aux_id = PD_Get_AuxID();
    uint8_t rev_id = PD_Get_RevID();
    uint8_t id = PD_Get_ID();

    return (aux_id == EXPECTED_AUX_ID) && (rev_id == EXPECTED_REV_ID) && (id == EXPECTED_ID);
}

void device_initialization(BOOL enable_waiting, uint16_t low_threshold, uint16_t high_threshold)
{
    PD_I2c_Init(get_i2c());
    PD_Power_On();

    // Wait for device to say it is ready
    while (PD_Get_IntBusy() == TRUE)
    {
    }

    if (check_identifiers() == FALSE)
    {
        // Error
    }

    PD_Initialize_LED();

    if (enable_waiting == TRUE)
    {
        PD_Set_LowThreshold(low_threshold);
        PD_Set_HighThreshold(high_threshold);
        PD_Enable_Interrupt();
    }
    else
    {
        PD_Disable_Interrupt();
    }

    PD_Measurement_Enable();
}

void pulse()
{
    PD_LED_On();
    HAL_Delay(500);
    PD_LED_Off();
}

uint16_t get_measurement(BOOL should_delay)
{
    if (should_delay == TRUE)
    {
        HAL_Delay(100);
    }

    if (PD_Get_Valid() == TRUE)
    {
        return PD_Get_Value();
    }
    else
    {
        return 0;
    }
}

void wait_for_measurement()
{
    HAL_Delay(INTERRUPT_WAIT_MS);

    while ((PD_Get_LowInterrupt() == FALSE) && (PD_Get_HighInterrupt() == FALSE))
    {
        HAL_Delay(INTERRUPT_WAIT_MS);
    }
}

void App()
{
    device_initialization(ENABLE_WAITING, LOW_THRESHOLD, HIGH_THRESHOLD);
    while (1)
    {
        if (ENABLE_WAITING)
        {
            wait_for_measurement();
        }
        else
        {
            HAL_Delay(POLL_WAIT_MS);
        }

        if (get_measurement(TRUE) > 0)
        {
            pulse();
        }
    }
}