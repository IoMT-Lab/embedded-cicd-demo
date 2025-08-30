#include "utils/test_harness.h"
#include "app/app.h"
#include "app/as7341.h"

#include "stm32wb0x_hal.h"
#include "globals.h"

#define COUNTOF(__BUFFER__) (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

TestResult baseline_test()
{
    return PASS;
}

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

TestResult measurement_test()
{
    device_initialization(FALSE, 0, 0);

    // Minor delay before taking measurement
    uint16_t measurement = get_measurement(TRUE);

    // Unless the device is in a pitch-black room, there should always be some level of measurement
    if (measurement > 0)
    {
        return PASS;
    }
    else
    {
        return FAIL;
    }
}

TestResult interrupt_test()
{
    device_initialization(TRUE, 100, 1000);
    wait_for_measurement();

    uint16_t measurement = get_measurement(TRUE);
    if (measurement > 0)
    {
        return PASS;
    }
    else
    {
        return FAIL;
    }
}

TestResult pulse_test()
{
    device_initialization(FALSE, 0, 0);
    pulse();

    // Should see LED pulse for 0.5s; no way to programmatically confirm that
    return PASS;
}

TestResult measurement_test_nodelay()
{
    device_initialization(FALSE, 0, 0);

    // Immediately take measurement
    uint16_t measurement = get_measurement(FALSE);

    // Unless the device is in a pitch-black room, there should always be some level of measurement
    if (measurement > 0)
    {
        return PASS;
    }
    else
    {
        return FAIL;
    }
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
    measurement_test_nodelay,
};
int num_cases = COUNTOF(test_cases);
