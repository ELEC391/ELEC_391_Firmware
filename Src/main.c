/**
 * @file main.c
 * @brief Application main file
 */

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "main.h"
#include "device_timer.h"
#include "device_config.h"
#include "device_gpio.h"
#include "device_uart.h"
#include "app_motor.h"

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/


/******************************************************************************/
/*            P R I V A T E  F U N C T I O N  P R O T O T Y P E S             */
/******************************************************************************/

// Prototypes for functions only used in this file (always declare static)

/******************************************************************************/
/*                       P U B L I C  F U N C T I O N S                       */
/******************************************************************************/

void Error_Handler(void)
{
    __disable_irq();
    while(1)
    {
        HAL_Delay(100);
        DeviceGpio_toggle(RED_LED_PIN);
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


    // Test and Debug params
    // uint16_t countx = 0;
    // int64_t county = 0;

    float_t velocity = 0;
    float_t position = 0;
    float_t filtVel = 0;
    float_t filtPos = 0;

    // int time = 0;
    char aTxMessage[100];

    sprintf(aTxMessage, "Raw_Velocity,Filtered_Velocity,Raw_Postiion,Filtered_Position\r\n");
    DeviceUart_sendMessage(MAIN_LOGGING_CHANNEL, aTxMessage);

    velocity =  AppMotor_getVelocity_Raw(X_AXIS_ENCODER);
    position =   AppMotor_getPosition_Raw(X_AXIS_ENCODER);
    filtVel =  AppMotor_getVelocity_10kHz(X_AXIS_ENCODER);
    filtPos =   AppMotor_getPosition_10kHz(X_AXIS_ENCODER);
    sprintf(aTxMessage, "%d,%d,%d,%d\r\n", (int) velocity, (int) filtVel,(int) position, (int) filtPos);
    DeviceUart_sendMessage(MAIN_LOGGING_CHANNEL, aTxMessage);

    HAL_Delay(8000);

    // Main loop
    while (1)
    {
        velocity =  AppMotor_getVelocity_Raw(X_AXIS_ENCODER);
        position =   AppMotor_getPosition_Raw(X_AXIS_ENCODER);
        filtVel =  AppMotor_getVelocity_10kHz(X_AXIS_ENCODER);
        filtPos =   AppMotor_getPosition_10kHz(X_AXIS_ENCODER);
        sprintf(aTxMessage, "%d,%d,%d,%d\r\n", (int) velocity, (int) filtVel,(int) position, (int) filtPos);
        DeviceUart_sendMessage(MAIN_LOGGING_CHANNEL, aTxMessage);

        // countx = DeviceTimer_getEncoderCount(encoder);
        // county = AppMotor_getEncoderCount(encoder);
        // // sprintf(aTxMessage, "%d,%d\r\n", (int) county, (int) countx);
        // // DeviceUart_sendMessage(MAIN_LOGGING_CHANNEL, aTxMessage);
        // HAL_Delay(100);
        // DeviceGpio_enable(GREEN_LED_PIN);
        // HAL_Delay(100);
        // DeviceGpio_disable(GREEN_LED_PIN);

        DeviceGpio_enable(X_AXIS_MOTOR_ENABLE);

        DeviceGpio_enable(X_AXIS_MOTOR_IN1);
        DeviceGpio_disable(X_AXIS_MOTOR_IN2);
        HAL_Delay(2000);

        DeviceGpio_enable(X_AXIS_MOTOR_IN2);
        DeviceGpio_disable(X_AXIS_MOTOR_IN1);
        HAL_Delay(2000);
    }
}
