#include "utils/test_harness.h"
#include "utils/serial.h"

#include "stm32wb0x_hal_cortex.h"
#include "stm32wb0x_nucleo.h"

#include <stdio.h>
#include <string.h>

const char PING[] = "PING";
const char RUN[] = "RUN";
const char RESULT[] = "RESULT";
const char RESTART[] = "RESTART";

const char PONG[] = "PONG";

void Respond_Result(int case_num, TestResult result)
{
    char buffer[DEFAULT_BUFFER_SIZE];
    memset(buffer, '\0', DEFAULT_BUFFER_SIZE);

    sprintf(buffer, "%s %d %d", RESULT, case_num, result);
    Send_Line(buffer);
}

void Harness_Loop(TestCase *test_cases, int num_cases)
{
    char buffer[DEFAULT_BUFFER_SIZE];
    int last_test_case = -1;
    TestResult last_result = UNKNOWN;

    Send_Line("READY");
    while (1)
    {
        Receive_Line(buffer);
        if (strncmp(buffer, PING, strlen(PING)) == 0)
        {
            Send_Line(PONG);
        }
        else if (strncmp(buffer, RUN, strlen(RUN)) == 0)
        {
            last_test_case = -1;
            last_result = UNKNOWN;
            sscanf(buffer, "RUN %d", &last_test_case);

            if (last_test_case >= 0 && last_test_case < num_cases)
            {
                BSP_LED_Off(LD2);
                BSP_LED_Off(LD3);
                last_result = test_cases[last_test_case]();

                if (last_result == PASS)
                {
                    // Turn on green LED
                    BSP_LED_On(LD2);
                }
                else
                {
                    // Turn on red LED
                    BSP_LED_On(LD3);
                }
                // Delay 0.5s to show LED, not strictly necessary
                HAL_Delay(500);
            }

            Respond_Result(last_test_case, last_result);
        }
        else if (strncmp(buffer, RESULT, strlen(RESULT)) == 0)
        {
            Respond_Result(last_test_case, last_result);
        }
        else if (strncmp(buffer, RESTART, strlen(RESTART)) == 0)
        {
            Send_Line(RESTART);
            HAL_NVIC_SystemReset();
        }
        else
        {
            // Invalid command
        }
    }
    Send_Line("SHUTDOWN");
}