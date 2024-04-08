/**
 * @file app_motor.h
 * @brief Motor App
 */

#ifndef APP_MOTOR_H_
#define APP_MOTOR_H_

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "device_timer.h"

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/


/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/

// Public typedefs that may be used by other files

/******************************************************************************/
/*                       G L O B A L  V A R I A B L E S                       */
/******************************************************************************/

// Public global variables that may be used by other files
// (always declare extern)

/******************************************************************************/
/*                             F U N C T I O N S                              */
/******************************************************************************/

void AppMotor_50kHz(void);
void AppMotor_init(void);
void AppMotor_reinitializeData(void);
float_t AppMotor_getVelocity_Raw(DeviceEncoder_Num encoder);
float_t AppMotor_getPosition_Raw(DeviceEncoder_Num encoder);
float_t AppMotor_getVelocity_50kHz(DeviceEncoder_Num encoder);
float_t AppMotor_getPosition_50kHz(DeviceEncoder_Num encoder);
int16_t AppMotor_getEncoderVelocity(DeviceEncoder_Num encoder);
int64_t AppMotor_getEncoderCount(DeviceEncoder_Num encoder);

/******************************************************************************/
/*                       I N L I N E  F U N C T I O N S                       */
/******************************************************************************/

// Inline function declarations and implementations

#endif // APP_MOTOR_H_
