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

#define FREQ_TO_PERIOD(x) 1.0 / x

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

void Lib_PI_init(Lib_PI_Controller* Contorller)
{
	/* Clear controller variables */
    Contorller->integrator = 0.0f;
    Contorller->prevError  = 0.0f;
    Contorller->out = 0.0f;
}

float_t Lib_PI_updateController(Lib_PI_Controller* Contorller, float_t SetPoint, float_t Measurement)
{
    float_t error = SetPoint - Measurement;
    float_t proportional = Contorller->Kp * error;

    // Integrator
    Contorller->integrator = Contorller->integrator + 0.5f * Contorller->Ki * Contorller->T * (error + Contorller->prevError);
    // Anti-wind-up via integrator clamping
    if (Contorller->integrator > Contorller->limMaxInt) {

        Contorller->integrator = Contorller->limMaxInt;

    } else if (Contorller->integrator < Contorller->limMinInt) {

        Contorller->integrator = Contorller->limMinInt;
    }

    // Output clamping
    Contorller->out = proportional + Contorller->integrator;

    if (Contorller->out > Contorller->limMax) {

        Contorller->out = Contorller->limMax;

    } else if (Contorller->out < Contorller->limMin) {

        Contorller->out = Contorller->limMin;

    }

    // Update previous error for integrator
    Contorller->prevError = error;
    return Contorller->out;
}

/******************************************************************************/
/*                      P R I V A T E  F U N C T I O N S                      */
/******************************************************************************/

// Implementations for functions only used in this file
