#include "utils/serial.h"

#include <stdio.h>
#include <string.h>

#include "globals.h"

UART_HandleTypeDef *huart;

void Serial_Init(UART_HandleTypeDef *uart)
{
    huart = uart;
}

void Ready_Wait(void)
{
    // Wait for the end of the transfer
    while (HAL_UART_GetState(huart) != HAL_UART_STATE_READY)
    {
    }
}

void Send(const char *msg)
{
    char buffer[DEFAULT_BUFFER_SIZE];
    strncpy(buffer, msg, DEFAULT_BUFFER_SIZE);
    Send_Raw(buffer, DEFAULT_BUFFER_SIZE);
    Ready_Wait();
}

void Send_Line(const char *msg)
{
    char buffer[DEFAULT_BUFFER_SIZE];
    memset(buffer, '\0', DEFAULT_BUFFER_SIZE);
    strncpy(buffer, msg, DEFAULT_BUFFER_SIZE - 1);
    strcat(buffer, "\n");
    Send_Raw(buffer, DEFAULT_BUFFER_SIZE);
    Ready_Wait();
}

void Send_Raw(const char *msg, uint8_t len)
{
    /* User start transmission data through "TxBuffer" buffer */
    if (HAL_UART_Transmit_DMA(huart, (uint8_t *)msg, len) != HAL_OK)
    {
        /* Transfer error in transmission process */
        Error_Handler();
    }
}

uint8_t Receive(char *msg)
{
    memset(msg, '\0', DEFAULT_BUFFER_SIZE + 1);
    char buffer[DEFAULT_BUFFER_SIZE];
    Receive_Raw(buffer, DEFAULT_BUFFER_SIZE);
    Ready_Wait();

    strncpy(msg, buffer, DEFAULT_BUFFER_SIZE);
    return strlen(msg);
}

uint8_t Receive_Line(char *msg)
{
    memset(msg, '\0', DEFAULT_BUFFER_SIZE + 1);
    char buffer[DEFAULT_BUFFER_SIZE];
    Receive_Raw(buffer, DEFAULT_BUFFER_SIZE);
    Ready_Wait();

    for (uint8_t index = 0; index < DEFAULT_BUFFER_SIZE; index++)
    {
        if (buffer[index] == '\n')
        {
            return index;
        }
        else
        {
            msg[index] = buffer[index];
        }
    }
    return DEFAULT_BUFFER_SIZE;
}

void Receive_Raw(char *msg, uint8_t len)
{
    /* Any data received will be stored in "msg" buffer : the number max of
     data received is 'len' */
    if (HAL_UART_Receive_DMA(huart, (uint8_t *)msg, len) != HAL_OK)
    {
        /* Transfer error in reception process */
        Error_Handler();
    }
}
