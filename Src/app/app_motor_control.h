/**
 * @file app_motor_control.h
 * @brief App for closed loop postion control of motor
 */

#ifndef APP_MOTOR_CONTROL_H_
#define APP_MOTOR_CONTROL_H_

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "app_motor_control.h"

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

// Public defines that may be used by other files

/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/

typedef enum AppMotorControl_Num
{
    X_AXIS_CONTROLLER = 0U,
    Y_AXIS_CONTROLLER,
    NUM_APP_CONTROLLER
} AppMotorControl_Num;

/******************************************************************************/
/*                       G L O B A L  V A R I A B L E S                       */
/******************************************************************************/

// Public global variables that may be used by other files
// (always declare extern)

/******************************************************************************/
/*                             F U N C T I O N S                              */
/******************************************************************************/

void AppMotorControl_init(void);
void AppMotorControl_10kHz(void);
void AppMotorControl_setKp(AppMotorControl_Num contorller, float_t kp);
void AppMotorControl_requestSetPoint(AppMotorControl_Num contorller, float_t setPointRequest);
void AppMotorControl_configureController(AppMotorControl_Num contorller, bool controllerEnabled);

/******************************************************************************/
/*                       I N L I N E  F U N C T I O N S                       */
/******************************************************************************/

// Inline function declarations and implementations

#endif // APP_MOTOR_CONTROL_H_
