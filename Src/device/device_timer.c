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

TIM_HandleTypeDef htim1;

/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/

typedef struct TimerConfig
{
    uint8_t configureIrq;
    uint8_t configurePwm;
    TIM_HandleTypeDef timConfig;
    TIM_ClockConfigTypeDef clockConfig;
    TIM_MasterConfigTypeDef masterConfig;
    TIM_OC_InitTypeDef pwmConfig;
}TimerConfig;

/******************************************************************************/
/*            P R I V A T E  F U N C T I O N  P R O T O T Y P E S             */
/******************************************************************************/

static void MX_TIM1_Init(void);

/******************************************************************************/
/*               P R I V A T E  G L O B A L  V A R I A B L E S                */
/******************************************************************************/

static TimerConfig TimerConfigs[NUM_DEVICE_TIMERS] =
{
    [MAIN_CONTROL_TIMER] =
    {
        .configureIrq = TRUE,
        .configurePwm = FALSE,
        .timConfig =
        {
            .Instance = TIM2,
            .Init.Prescaler = 99, // CLK / ((PRE +1)(Period + 1)) = 1/Timer Freq  => 1Khz
            .Init.CounterMode = TIM_COUNTERMODE_UP,
            .Init.Period = 2499,
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
        .configureIrq = TRUE,
        .configurePwm = TRUE,
        .timConfig =
        {
            .Instance = TIM3,
            .Init.Prescaler = 9, // CLK / ((PRE +1)(Period + 1)) = 1/Timer Freq  => 10Khz
            .Init.CounterMode = TIM_COUNTERMODE_UP,
            .Init.Period = 2499,
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
        .pwmConfig =
        {
            .OCMode = TIM_OCMODE_PWM1;
            .Pulse = 0;
            .OCPolarity = TIM_OCPOLARITY_HIGH;
            .OCFastMode = TIM_OCFAST_DISABLE;
        }

    },
};

/******************************************************************************/
/*                       P U B L I C  F U N C T I O N S                       */
/******************************************************************************/

void DeviceTimer_init(void)
{
    for(DeviceTimer_Num i = (DeviceTimer_Num) 1U; i < NUM_DEVICE_TIMERS; i++)
    {
        HAL_StatusTypeDef configRet = HAL_OK;
        // Try to configure each HAL_OK == 0x00 Any other fail is a fail condition
        configRet |= HAL_TIM_Base_Init(&TimerConfigs[i].timConfig);
        configRet |= HAL_TIM_ConfigClockSource(&TimerConfigs[i].timConfig, &TimerConfigs[i].clockConfig);
        configRet |= HAL_TIMEx_MasterConfigSynchronization(&TimerConfigs[i].timConfig, &TimerConfigs[i].masterConfig);

        // Check if PWM if configured for timer
        if(TimerConfigs[i].configurePwm == TRUE)
        {
            configRet |= HAL_TIM_PWM_ConfigChannel(&TimerConfigs[i].timConfig, &TimerConfigs[i].pwmConfig, TIM_CHANNEL_1) // TODO -- make output channel configurable
        }

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
        if (TimerConfigs[timer].configureIrq == TRUE)
        {
            HAL_TIM_Base_Start_IT(&TimerConfigs[timer].timConfig);
        }
    }
}

void DeviceTimer_startEncoder(void)
{
    // Encoder timer init
    MX_TIM1_Init();
    // Start encoder
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
}

void DeviceTimer_handleIrq(DeviceTimer_Num timer)
{
    if(timer != NUM_DEVICE_TIMERS)
    {
        HAL_TIM_IRQHandler(&TimerConfigs[timer].timConfig);

    }
}

uint16_t DeviceTimer_getEncoderCount(void)
{
    return (uint16_t) __HAL_TIM_GET_COUNTER(&htim1);
}

// AutoGen Function Required for HAL_TIM_Base_Init call
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

// AutoGen Function Required for HAL_TIM_Base_Init call
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

/******************************************************************************/
/*                      P R I V A T E  F U N C T I O N S                      */
/******************************************************************************/

/* TIM1 init function */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 2;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 2;
  if (HAL_TIM_Encoder_Init(&htim1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* tim_encoderHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(tim_encoderHandle->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspInit 0 */

  /* USER CODE END TIM1_MspInit 0 */
    /* TIM1 clock enable */
    __HAL_RCC_TIM1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**TIM1 GPIO Configuration
    PB1     ------> TIM1_CH1
    PB4(NJTRST)     ------> TIM1_CH2
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_TIM1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM1_MspInit 1 */

  /* USER CODE END TIM1_MspInit 1 */
  }
}

void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef* tim_encoderHandle)
{

  if(tim_encoderHandle->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspDeInit 0 */

  /* USER CODE END TIM1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM1_CLK_DISABLE();

    /**TIM1 GPIO Configuration
    PB1     ------> TIM1_CH1
    PB4(NJTRST)     ------> TIM1_CH2
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_1|GPIO_PIN_4);

  /* USER CODE BEGIN TIM1_MspDeInit 1 */

  /* USER CODE END TIM1_MspDeInit 1 */
  }
}
