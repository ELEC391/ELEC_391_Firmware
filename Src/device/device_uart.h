/**
 * @file device_uart.h
 * @brief  Base UART config and functions
 */

#ifndef DEVICE_UART_H_
#define DEVICE_UART_H_

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "device_uart.h"

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

// Public defines that may be used by other files

/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/

typedef enum DeviceUart_Num
{
    MAIN_LOGGING_CHANNEL = 0,
    NUM_UART_CONNECTIONS
} DeviceUart_Num;

/******************************************************************************/
/*                       G L O B A L  V A R I A B L E S                       */
/******************************************************************************/

// Public global variables that may be used by other files
// (always declare extern)

/******************************************************************************/
/*                             F U N C T I O N S                              */
/******************************************************************************/

/**
 * @brief Device initialization function
 *
 * configures all timers
 *
 */
void DeviceUart_init(void);

/**
 * @brief Will send message over requested UART Connection
 *
 * @param uartConnection Enum value for desired uart channel
 * @param msg String containing message to be transmitted
 */
void DeviceUart_sendMessage(DeviceUart_Num uartChannel, char* msg);

/******************************************************************************/
/*                       I N L I N E  F U N C T I O N S                       */
/******************************************************************************/

// Inline function declarations and implementations

#endif // DEVICE_UART_H_
