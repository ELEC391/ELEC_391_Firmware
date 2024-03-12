/**
 * @file lib_pi_compensator.h
 * @brief Implementation of PI controller
 */

#ifndef LIB_PI_COMPENSATOR_H_
#define LIB_PI_COMPENSATOR_H_

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "lib_pi_compensator.h"

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

// Public defines that may be used by other files

/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/

typedef struct
{
	// Controller gains
	float_t Kp;
	float_t Ki;

	// Output limits
	float_t limMin;
	float_t limMax;

	// Integrator limits -- anti-windup
	float_t limMinInt;
	float_t limMaxInt;

	// Contorller Freq
	float_t controllerFreq;

	// Controller memory
	float_t integrator;
	float_t prevError; // Required for integrator

	//Controller output
	float_t out;

} Lib_PI_Controller;


/******************************************************************************/
/*                       G L O B A L  V A R I A B L E S                       */
/******************************************************************************/

// Public global variables that may be used by other files
// (always declare extern)

/******************************************************************************/
/*                             F U N C T I O N S                              */
/******************************************************************************/

void Lib_PI_init(Lib_PI_Controller* Contorller);
float_t Lib_PI_updateController(Lib_PI_Controller* Contorller);

/******************************************************************************/
/*                       I N L I N E  F U N C T I O N S                       */
/******************************************************************************/

// Inline function declarations and implementations

#endif // LIB_PI_COMPENSATOR_H_
