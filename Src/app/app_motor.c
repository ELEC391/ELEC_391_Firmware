/**
 * @file app_motor.c
 * @brief Motor app
 */

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "main.h"
#include "app_motor.h"
#include "device_timer.h"

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
/*               P R I V A T E  G L O B A L  V A R I A B L E S                */
/******************************************************************************/

static App_MotorEncoder Encoder = {};

/******************************************************************************/
/*                P U B L I C  G L O B A L  V A R I A B L E S                 */
/******************************************************************************/

extern TIM_HandleTypeDef htim1;

/******************************************************************************/
/*                       P U B L I C  F U N C T I O N S                       */
/******************************************************************************/

void AppMotor_init(void)
{
    DeviceTimer_startEncoder();
    AppMotor_updateEncoder();
}

int16_t AppMotor_getVelocity(void)
{
    int16_t ret = Encoder.velocity;
    return ret;
}

int64_t AppMotor_getPostion(void)
{
    int64_t ret = Encoder.position;
    return ret;
}

// Stolen from https://www.steppeschool.com/pages/blog/stm32-timer-encoder-mode
// I might have a stroke seeing how this copy pasta is formatted
void AppMotor_updateEncoder(void)
 {
uint16_t temp_counter = __HAL_TIM_GET_COUNTER(&htim1);
static uint8_t first_time = 0;
if(!first_time)
{
   Encoder.velocity = 0;
   first_time = 1;
}
else
{
  if(temp_counter == Encoder.last_counter_value)
  {
    Encoder.velocity = 0;
  }
  else if(temp_counter > Encoder.last_counter_value)
  {
    if (__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim1))
    {
      Encoder.velocity = -Encoder.last_counter_value -
	(__HAL_TIM_GET_AUTORELOAD(&htim1)-temp_counter);
    }
    else
    {
      Encoder.velocity = temp_counter -
           Encoder.last_counter_value;
    }
  }
  else
  {
    if (__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim1))
    {
	Encoder.velocity = temp_counter -
            Encoder.last_counter_value;
    }
    else
    {
	Encoder.velocity = temp_counter +
	(__HAL_TIM_GET_AUTORELOAD(&htim1) -
              Encoder.last_counter_value);
    }
   }
}
Encoder.position += Encoder.velocity;
Encoder.last_counter_value = temp_counter;
 }

/******************************************************************************/
/*                      P R I V A T E  F U N C T I O N S                      */
/******************************************************************************/

// Implementations for functions only used in this file
