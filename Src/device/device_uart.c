/**
 * @file device_uart.c
 * @brief  Base UART config and functions
 */

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "device_uart.h"
#include "main.h"

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

#define STLK_VCP_RX_Pin GPIO_PIN_8
#define STLK_VCP_RX_GPIO_Port GPIOD
#define STLK_VCP_TX_Pin GPIO_PIN_9
#define STLK_VCP_TX_GPIO_Port GPIOD

/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/
typedef struct UartConfig
{
    UART_HandleTypeDef uartConfig;
}UartConfig;

/******************************************************************************/
/*            P R I V A T E  F U N C T I O N  P R O T O T Y P E S             */
/******************************************************************************/

/******************************************************************************/
/*               P R I V A T E  G L O B A L  V A R I A B L E S                */
/******************************************************************************/

static UartConfig UartConfigs[NUM_UART_CONNECTIONS] =
{
    [MAIN_LOGGING_CHANNEL] =
    {
        .uartConfig =
        {
            .Instance = USART3,
            .Init.BaudRate = 460800,
            .Init.WordLength = UART_WORDLENGTH_8B,
            .Init.StopBits = UART_STOPBITS_1, // 7 Data 1 Parity bit
            .Init.Parity = UART_PARITY_ODD,
            .Init.Mode = UART_MODE_TX_RX,
            .Init.HwFlowCtl = UART_HWCONTROL_NONE,
            .Init.OverSampling = UART_OVERSAMPLING_16,
            .Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE,
            .Init.ClockPrescaler = UART_PRESCALER_DIV1,
            .AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT
        }
    }
};

/******************************************************************************/
/*                       P U B L I C  F U N C T I O N S                       */
/******************************************************************************/

void DeviceUart_init(void)
{
    HAL_StatusTypeDef configRet = HAL_OK;

    // Configure timers
    for(DeviceUart_Num i = (DeviceUart_Num) 0U; i < NUM_UART_CONNECTIONS; i++)
    {
        // Try to configure each HAL_OK == 0x00 Any other fail is a fail condition
        configRet |= HAL_UART_Init(&UartConfigs[i].uartConfig);
        configRet |= HAL_UARTEx_SetTxFifoThreshold(&UartConfigs[i].uartConfig, UART_TXFIFO_THRESHOLD_1_8);
        configRet |= HAL_UARTEx_SetRxFifoThreshold(&UartConfigs[i].uartConfig, UART_TXFIFO_THRESHOLD_1_8);
        configRet |= HAL_UARTEx_DisableFifoMode(&UartConfigs[i].uartConfig);

        if (configRet != HAL_OK)
        {
            Error_Handler();
        }

        // TODO configure RX
        // LL_USART_EnableIT_RXNE(USART3);
        // LL_USART_EnableIT_ERROR(USART3);
    }
}

// TODO Make an application to better handle UART COMMS
void DeviceUart_sendMessage(DeviceUart_Num uartChannel, char* msg)
{
    // TODO: configure better error handling
    if (uartChannel != NUM_UART_CONNECTIONS)
    {
        if (HAL_UART_Transmit(&UartConfigs[uartChannel].uartConfig, (uint8_t*)msg, strlen(msg), 1000)!= HAL_OK)
        {
            /* Transfer error in transmission process */
            Error_Handler();
        }
    }
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

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
    PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PA3     ------> USART3_RX
    PA4     ------> USART3_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF13_USART3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PA3     ------> USART3_RX
    PA4     ------> USART3_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3|GPIO_PIN_4);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}
