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
#define DEGREE_PER_COUNT 0.007353F //360 /  (CPR * GEARING * 4) Counts per Pulse for encoder mode
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
}App_MotorControlData;

typedef struct{
    App_MotorEncoder encoder;
    App_MotorControlData rawData;
    App_MotorControlData filteredData;
}App_MotorData;

/******************************************************************************/
/*            P R I V A T E  F U N C T I O N  P R O T O T Y P E S             */
/******************************************************************************/

static void updateEncoderPulseCount(DeviceEncoder_Num encoder);
static void updateMotorData(DeviceEncoder_Num encoder);

/******************************************************************************/
/*               P R I V A T E  G L O B A L  V A R I A B L E S                */
/******************************************************************************/

static volatile App_MotorData MotorData[NUM_DEVICE_ENCODERS] =
{
    // Initialize all data to 0
    [X_AXIS_ENCODER] =
    {
        .encoder = {0},
        .rawData = {0},
        .filteredData = {0}
    },
    [Y_AXIS_ENCODER] =
    {
        .encoder = {0},
        .rawData = {0},
        .filteredData = {0}
    }
};

/******************************************************************************/
/*                P U B L I C  G L O B A L  V A R I A B L E S                 */
/******************************************************************************/

/******************************************************************************/
/*                       P U B L I C  F U N C T I O N S                       */
/******************************************************************************/

void AppMotor_init(void)
{
    DeviceTimer_startEncoder(X_AXIS_ENCODER);
    DeviceTimer_startEncoder(Y_AXIS_ENCODER);

    updateEncoderPulseCount(X_AXIS_ENCODER);
    updateEncoderPulseCount(Y_AXIS_ENCODER);
}

void AppMotor_10kHz(void)
{
    // Update X-Axis Motor
    updateEncoderPulseCount(X_AXIS_ENCODER);
    updateMotorData(X_AXIS_ENCODER);

    // Update Y-Axis Motor
    updateEncoderPulseCount(Y_AXIS_ENCODER);
    updateMotorData(Y_AXIS_ENCODER);
}

float_t AppMotor_getVelocity_Raw(DeviceEncoder_Num encoder)
{
    float_t ret = 0;
    if (encoder < NUM_DEVICE_ENCODERS)
    {
        ret = MotorData[encoder].rawData.velocity;
    }

    return ret;
}

float_t AppMotor_getPosition_Raw(DeviceEncoder_Num encoder)
{
    float_t ret = 0;
    if (encoder < NUM_DEVICE_ENCODERS)
    {
        ret = MotorData[encoder].rawData.position;
    }
    return ret;
}

float_t AppMotor_getVelocity_10kHz(DeviceEncoder_Num encoder)
{
    float_t ret = 0;
    if (encoder < NUM_DEVICE_ENCODERS)
    {
        ret = MotorData[encoder].filteredData.velocity;
    }
    return ret;
}

float_t AppMotor_getPosition_10kHz(DeviceEncoder_Num encoder)
{
    float_t ret = 0;
    if (encoder < NUM_DEVICE_ENCODERS)
    {
        ret = MotorData[encoder].filteredData.position;
    }
    return ret;
}

int16_t AppMotor_getEncoderVelocity(DeviceEncoder_Num encoder)
{
    int16_t ret = 0;
    if (encoder < NUM_DEVICE_ENCODERS)
    {
        ret = MotorData[encoder].encoder.deltaCount;
    }
    return ret;
}

int64_t AppMotor_getEncoderCount(DeviceEncoder_Num encoder)
{
    int16_t ret = 0;
    if (encoder < NUM_DEVICE_ENCODERS)
    {
        ret = MotorData[encoder].encoder.count;
    }
    return ret;
}

/******************************************************************************/
/*                      P R I V A T E  F U N C T I O N S                      */
/******************************************************************************/

static void updateMotorData(DeviceEncoder_Num encoder)
{
    // Update raw data
    MotorData[encoder].rawData.positionPrev = MotorData[encoder].rawData.position;
    MotorData[encoder].rawData.position = MotorData[encoder].encoder.count * DEGREE_PER_COUNT; // output Degree
    MotorData[encoder].rawData.velocityPrev = MotorData[encoder].rawData.velocity;
    MotorData[encoder].rawData.velocity = MotorData[encoder].encoder.deltaCount * (DEGREE_PER_COUNT * SAMPLE_FREQ_HZ * SEC_PER_MIN) / (DEGREE_PER_ROTATION * 2); // output RPM TODO Figure out why 2x is required on H7

    // Update Filtered Data
    MotorData[encoder].filteredData.positionPrev = MotorData[encoder].filteredData.position;
    MotorData[encoder].filteredData.position = Lib_lpf(MotorData[encoder].rawData.position, MotorData[encoder].rawData.positionPrev, MotorData[encoder].filteredData.positionPrev, CUTOFF_HZ, SAMPLE_FREQ_HZ);
    MotorData[encoder].filteredData.velocityPrev = MotorData[encoder].filteredData.velocity;
    MotorData[encoder].filteredData.velocity = Lib_lpf(MotorData[encoder].rawData.velocity, MotorData[encoder].rawData.velocityPrev, MotorData[encoder].filteredData.velocityPrev, CUTOFF_HZ, SAMPLE_FREQ_HZ);
}

// From https://www.steppeschool.com/pages/blog/stm32-timer-encoder-mode
static volatile void updateEncoderPulseCount(DeviceEncoder_Num encoder)
{
    if (encoder < NUM_DEVICE_ENCODERS)
    {
        uint16_t temp_counter = DeviceTimer_getEncoderCount(encoder);

        if (temp_counter == MotorData[encoder].encoder.last_counter_value)
        {
            MotorData[encoder].encoder.deltaCount = 0;
        }
        else if (temp_counter > MotorData[encoder].encoder.last_counter_value)
        {
            if (DeviceTimer_isEncoderCountingDown(encoder))
            {
                MotorData[encoder].encoder.deltaCount = (- MotorData[encoder].encoder.last_counter_value - (DeviceTimer_getEncoderAutoReload(encoder) - temp_counter));
            }
            else
            {
                MotorData[encoder].encoder.deltaCount = temp_counter - MotorData[encoder].encoder.last_counter_value;
            }
        }
        else
        {
            if (DeviceTimer_isEncoderCountingDown(encoder))
            {
                MotorData[encoder].encoder.deltaCount = temp_counter - MotorData[encoder].encoder.last_counter_value;
            }
            else
            {
                MotorData[encoder].encoder.deltaCount = temp_counter + (DeviceTimer_getEncoderAutoReload(encoder) - MotorData[encoder].encoder.last_counter_value);
            }
        }

        // Update counts
        MotorData[encoder].encoder.count += MotorData[encoder].encoder.deltaCount;
        MotorData[encoder].encoder.last_counter_value = temp_counter;
    }
 }
