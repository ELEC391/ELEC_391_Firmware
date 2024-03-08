/**
 * @file device_config.h
 * @brief Device Configuration Functions
 */

#ifndef DEVICE_CONFIG_H_
#define DEVICE_CONFIG_H_

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "device_config.h"

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
 * @brief Device initialization function
 *
 * configures and calls all init functions before entering main loop
 *
 */
void DeviceConfig_init(void);


#endif // DEVICE_CONFIG_H_
