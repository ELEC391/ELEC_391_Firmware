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
#include "app_bridge.h"
#include "app_motor_control.h"

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

float_t saturate(float_t val, float_t max, float_t min)
{
    return fmin(fmax(val, min), max);
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

    velocity = AppMotor_getVelocity_Raw(Y_AXIS_ENCODER);
    position = AppMotor_getPosition_Raw(Y_AXIS_ENCODER);
    filtVel =  AppMotor_getVelocity_10kHz(Y_AXIS_ENCODER);
    filtPos =  AppMotor_getPosition_10kHz(Y_AXIS_ENCODER);
    sprintf(aTxMessage, "%d,%d,%d,%d\r\n", (int) velocity, (int) filtVel,(int) position, (int) filtPos);
    DeviceUart_sendMessage(MAIN_LOGGING_CHANNEL, aTxMessage);

    uint16_t count = 0;
    uint16_t count2 = 0U;
    bool dir = true;
    float_t setpoint = 0.0F;
    AppMotorControl_configureController(Y_AXIS_CONTROLLER, true);
    AppMotorControl_requestSetPoint(Y_AXIS_CONTROLLER, setpoint);

    // Main loop
    while (1)
    {
        velocity = AppMotor_getVelocity_Raw(Y_AXIS_ENCODER);
        position = AppMotor_getPosition_Raw(Y_AXIS_ENCODER);
        filtVel =  AppMotor_getVelocity_10kHz(Y_AXIS_ENCODER);
        filtPos =  AppMotor_getPosition_10kHz(Y_AXIS_ENCODER);
        sprintf(aTxMessage, "%d,%d,%d,%d\r\n", (int) velocity, (int) filtVel,(int) position, (int) filtPos);
        DeviceUart_sendMessage(MAIN_LOGGING_CHANNEL, aTxMessage);
        HAL_Delay(100);
        DeviceGpio_toggle(GREEN_LED_PIN);

        if ((count >= 10) && (count2 < 51))
        {
            dir = !dir;
            count = 0U;

            if (dir)
            {
                AppBridge_setBridge(Y_AXIS_BRIDGE, BRIDGE_FORWARD, 50.0F);
                setpoint = 0.0F;
            }
            else
            {
                AppBridge_setBridge(Y_AXIS_BRIDGE, BRIDGE_REVERSE, 50.0F);
                setpoint = 90.0F;
            }
        }
        else
        {
            AppBridge_setBridge(Y_AXIS_BRIDGE, BRIDGE_OFF, 50.0F);
        }
        count++;
        count2++;
        AppMotorControl_requestSetPoint(Y_AXIS_CONTROLLER, setpoint);

    }
}

