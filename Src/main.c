/**
 * @file main.c
 * @brief Application main file
 */

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "main.h"
#include "device_timer.h"
#include "device_irq.h"
#include "device_config.h"
#include "device_gpio.h"
#include "device_uart.h"
#include "app_motor.h"
#include "app_bridge.h"
#include "app_motor_control.h"

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

#define MAX_KP 16.0F
#define KP_INCREMENT 0.5F

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
    float_t velocity = 0;
    float_t position = 0;
    float_t filtVel = 0;
    float_t filtPos = 0;
    uint32_t timer = 0U;

    uint16_t setPointCount = 0U;
    uint16_t updatePiParamCount = 0U;
    float_t setpoint = 0.0F;
    float_t kp = 0.5F;
    bool doneMsg = false;

    // int time = 0;
    char aTxMessage[100];

    sprintf(aTxMessage, "Raw_Velocity,Filtered_Velocity,Raw_Postiion,Filtered_Position,Setpoint,timerCount\r\n");
    DeviceUart_sendMessage(MAIN_LOGGING_CHANNEL, aTxMessage);

    // AppMotorControl_setKp(X_AXIS_CONTROLLER, kp);
    AppMotorControl_requestSetPoint(X_AXIS_CONTROLLER, setpoint);
    AppMotorControl_configureController(X_AXIS_CONTROLLER, true);

    // Main loop
    while (1)
    {
        if (kp > MAX_KP)
        {
            if (!doneMsg)
            {
                sprintf(aTxMessage, "********************************** DONE **********************************\r\n");
                DeviceUart_sendMessage(MAIN_LOGGING_CHANNEL, aTxMessage);
                doneMsg = true;
            }

            HAL_Delay(150); // Ensure motor stops moving
            DeviceGpio_enable(GREEN_LED_PIN);
        }
        else if (updatePiParamCount < 600U) // Run each test for 3sec
        {
            // Wait 250ms for step
            if (setPointCount == 50U)
            {
                setpoint = 90.0F;
                AppMotorControl_requestSetPoint(X_AXIS_CONTROLLER, setpoint);
            }
            timer = DeviceIrq_getCount_ms();
            velocity = AppMotor_getVelocity_Raw(X_AXIS_ENCODER);
            position = AppMotor_getPosition_Raw(X_AXIS_ENCODER);
            filtVel =  AppMotor_getVelocity_10kHz(X_AXIS_ENCODER);
            filtPos =  AppMotor_getPosition_10kHz(X_AXIS_ENCODER);
            sprintf(aTxMessage, "%d,%d,%d,%d,%d,%d\r\n", (int) velocity, (int) filtVel,(int) position, (int) filtPos,(int)setpoint,(int)timer);
            DeviceUart_sendMessage(MAIN_LOGGING_CHANNEL, aTxMessage);
            HAL_Delay(5);
            setPointCount++;
            updatePiParamCount++;
        }
        else // Update for next sweep
        {
            AppMotorControl_configureController(X_AXIS_CONTROLLER, false);
            // Reset controller and data
            AppMotor_reinitializeData();
            AppMotorControl_init(); // zeros out controller variables
            updatePiParamCount = 0U;
            setPointCount = 0U;
            setpoint = 0.0F;
            kp = kp + KP_INCREMENT;
            HAL_Delay(175); // Ensure motor stops moving

            // Update UART FOR NEW LOG
            sprintf(aTxMessage, "********************************** NEW KP SET **********************************\r\n");
            DeviceUart_sendMessage(MAIN_LOGGING_CHANNEL, aTxMessage);
            int kp_int = (int)kp;
            float_t kp_times_two = kp * 2.0F;
            int rem = ((int) kp_times_two) - 2 * kp_int;
            int dec = 0;
            if (rem == 1)
            {
                dec = 5;
            }
            sprintf(aTxMessage, "KI = %d . %d\r\n", (int)kp, dec);
            DeviceUart_sendMessage(MAIN_LOGGING_CHANNEL, aTxMessage);
            sprintf(aTxMessage, "Raw_Velocity,Filtered_Velocity,Raw_Postiion,Filtered_Position,Setpoint,timerCount\r\n");
            DeviceUart_sendMessage(MAIN_LOGGING_CHANNEL, aTxMessage);

            DeviceIrq_clearCounter();
            // AppMotorControl_setKp(X_AXIS_CONTROLLER, kp);
            AppMotorControl_requestSetPoint(X_AXIS_CONTROLLER, setpoint);
            AppMotorControl_configureController(X_AXIS_CONTROLLER, true);
        }
    }
}

