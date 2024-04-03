/**
 * @file app_bridge.c
 * @brief app that interfaces with H-Bridges
 */

#ifndef APP_BRIDGE_H_
#define APP_BRIDGE_H_

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "app_bridge.h"
#include <math.h> // TODO -- not sure why this is needed for float_t used setBridge, this is not required in other apps

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

// Public defines that may be used by other files

/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/

typedef enum AppBridge_Num
{
    X_AXIS_BRIDGE = 0U,
    Y_AXIS_BRIDGE,
    NUM_APP_BRIDGE
} AppBridge_Num;

typedef enum AppBridge_State
{
    BRIDGE_OFF = 0U, // Disable Bridge output -- Motor terminals connected to high impeadance
    BRIDGE_STOP, // connect both motor terminals to ground
    BRIDGE_FORWARD, // PWM Channel A Driving
    BRIDGE_REVERSE // PWM Channel B Driving
} AppBridge_State;

/******************************************************************************/
/*                       G L O B A L  V A R I A B L E S                       */
/******************************************************************************/

/******************************************************************************/
/*                             F U N C T I O N S                              */
/******************************************************************************/

void AppBridge_init(void);
void AppBridge_10kHz(void);
void AppBridge_setBridge(AppBridge_Num bridge, AppBridge_State state, float_t dutyCycle);

/******************************************************************************/
/*                       I N L I N E  F U N C T I O N S                       */
/******************************************************************************/

// Inline function declarations and implementations

#endif // APP_BRIDGE_H_
