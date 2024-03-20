/**
 * @file <filename>
 * @brief <description>
 */

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "math.h"

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

#define FREQ_TO_PERIOD(x) 1.0F / x
#define FREQ_TO_RAD_PER_S(x) 2 * M_PI * x

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

// From https://github.com/botprof/first-order-low-pass-filter
float_t Lib_lpf(float_t x, float_t xPrev, float_t yPrev, float_t cutoff_Hz, float_t samplingFreq_Hz)
{
    float_t omega_c = FREQ_TO_RAD_PER_S(cutoff_Hz);
    float_t T = FREQ_TO_PERIOD(samplingFreq_Hz);
    float_t alpha = (2.0F - T * omega_c) / (2.0F + T * omega_c);
    float_t beta = T * omega_c / (2.0F + T * omega_c);

    return (alpha * yPrev) + (beta * (x + xPrev));
}

/******************************************************************************/
/*                      P R I V A T E  F U N C T I O N S                      */
/******************************************************************************/

// Implementations for functions only used in this file
