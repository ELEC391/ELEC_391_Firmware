/**
 * @file main.c
 * @brief Application main file
 */

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "main.h"
#include "device_config.h"
#include "device_gpio.h"
#include "device_timer.h"

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/
extern UART_HandleTypeDef huart3;
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
    DeviceConfig_init();

    uint16_t count = 0;
    char aTxMessage[100];

    // Main loop
    while (1)
    {
        count = DeviceTimer_getEncoderCount();
        sprintf(aTxMessage, "count is  %u \r\n", (unsigned int) count);
        if(HAL_UART_Transmit(&huart3, (uint8_t*)aTxMessage, strlen(aTxMessage), 1000)!= HAL_OK)
        {
        /* Transfer error in transmission process */
        Error_Handler();
        }
        HAL_Delay(500);
        __asm__("nop");
    }
}
