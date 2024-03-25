/**
 * @file main.c
 * @brief Application main file
 */

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "main.h"
#include "app_motor.h"
#include "device_config.h"
#include "device_gpio.h"
#include "device_timer.h"
#include "device_uart.h"

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/
extern UART_HandleTypeDef huart3;
/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/

// #define DEGREE_PER_PULSE 0.014706

// // Filter Defines
// #define SAMPLE_FREQ_HZ 10000
// #define CUTOFF_HZ 50 // TODO MODEL
// #define SEC_PER_MIN 60

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

        HAL_Delay(100);
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

    // int16_t velocity = 0;
    // int64_t position = 0;

    // float_t velocity = 0;
    // float_t position = 0;
    // float_t filtVel = 0;
    // float_t filtPos = 0;
    // int time = 0;
    char aTxMessage[100];

    sprintf(aTxMessage, "Raw_Velocity,Filtered_Velocity,Raw_Postiion,Filtered_Position,timeStep\r\n");
    // if(HAL_UART_Transmit(&huart3, (uint8_t*)aTxMessage, strlen(aTxMessage), 1000)!= HAL_OK)
    // {
    // /* Transfer error in transmission process */
    // Error_Handler();
    // }
    // Main loop
    while (1)
    {
        // // count = DeviceTimer_getEncoderCount();
        // velocity =  AppMotor_getVelocity_Raw();
        // position =   AppMotor_getPosition_Raw();
        // filtVel =  AppMotor_getVelocity_10kHz();
        // filtPos =   AppMotor_getPosition_10kHz();
        // // velocity =  AppMotor_getEncoderVelocity();
        // // position =   AppMotor_getEncoderCount() * (0.01470F);
        // sprintf(aTxMessage, "%d,%d,%d,%d,%d\r\n", (int) velocity, (int) filtVel,(int) position, (int) filtPos, time);
        // if(HAL_UART_Transmit(&huart3, (uint8_t*)aTxMessage, strlen(aTxMessage), 1000)!= HAL_OK)
        // {
        // /* Transfer error in transmission process */
        //     Error_Handler();
        // }
        // HAL_Delay(500);
        // DeviceGpio_toggle(RED_LED_PIN);
        // DeviceGpio_toggle(GREEN_LED_PIN);
        // DeviceGpio_toggle(YELLOW_LED_PIN);
        HAL_Delay(1000);
        DeviceUart_sendMessage(MAIN_LOGGING_CHANNEL, aTxMessage);
    }
}
