#include "app/app.h"
#include "app/as7341.h"

#include "globals.h"
#include "stm32wb0x_nucleo.h"
#include "utils/serial.h"

#define POLL_WAIT_MS 100
#define INTERRUPT_WAIT_MS 1000
#define LOW_THRESHOLD 1000
#define HIGH_THRESHOLD 10000
#define ENABLE_WAITING FALSE

BOOL interrupts = FALSE;

BOOL check_identifiers()
{
    uint8_t aux_id = PD_Get_AuxID();
    uint8_t rev_id = PD_Get_RevID();
    uint8_t id = PD_Get_ID();

    return (aux_id == EXPECTED_AUX_ID) && (rev_id == EXPECTED_REV_ID) && (id == EXPECTED_ID);
}

void device_initialization(BOOL enable_waiting, uint16_t low_threshold, uint16_t high_threshold)
{
    interrupts = enable_waiting;

    PD_I2c_Init(get_i2c());
    PD_Power_Off();
    HAL_Delay(10);
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
    PD_enable_sleep_after_interrupt();

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
    HAL_Delay(500);

    reset();
}

void pulse()
{
    PD_LED_On();
    HAL_Delay(100);
    PD_LED_Off();
}

BOOL get_measurement(uint16_t *data)
{
    if (PD_Get_Valid() == TRUE)
    {
        (*data) = PD_Get_Value();
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void reset()
{
    if (interrupts)
    {
        PD_clear_interrupts();
    }
    else
    {
    }
}

void wait_for_measurement(void)
{
    HAL_Delay(10);
    while ((PD_Get_LowInterrupt() == FALSE) && (PD_Get_HighInterrupt() == FALSE))
    {
        HAL_Delay(INTERRUPT_WAIT_MS);
    }
}

void App()
{
    uint16_t measurement = 0;
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

        if (get_measurement(&measurement) == TRUE && measurement < 10)
        {

            pulse();
        }

        reset();
    }
}