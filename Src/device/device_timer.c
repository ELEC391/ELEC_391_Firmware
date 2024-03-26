/**
 * @file device_timer.c
 * @brief configures and interfaces with timers
 */

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "main.h"
#include "device_timer.h"

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

#define CLOCK_SPEED_MHZ 550

/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/
typedef struct TimerConfig
{
    bool configureIrq;
    bool configurePwm;
    TIM_HandleTypeDef timConfig;
    TIM_ClockConfigTypeDef clockConfig;
    TIM_MasterConfigTypeDef masterConfig;
    TIM_OC_InitTypeDef pwmConfig;
}TimerConfig;

typedef struct EncoderConfig
{
    TIM_HandleTypeDef timConfig;
    TIM_Encoder_InitTypeDef encConfig;
    TIM_MasterConfigTypeDef masterConfig;
}EncoderConfig;

/******************************************************************************/
/*            P R I V A T E  F U N C T I O N  P R O T O T Y P E S             */
/******************************************************************************/

/******************************************************************************/
/*               P R I V A T E  G L O B A L  V A R I A B L E S                */
/******************************************************************************/

static TimerConfig TimerConfigs[NUM_DEVICE_TIMERS] =
{
    [MAIN_CONTROL_TIMER] =
    {
        .configureIrq = true,
        .configurePwm = false,
        .timConfig =
        {
            .Instance = TIM2,
            .Init.Prescaler = 999, // CLK / ((PRE +1)(Period + 1)) = 1/Timer Freq  => 1Khz
            .Init.CounterMode = TIM_COUNTERMODE_UP,
            .Init.Period = CLOCK_SPEED_MHZ - 1,
            .Init.ClockDivision = TIM_CLOCKDIVISION_DIV1,
            .Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE,
        },
        .clockConfig =
        {
            .ClockSource = TIM_CLOCKSOURCE_INTERNAL,
        },
        .masterConfig =
        {
            .MasterOutputTrigger = TIM_TRGO_RESET,
            .MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE
        },
        .pwmConfig = {} // Init to 0

    },
    [SIGNAL_FILTER_TIMER] =
    {
        .configureIrq = true,
        .configurePwm = true,
        .timConfig =
        {
            .Instance = TIM3,
            .Init.Prescaler = 99, // CLK / ((PRE +1)(Period + 1)) = 1/Timer Freq  => 10Khz
            .Init.CounterMode = TIM_COUNTERMODE_UP,
            .Init.Period = CLOCK_SPEED_MHZ - 1,
            .Init.ClockDivision = TIM_CLOCKDIVISION_DIV1,
            .Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE,
        },
        .clockConfig =
        {
            .ClockSource = TIM_CLOCKSOURCE_INTERNAL,
        },
        .masterConfig =
        {
            .MasterOutputTrigger = TIM_TRGO_RESET,
            .MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE
        },
        .pwmConfig = {}
    },
};

static EncoderConfig EncoderConfigs[NUM_DEVICE_ENCODERS] =
{
    [X_AXIS_ENCODER] =
    {
        .timConfig =
        {
            .Instance = TIM23,
            .Init.Prescaler = 0, // Not required
            .Init.CounterMode = TIM_COUNTERMODE_UP,
            .Init.Period = 4294967295, // Default Max Value
            .Init.ClockDivision = TIM_CLOCKDIVISION_DIV1,
            .Init.RepetitionCounter = 0,
            .Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE,
        },
        .encConfig =
        {
            .EncoderMode = TIM_ENCODERMODE_TI12,
            .IC1Polarity = TIM_ICPOLARITY_RISING,
            .IC1Selection = TIM_ICSELECTION_DIRECTTI,
            .IC1Prescaler = TIM_ICPSC_DIV1,
            .IC1Filter = 5, // Require 5 clock cycles before counting -- increase if jitter is an issue
            .IC2Polarity = TIM_ICPOLARITY_RISING,
            .IC2Selection = TIM_ICSELECTION_DIRECTTI,
            .IC2Prescaler = TIM_ICPSC_DIV1,
            .IC2Filter = 5, // Require 5 clock cycles before counting -- increase if jitter is an issue
        },
        .masterConfig =
        {
            .MasterOutputTrigger = TIM_TRGO_RESET,
            .MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE
        }
    },
    [Y_AXIS_ENCODER] =
    {
        .timConfig =
        {
            .Instance = TIM24,
            .Init.Prescaler = 0, // Not required
            .Init.CounterMode = TIM_COUNTERMODE_UP,
            .Init.Period = 4294967295, // Default Max Value
            .Init.ClockDivision = TIM_CLOCKDIVISION_DIV1,
            .Init.RepetitionCounter = 0,
            .Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE,
        },
        .encConfig =
        {
            .EncoderMode = TIM_ENCODERMODE_TI12,
            .IC1Polarity = TIM_ICPOLARITY_RISING,
            .IC1Selection = TIM_ICSELECTION_DIRECTTI,
            .IC1Prescaler = TIM_ICPSC_DIV1,
            .IC1Filter = 5, // Require 5 clock cycles before counting -- increase if jitter is an issue
            .IC2Polarity = TIM_ICPOLARITY_RISING,
            .IC2Selection = TIM_ICSELECTION_DIRECTTI,
            .IC2Prescaler = TIM_ICPSC_DIV1,
            .IC2Filter = 5, // Require 5 clock cycles before counting -- increase if jitter is an issue
        },
        .masterConfig =
        {
            .MasterOutputTrigger = TIM_TRGO_RESET,
            .MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE
        }
    }
};

/******************************************************************************/
/*                       P U B L I C  F U N C T I O N S                       */
/******************************************************************************/

void DeviceTimer_init(void)
{
    HAL_StatusTypeDef configRet = HAL_OK;

    // Configure timers
    for(DeviceTimer_Num i = (DeviceTimer_Num) 0U; i < NUM_DEVICE_TIMERS; i++)
    {
        // Try to configure each HAL_OK == 0x00 Any other fail is a fail condition
        configRet |= HAL_TIM_Base_Init(&TimerConfigs[i].timConfig);
        configRet |= HAL_TIM_ConfigClockSource(&TimerConfigs[i].timConfig, &TimerConfigs[i].clockConfig);
        configRet |= HAL_TIMEx_MasterConfigSynchronization(&TimerConfigs[i].timConfig, &TimerConfigs[i].masterConfig);

        // Check if PWM if configured for timer
        if(TimerConfigs[i].configurePwm)
        {
            configRet |= HAL_TIM_PWM_ConfigChannel(&TimerConfigs[i].timConfig, &TimerConfigs[i].pwmConfig, TIM_CHANNEL_1); // TODO -- make output channel configurable
        }

        if (configRet != HAL_OK)
        {
            Error_Handler();
        }
    }

    // Configure encoders
    for(DeviceEncoder_Num i = (DeviceEncoder_Num) 0U; i < NUM_DEVICE_ENCODERS; i++)
    {
        // Try to configure each HAL_OK == 0x00 Any other fail is a fail condition
        configRet |= HAL_TIM_Encoder_Init(&EncoderConfigs[i].timConfig, &EncoderConfigs[i].encConfig);
        configRet |= HAL_TIMEx_MasterConfigSynchronization(&EncoderConfigs[i].timConfig, &EncoderConfigs[i].masterConfig);

        if (configRet != HAL_OK)
        {
            Error_Handler();
        }
    }
}

void DeviceTimer_startIrq(DeviceTimer_Num timer)
{
    if(timer != NUM_DEVICE_TIMERS)
    {
        if (TimerConfigs[timer].configureIrq)
        {
            HAL_TIM_Base_Start_IT(&TimerConfigs[timer].timConfig);
        }
    }
}

void DeviceTimer_handleIrq(DeviceTimer_Num timer)
{
    if (timer != NUM_DEVICE_TIMERS)
    {
        HAL_TIM_IRQHandler(&TimerConfigs[timer].timConfig);

    }
}

void DeviceTimer_startEncoder(DeviceEncoder_Num encoder)
{
    if (encoder != NUM_DEVICE_ENCODERS)
    {
        HAL_TIM_Encoder_Start(&EncoderConfigs[encoder].timConfig, TIM_CHANNEL_ALL);
    }
}

volatile uint16_t DeviceTimer_getEncoderCount(DeviceEncoder_Num encoder)
{
    uint16_t count = 0U; // TODO: configure better error handling
    if (encoder != NUM_DEVICE_ENCODERS)
    {
        count  = (uint16_t) __HAL_TIM_GET_COUNTER(&EncoderConfigs[encoder].timConfig);
    }

    return count;
}

volatile bool DeviceTimer_isEncoderCountingDown(DeviceEncoder_Num encoder)
{
    bool isCountingDown = false; // TODO: configure better error handling
    if (encoder != NUM_DEVICE_ENCODERS)
    {
        isCountingDown = (bool) __HAL_TIM_IS_TIM_COUNTING_DOWN(&EncoderConfigs[encoder].timConfig);
    }

    return isCountingDown;
}

volatile uint32_t DeviceTimer_getEncoderAutoReload(DeviceEncoder_Num encoder)
{
    uint32_t arrCount = 0U; // TODO: configure better error handling
    if (encoder != NUM_DEVICE_ENCODERS)
    {
        arrCount = (uint32_t) __HAL_TIM_GET_AUTORELOAD(&EncoderConfigs[encoder].timConfig);
    }

    return arrCount;
}

/******************************************************************************/
/*                      P R I V A T E  F U N C T I O N S                      */
/******************************************************************************/


/******************************************************************************/
/*                       H A L  M S P  F U N C T I O N S                      */
/******************************************************************************/

/*
 * List of all function callback required by HAL API for timer functions
 * For new feature bring up cubeMX is used to quickly bring up required
 * functions which are tested and then bought over to our main repo
*/

void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* tim_encoderHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(tim_encoderHandle->Instance==TIM23)
    {
        /* TIM23 clock enable */
        __HAL_RCC_TIM23_CLK_ENABLE();

        __HAL_RCC_GPIOF_CLK_ENABLE();
        /**TIM23 GPIO Configuration
        PF0     ------> TIM23_CH1
        PF1     ------> TIM23_CH2
        */
        GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF13_TIM23;
        HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
    }
    else if(tim_encoderHandle->Instance==TIM24)
    {
        /* TIM24 clock enable */
        __HAL_RCC_TIM24_CLK_ENABLE();

        __HAL_RCC_GPIOF_CLK_ENABLE();
        /**TIM24 GPIO Configuration
        PF11     ------> TIM24_CH1
        PF12     ------> TIM24_CH2
        */
        GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF14_TIM24;
        HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
    }
}

void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef* tim_encoderHandle)
{
    if(tim_encoderHandle->Instance==TIM23)
    {
        /* Peripheral clock disable */
        __HAL_RCC_TIM23_CLK_DISABLE();

        /**TIM23 GPIO Configuration
        PF0     ------> TIM23_CH1
        PF1     ------> TIM23_CH2
        */
        HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0|GPIO_PIN_1);
    }
    else if(tim_encoderHandle->Instance==TIM24)
    {
        /* Peripheral clock disable */
        __HAL_RCC_TIM24_CLK_DISABLE();

        /**TIM24 GPIO Configuration
        PF11     ------> TIM24_CH1
        PF12     ------> TIM24_CH2
        */
        HAL_GPIO_DeInit(GPIOF, GPIO_PIN_11|GPIO_PIN_12);
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

    if (tim_baseHandle->Instance==TIM2)
    {
        __HAL_RCC_TIM2_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM2_IRQn, 0, 1); // One sub priority below Signal filter
        HAL_NVIC_EnableIRQ(TIM2_IRQn);
    }
    else if (tim_baseHandle->Instance==TIM3)
    {
        __HAL_RCC_TIM3_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0); // Highest priority
        HAL_NVIC_EnableIRQ(TIM3_IRQn);
    }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

    if (tim_baseHandle->Instance==TIM2)
    {
        __HAL_RCC_TIM2_CLK_DISABLE();
        HAL_NVIC_DisableIRQ(TIM2_IRQn);
    }
    else if (tim_baseHandle->Instance==TIM3)
    {
        __HAL_RCC_TIM3_CLK_DISABLE();
        HAL_NVIC_DisableIRQ(TIM3_IRQn);
    }
}
