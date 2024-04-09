/**
 * @file app_motor_control.c
 * @brief App for closed loop postion control of motor
 */

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "main.h"
#include "app_motor_control.h"
#include "app_motor.h"
#include "app_bridge.h"
#include "lib_pi_compensator.h"
#include <math.h>
#include <stdbool.h>

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

#define CONTROLLER_FREQUENCY 1000.0F // Currently running in 1Khz ISR

// Controller Gains
#define KP 1.0F
#define KI 0.5F

// Controller Limits
#define MAX_DUTY_OUTPUT 100.0F
#define MIN_DUTY_OUTPUT -100.0F // Forward and reverse
#define MAX_INTEGRATOR_LIMIT 250.0F

/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/

typedef struct{
    Lib_PI_Controller controller;
    AppBridge_Num bridge;
    DeviceEncoder_Num encoder;
    float_t requestedSetPoint;
    bool controllerEnabled;
}App_MotorControllerData;

/******************************************************************************/
/*            P R I V A T E  F U N C T I O N  P R O T O T Y P E S             */
/******************************************************************************/

static void sendBridgeRequest(float_t controllerDuty, AppBridge_Num bridge);

/******************************************************************************/
/*               P R I V A T E  G L O B A L  V A R I A B L E S                */
/******************************************************************************/

static App_MotorControllerData ControllerData[NUM_APP_CONTROLLER] =
{
    // Initialize all data to 0
    [X_AXIS_CONTROLLER] =
    {
        .controller =
        {
            .Kp = KP,
            .Ki = KI,
            .limMax = MAX_DUTY_OUTPUT,
            .limMin = MIN_DUTY_OUTPUT,
            .limMaxInt = MAX_INTEGRATOR_LIMIT,
            .limMinInt = - MAX_INTEGRATOR_LIMIT,
            .T = (1.0F / CONTROLLER_FREQUENCY),
            .integrator = 0.0F,
            .prevError = 0.0F,
            .out = 0.0F
        },
        .bridge = X_AXIS_BRIDGE,
        .encoder = X_AXIS_ENCODER,
        .requestedSetPoint = 0.0F,
        .controllerEnabled = false,
    },
    [Y_AXIS_CONTROLLER] =
    {
        .controller =
        {
            .Kp = KP,
            .Ki = KI,
            .limMax = MAX_DUTY_OUTPUT,
            .limMin = MIN_DUTY_OUTPUT,
            .limMaxInt = MAX_INTEGRATOR_LIMIT,
            .limMinInt = - MAX_INTEGRATOR_LIMIT,
            .T = (1.0F / CONTROLLER_FREQUENCY),
            .integrator = 0.0F,
            .prevError = 0.0F,
            .out = 0.0F
        },
        .bridge = Y_AXIS_BRIDGE,
        .encoder = Y_AXIS_ENCODER,
        .requestedSetPoint = 0.0F,
        .controllerEnabled = false,
    },
};

/******************************************************************************/
/*                P U B L I C  G L O B A L  V A R I A B L E S                 */
/******************************************************************************/


/******************************************************************************/
/*                       P U B L I C  F U N C T I O N S                       */
/******************************************************************************/
void AppMotorControl_init(void)
{
    // Ensure controllers are cleared out
    for (AppMotorControl_Num i = 0U; i < NUM_APP_CONTROLLER; i++)
    {
        Lib_PI_init(&ControllerData[i].controller);
    }
}

void AppMotorControl_1kHz(void)
{
    // Ensure controllers are cleared out
    for (AppMotorControl_Num i = 0U; i < NUM_APP_CONTROLLER; i++)
    {
        if (ControllerData[i].controllerEnabled)
        {
            float_t currPos = AppMotor_getPosition_10kHz(ControllerData[i].encoder);
            float_t setPoint = ControllerData[i].requestedSetPoint;
            Lib_PI_updateController(&ControllerData[i].controller, setPoint, currPos);
            sendBridgeRequest(ControllerData[i].controller.out, ControllerData[i].bridge);
        }
        else
        {
            // Controller disabled -- No Bridge Commands Sent
        }
    }
}

void AppMotorControl_setKp(AppMotorControl_Num contorller, float_t kp)
{
    if (contorller < NUM_APP_CONTROLLER)
    {
        ControllerData[contorller].controller.Ki = kp;
    }
}

void AppMotorControl_requestSetPoint(AppMotorControl_Num contorller, float_t setPointRequest)
{
    if (contorller < NUM_APP_CONTROLLER)
    {
        ControllerData[contorller].requestedSetPoint = setPointRequest;
    }
}

void AppMotorControl_configureController(AppMotorControl_Num contorller, bool controllerEnabled)
{
    if (contorller < NUM_APP_CONTROLLER)
    {
        ControllerData[contorller].controllerEnabled = controllerEnabled;
    }
}

/******************************************************************************/
/*                      P R I V A T E  F U N C T I O N S                      */
/******************************************************************************/

static void sendBridgeRequest(float_t controllerDuty, AppBridge_Num bridge)
{
    if (controllerDuty > 0.0F)
    {
        AppBridge_setBridge(bridge, BRIDGE_FORWARD, controllerDuty);
    }
    else if (controllerDuty < 0.0F)
    {
        AppBridge_setBridge(bridge, BRIDGE_REVERSE, fabs(controllerDuty));
    }
    else  // Output duty is 0%
    {
        AppBridge_setBridge(bridge, BRIDGE_STOP, 0.0F);
    }
}
