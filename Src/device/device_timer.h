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
    NUM_DEVICE_TIMERS
} DeviceTimer_Num;

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
void DeviceTimer_init(void);

/**
 * @brief Will handle timer interrupt if enabled otherwise no-op
 *
 */
void DeviceTimer_handleIrq(DeviceTimer_Num timer);

/******************************************************************************/
/*                       I N L I N E  F U N C T I O N S                       */
/******************************************************************************/

// Inline function declarations and implementions

#endif // DEVICE_TIMER_H_