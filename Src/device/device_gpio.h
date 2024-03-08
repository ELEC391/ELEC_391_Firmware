/**
 * @file device_gpio.h
 * @brief GPIO device configuration
 */

#ifndef DEVICE_GPIO_H_
#define DEVICE_GPIO_H_

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "device_gpio.h"

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

// Public defines that may be used by other files

/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/

typedef enum DeviceGpio_Pin
{
    BOARD_LED_PIN = 1,
    DEBUG_PIN_D7,
    DEBUG_PIN_D8,
    BLUE_BUTTON_PIN,
    NUM_GPIO_PINS
}DeviceGpio_Pin;

/******************************************************************************/
/*                       G L O B A L  V A R I A B L E S                       */
/******************************************************************************/

/******************************************************************************/
/*                             F U N C T I O N S                              */
/******************************************************************************/

/**
 * @brief Device initialization function
 *
 * configures and calls all init functions before entering main loop
 *
 */
void DeviceGpio_init(void);


/**
 * @brief Toggle Specified GPIO Pin
 *
 * Toggles Specfied GPIO PIN
 *
 */
void DeviceGpio_toggle(DeviceGpio_Pin pin_num);

/******************************************************************************/
/*                       I N L I N E  F U N C T I O N S                       */
/******************************************************************************/

// Inline function declarations and implementions

#endif // DEVICE_GPIO_H_