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

// Defines that are only used in this file

/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/

typedef struct TimerConfig
{
    uint8_t configureIrq;
    TIM_HandleTypeDef timConfig;
    TIM_ClockConfigTypeDef clockConfig;
    TIM_MasterConfigTypeDef masterConfig;
}TimerConfig;

/******************************************************************************/
/*            P R I V A T E  F U N C T I O N  P R O T O T Y P E S             */
/******************************************************************************/

// Prototypes for functions only used in this file (always declare static)

/******************************************************************************/
/*               P R I V A T E  G L O B A L  V A R I A B L E S                */
/******************************************************************************/

static TimerConfig TimerConfigs[NUM_DEVICE_TIMERS] =
{
    [MAIN_CONTROL_TIMER] =
    {
        .configureIrq = TRUE,
        .timConfig =
        {
            .Instance = TIM2,
            .Init.Prescaler = 9999, // TODO Add comment on math for timing
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
        }
    }
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

        if (configRet != HAL_OK)
        {
            Error_Handler();
        }
        else if (TimerConfigs[i].configureIrq == TRUE)
        {
            HAL_TIM_Base_Start_IT(&TimerConfigs[i].timConfig);
        }
    }
}

void DeviceTimer_handleIrq(DeviceTimer_Num timer)
{
    HAL_TIM_IRQHandler(&TimerConfigs[timer].timConfig);
}

// AutoGen Function Required for HAL_TIM_Base_Init call
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();

    /* TIM2 interrupt Init */
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
}

// AutoGen Function Required for HAL_TIM_Base_Init call
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();

    /* TIM2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
}

/******************************************************************************/
/*                      P R I V A T E  F U N C T I O N S                      */
/******************************************************************************/

// Implementations for functions only used in this file
