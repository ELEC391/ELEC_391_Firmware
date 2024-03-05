/**
 * @file main.c
 * @brief Application main file
 */

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "main.h"
# include "device_config.h"

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

// Defines that are only used in this file

/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/

// Typedefs that are only used in this file

/******************************************************************************/
/*            P R I V A T E  F U N C T I O N  P R O T O T Y P E S             */
/******************************************************************************/

// Prototypes for functions only used in this file (always declare static)

/******************************************************************************/
/*                       P U B L I C  F U N C T I O N S                       */
/******************************************************************************/

void Error_Handler(void)
{
    while(1)
    {
        // TODO
    }
}

/******************************************************************************/
/*                      P R I V A T E  F U N C T I O N S                      */
/******************************************************************************/
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    // Configure hardware
    device_config_init();

    // Main loop
    while (1)
    {
        __asm__("nop");
    }
}