/**
 * @file app_motor.h
 * @brief Motor App
 */

#ifndef APP_MOTOR_H_
#define APP_MOTOR_H_

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "app_motor.h"

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

void AppMotor_10kHz(void);
void AppMotor_init(void);
float_t AppMotor_getVelocity_Raw(void);
float_t AppMotor_getPosition_Raw(void);
float_t AppMotor_getVelocity_10kHz(void);
float_t AppMotor_getPosition_10kHz(void);
int16_t AppMotor_getEncoderVelocity(void);
int64_t AppMotor_getEncoderCount(void);

/******************************************************************************/
/*                       I N L I N E  F U N C T I O N S                       */
/******************************************************************************/

// Inline function declarations and implementations

#endif // APP_MOTOR_H_
