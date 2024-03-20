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
#include "lib_lpf.h"

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

// TODO figure out weird floating point macro bug when writing as DEGREE_PER_ROTATION / PULSE_PER_ROTATION
#define DEGREE_PER_COUNT 0.014706F //360 /  CPR * GEARING * 2 Counts per Pulse for encoder mode
#define DEGREE_PER_ROTATION 360

// Filter Defines
#define SAMPLE_FREQ_HZ 10000
#define CUTOFF_HZ 150
#define SEC_PER_MIN 60



/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/

typedef struct{
	int16_t deltaCount;
	int64_t count;
	uint16_t last_counter_value;
}App_MotorEncoder;

typedef struct{
	float_t velocity;
    float_t velocityPrev;
	float_t position;
    float_t positionPrev;
}App_MotorData;
/******************************************************************************/
/*            P R I V A T E  F U N C T I O N  P R O T O T Y P E S             */
/******************************************************************************/

static void updateEncoderPulseCount(void);

/******************************************************************************/
/*               P R I V A T E  G L O B A L  V A R I A B L E S                */
/******************************************************************************/

static volatile App_MotorEncoder Encoder = {};
static volatile App_MotorData RawMotorData = {};
static volatile App_MotorData FilteredMotorData = {};

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
    updateEncoderPulseCount();
}

void AppMotor_10kHz(void)
{
    // Update Encoder Pulse Count
    updateEncoderPulseCount();

    // Update raw data
    RawMotorData.positionPrev = RawMotorData.position;
    RawMotorData.position = Encoder.count * DEGREE_PER_COUNT; // ouput Degree
    RawMotorData.velocityPrev = RawMotorData.velocity;
    RawMotorData.velocity = Encoder.deltaCount * (DEGREE_PER_COUNT * SAMPLE_FREQ_HZ * SEC_PER_MIN) / (DEGREE_PER_ROTATION); // output RPM
    // Update filtered data
    FilteredMotorData.positionPrev = FilteredMotorData.position;
    FilteredMotorData.position = Lib_lpf(RawMotorData.position, RawMotorData.positionPrev, FilteredMotorData.positionPrev, CUTOFF_HZ, SAMPLE_FREQ_HZ);
    FilteredMotorData.velocityPrev = FilteredMotorData.velocity;
    FilteredMotorData.velocity = Lib_lpf(RawMotorData.velocity, RawMotorData.velocityPrev, FilteredMotorData.velocityPrev, CUTOFF_HZ, SAMPLE_FREQ_HZ);
}

float_t AppMotor_getVelocity_Raw(void)
{
    float_t ret = RawMotorData.velocity;
    return ret;
}

float_t AppMotor_getPosition_Raw(void)
{
    float_t ret = RawMotorData.position;
    return ret;
}

float_t AppMotor_getVelocity_10kHz(void)
{
    float_t ret = FilteredMotorData.velocity;
    return ret;
}

float_t AppMotor_getPosition_10kHz(void)
{
    float_t ret = FilteredMotorData.position;
    return ret;
}

int16_t AppMotor_getEncoderVelocity(void)
{
    int16_t ret = Encoder.deltaCount;
    return ret;
}

int64_t AppMotor_getEncoderCount(void)
{
    int64_t ret = Encoder.count;
    return ret;
}

/******************************************************************************/
/*                      P R I V A T E  F U N C T I O N S                      */
/******************************************************************************/

// From https://www.steppeschool.com/pages/blog/stm32-timer-encoder-mode
static void updateEncoderPulseCount(void)
 {
uint16_t temp_counter = __HAL_TIM_GET_COUNTER(&htim1);
static uint8_t first_time = 0;
if(!first_time)
{
   Encoder.deltaCount = 0;
   first_time = 1;
}
else
{
  if(temp_counter == Encoder.last_counter_value)
  {
    Encoder.deltaCount = 0;
  }
  else if(temp_counter > Encoder.last_counter_value)
  {
    if (__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim1))
    {
      Encoder.deltaCount = -Encoder.last_counter_value -
	(__HAL_TIM_GET_AUTORELOAD(&htim1)-temp_counter);
    }
    else
    {
      Encoder.deltaCount = temp_counter -
           Encoder.last_counter_value;
    }
  }
  else
  {
    if (__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim1))
    {
	Encoder.deltaCount = temp_counter -
            Encoder.last_counter_value;
    }
    else
    {
	Encoder.deltaCount = temp_counter +
	(__HAL_TIM_GET_AUTORELOAD(&htim1) -
              Encoder.last_counter_value);
    }
   }
}
Encoder.count += Encoder.deltaCount;
Encoder.last_counter_value = temp_counter;
 }
