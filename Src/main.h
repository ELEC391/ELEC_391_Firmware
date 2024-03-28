/**
 * @file main.h
 * @brief main header file -- only used as entry point for error handler
 */

#ifndef MAIN_H_
#define MAIN_H_

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "stm32h7xx_hal.h"


/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/

/******************************************************************************/
/*                       G L O B A L  V A R I A B L E S                       */
/******************************************************************************/

// Public global variables that may be used by other files
// (always declare extern)

/******************************************************************************/
/*                             F U N C T I O N S                              */
/******************************************************************************/

void Error_Handler(void);
float_t saturate(float_t val, float_t max, float_t min);

#endif // MAIN_H_
