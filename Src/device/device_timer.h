/**
 * @file device_time.h
 * @brief Device timer configuration
 */

#ifndef DEVICE_TIMER_H_
#define DEVICE_TIMER_H_

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

// Minimum required set of includes ordered from local to global and grouped
// by category

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

// Public defines that may be used by other files

/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/
typedef enum DeviceTimer_Num
{
    MAIN_CONTROL_TIMER = 1,
    SIGNAL_FILTER_TIMER,
    NUM_DEVICE_TIMERS
} DeviceTimer_Num;

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
void DeviceTimer_startIrq(DeviceTimer_Num timer);
void DeviceTimer_startEncoder(void);

/**
 * @brief Will handle timer interrupt if enabled otherwise no-op
 *
 */
void DeviceTimer_handleIrq(DeviceTimer_Num timer);

uint16_t DeviceTimer_getEncoderCount(void);

/******************************************************************************/
/*                       I N L I N E  F U N C T I O N S                       */
/******************************************************************************/

// Inline function declarations and implementations

#endif // DEVICE_TIMER_H_