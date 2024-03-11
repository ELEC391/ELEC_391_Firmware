/**
 * @file lib_lpf.h
 * @brief Library containing first order discrete time low pass filter
 */

#ifndef LIB_LPF_H_
#define LIB_LPF_H_

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "lib_lpf.h"

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

// Public defines that may be used by other files

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

/**
 * @brief Discrete first order low-pass filter
 *
 * More details about the function.
 *
 * @param[in]  x newly sampled signal value
 * @param[in]  xPrev previously sampled signal value
 * @param[in]  yPrev previously outputted filtered value
 * @param[in]  cutoff_Hz lowpass filter cutoff frequency in hertz
 * @param[in]  samplingFreq_Hz signal sampling rate i.e. 1khz sampling
 *
 * @param[out] y returns newly filtered output
 */
float_t Lib_lpf(float_t x, float_t xPrev, float_t yPrev, float_t cutoff_Hz, float_t samplingFreq_Hz);

/******************************************************************************/
/*                       I N L I N E  F U N C T I O N S                       */
/******************************************************************************/

// Inline function declarations and implementations

#endif // LIB_LPF_H_
