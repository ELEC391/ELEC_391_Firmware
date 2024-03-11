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

typedef struct{
	int16_t velocity;
	int64_t position;
	uint16_t last_counter_value;
}App_MotorEncoder;

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

void AppMotor_updateEncoder(void);
int16_t AppMotor_getVelocity(void);
int64_t AppMotor_getPostion(void);
void AppMotor_init(void);
/******************************************************************************/
/*                       I N L I N E  F U N C T I O N S                       */
/******************************************************************************/

// Inline function declarations and implementations

#endif // APP_MOTOR_H_
