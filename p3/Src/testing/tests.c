#include "utils/test_harness.h"
#include "app/app.h"
#include "app/as7341.h"

#include "stm32wb0x_hal.h"
#include "globals.h"

#define COUNTOF(__BUFFER__) (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

// P1
TestResult baseline_test()
{
    return PASS;
}

// P2
TestResult i2c_initialization_test()
{
    if (HAL_I2C_IsDeviceReady(get_i2c(), PD_DEVICE_ADDR, 3, 1000) == HAL_OK)
    {
        return PASS;
    }
    else
    {
        return FAIL;
    }
}

// P3
TestResult device_talk_test()
{
    PD_I2c_Init(get_i2c());
    PD_Power_On();
    if (check_identifiers() == TRUE)
    {
        return PASS;
    }
    else
    {
        return FAIL;
    }
}

// P4
TestResult device_initialization_nowait_test()
{
    device_initialization(FALSE, 0x10, 0x100);

    // Interrupt should be disabled
    if (PD_Interrupt_Enabled() == TRUE)
    {
        return FAIL;
    }

    // Thresholds shouldn't be set if interrupt is disabled
    if (PD_Get_HighThreshold() == 0x100)
    {
        return FAIL;
    }

    if (PD_Get_LowThreshold() == 0x10)
    {
        return FAIL;
    }

    // Measurements should be enabled
    if (PD_Measurement_Enabled() == FALSE)
    {
        return FAIL;
    }

    return PASS;
}

// P5
TestResult device_initialization_wait_test()
{
    device_initialization(TRUE, 0x10, 0x100);

    // Interrupt should be enabled
    if (PD_Interrupt_Enabled() == FALSE)
    {
        return FAIL;
    }

    // Thresholds should be set if interrupt is enabled
    if (PD_Get_HighThreshold() != 0x100)
    {
        return FAIL;
    }

    if (PD_Get_LowThreshold() != 0x10)
    {
        return FAIL;
    }

    // Measurements should be enabled
    if (PD_Measurement_Enabled() == FALSE)
    {
        return FAIL;
    }

    return PASS;
}

// P6
TestResult measurement_test()
{
    uint16_t measurement = 0;
    device_initialization(FALSE, 0, 0);

    // Minor delay before taking measurement
    BOOL valid = get_measurement(&measurement);

    // Unless the device is in a pitch-black room, there should always be some level of measurement
    if (valid && measurement > 0)
    {
        return PASS;
    }
    else
    {
        return FAIL;
    }
}

// P7
TestResult interrupt_test()
{
    uint16_t measurement = 0;

    device_initialization(TRUE, 100, 1000);
    wait_for_measurement();

    BOOL valid = get_measurement(&measurement);
    if (valid && measurement > 0)
    {
        return PASS;
    }
    else
    {
        return FAIL;
    }
}

// P8
TestResult pulse_test()
{
    device_initialization(FALSE, 0, 0);
    pulse();

    // Should see LED pulse for 0.5s; no way to programmatically confirm that
    return PASS;
}

// P9
TestResult repeated_interrupt_test()
{
    uint16_t measurement = (uint16_t)-1;

    device_initialization(TRUE, 100, 1000);

    for (int i = 0; i < 5; i++)
    {
        wait_for_measurement();
        BOOL valid = get_measurement(&measurement);
        if (!valid || measurement == (uint16_t)-1)
        {
            return FAIL;
        }
        HAL_Delay(100);
        reset();
        measurement = (uint16_t)-1;
    }

    return PASS;
}

// N1
TestResult repeated_measurement_test()
{
    uint16_t measurement = (uint16_t)-1;

    device_initialization(FALSE, 0, 0);

    for (int i = 0; i < 5; i++)
    {
        BOOL valid = get_measurement(&measurement);
        if (!valid || measurement == (uint16_t)-1)
        {
            return FAIL;
        }
        HAL_Delay(100);
        reset();
        measurement = (uint16_t)-1;
    }

    return PASS;
}

TestCase test_cases[] = {
    baseline_test,
    i2c_initialization_test,
    device_talk_test,
    device_initialization_nowait_test,
    device_initialization_wait_test,
    measurement_test,
    interrupt_test,
    pulse_test,
    repeated_interrupt_test,
    repeated_measurement_test};
int num_cases = COUNTOF(test_cases);
