/**
 * @file lib_pi_compensator.h
 * @brief Implementation of PI controller
 */

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include  "lib_pi_compensator.h"

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/

// Typedefs that are only used in this file

/******************************************************************************/
/*            P R I V A T E  F U N C T I O N  P R O T O T Y P E S             */
/******************************************************************************/

// Prototypes for functions only used in this file (always declare static)

/******************************************************************************/
/*               P R I V A T E  G L O B A L  V A R I A B L E S                */
/******************************************************************************/

// Global variables that are only used in this file (always declare static)

/******************************************************************************/
/*                P U B L I C  G L O B A L  V A R I A B L E S                 */
/******************************************************************************/

// Global variable definitions matching the extern definitions in the header

/******************************************************************************/
/*                       P U B L I C  F U N C T I O N S                       */
/******************************************************************************/

void Lib_PI_init(Lib_PI_Controller* controller)
{
	/* Clear controller variables */
    controller->integrator = 0.0f;
    controller->prevError  = 0.0f;
    controller->out = 0.0f;
}

float_t Lib_PI_updateController(Lib_PI_Controller* controller, float_t setPoint, float_t measurement)
{
    float_t error = setPoint - measurement;
    float_t proportional = controller->Kp * error;

    // Integrator
    controller->integrator = controller->integrator + 0.5f * controller->Ki * controller->T * (error + controller->prevError);
    // Anti-wind-up via integrator clamping
    if (controller->integrator > controller->limMaxInt) {

        controller->integrator = controller->limMaxInt;

    } else if (controller->integrator < controller->limMinInt) {

        controller->integrator = controller->limMinInt;
    }

    // Output clamping
    controller->out = proportional + controller->integrator;

    if (controller->out > controller->limMax) {

        controller->out = controller->limMax;

    } else if (controller->out < controller->limMin) {

        controller->out = controller->limMin;

    }

    // Update previous error for integrator
    controller->prevError = error;
    return controller->out;
}

/******************************************************************************/
/*                      P R I V A T E  F U N C T I O N S                      */
/******************************************************************************/

// Implementations for functions only used in this file
