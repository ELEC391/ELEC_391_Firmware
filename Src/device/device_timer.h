/**
 * @file device_time.h
 * @brief Device timer configuration
 */

#ifndef DEVICE_TIMER_H_
#define DEVICE_TIMER_H_

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "device_timer.h"

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

// Public defines that may be used by other files

/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/
typedef enum DeviceTimer_Num
{
    MAIN_CONTROL_TIMER = 0,
    SIGNAL_FILTER_TIMER,
    NUM_DEVICE_TIMERS
} DeviceTimer_Num;
typedef enum DeviceEncoder_Num
{
    X_AXIS_ENCODER = 0,
    Y_AXIS_ENCODER,
    NUM_DEVICE_ENCODERS
} DeviceEncoder_Num;

/******************************************************************************/
/*                       G L O B A L  V A R I A B L E S                       */
/******************************************************************************/


/******************************************************************************/
/*                             F U N C T I O N S                              */
/******************************************************************************/

/**
 * @brief Device initialization function
 *
 * configures all timers
 *
 */
void DeviceTimer_init(void);

/**
 * @brief Will Start requested timer interrupt
 *
 * @param timer Enum value for desired timer
 */
void DeviceTimer_startIrq(DeviceTimer_Num timer);

/**
 * @brief To be called inside ISR to handle interrupt
 *
 * @param timer Enum value for desired timer
 */
void DeviceTimer_handleIrq(DeviceTimer_Num timer);

/**
 * @brief Will start desired encoder to count quadrature pulses
 *
 * @param encoder Enum value for desired encoder
 */
void DeviceTimer_startEncoder(DeviceEncoder_Num encoder);

/**
 * @brief Return count of request encoder
 *
 * @param encoder Enum value for desired encoder
 * @return Count register value
 */
volatile uint16_t DeviceTimer_getEncoderCount(DeviceEncoder_Num encoder);

/**
 * @brief Return bool if counting downwards
 *
 * @param encoder Enum value for desired encoder
 * @return bool of count direction
 */
volatile bool DeviceTimer_isEncoderCountingDown(DeviceEncoder_Num encoder);

/**
 * @brief Return encoder auto reloader register
 *
 * @param encoder Enum value for desired encoder
 * @return 32 bit values of auto reload register
 */
volatile uint32_t DeviceTimer_getEncoderAutoReload(DeviceEncoder_Num encoder);

/******************************************************************************/
/*                       I N L I N E  F U N C T I O N S                       */
/******************************************************************************/

// Inline function declarations and implementations

#endif // DEVICE_TIMER_H_