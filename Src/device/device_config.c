/**
 * @file device
 * @brief <description>
 */

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "main.h"
#include "device_config.h"
#include "device_gpio.h"
#include "device_timer.h"
#include "app_motor.h"

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

static void SystemClock_Config(void);
static void enableCache(void);
// static void MX_USART3_UART_Init(void);


/******************************************************************************/
/*               P R I V A T E  G L O B A L  V A R I A B L E S                */
/******************************************************************************/

// Global variables that are only used in this file (always declare static)

/******************************************************************************/
/*                P U B L I C  G L O B A L  V A R I A B L E S                 */
/******************************************************************************/
// UART_HandleTypeDef huart3;

/******************************************************************************/
/*                       P U B L I C  F U N C T I O N S                       */
/******************************************************************************/

void DeviceConfig_init(void)
{
    // Initial Hardware and clock setup
    enableCache();
    HAL_Init();
    SystemClock_Config();

    // Peripheral configurations
    // DeviceGpio_init();
    // DeviceTimer_init(); // Configures main controller ISR
    // MX_USART3_UART_Init();


    // App configurations
    // AppMotor_init();

    // // Start ISRs
    // DeviceTimer_startIrq(MAIN_CONTROL_TIMER);
    // DeviceTimer_startIrq(SIGNAL_FILTER_TIMER);

}

/******************************************************************************/
/*                      P R I V A T E  F U N C T I O N S                      */
/******************************************************************************/

/**
 * @brief Configures System clock
 *
 * Adapted from MXcube Auto generated file
 *
 */
static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Supply configuration update enable
    */
    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

    /** Configure the main internal regulator output voltage
    */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

    while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
    RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 275;
    RCC_OscInitStruct.PLL.PLLP = 1;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                                |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief Enables Multiple Caching levels on ARM CORE-7 MCU
 */
static void enableCache(void)
{
    // Enable I and D cache
    SCB_EnableICache();
    SCB_EnableDCache();
}




/********************************* TODO FIX UART GARBAGE ************************************/
// /* USART3 init function */

// static void MX_USART3_UART_Init(void)
// {

//   /* USER CODE BEGIN USART3_Init 0 */

//   /* USER CODE END USART3_Init 0 */

//   /* USER CODE BEGIN USART3_Init 1 */

//   /* USER CODE END USART3_Init 1 */
//   huart3.Instance = USART3;
//   huart3.Init.BaudRate = 921600;
//   huart3.Init.WordLength = UART_WORDLENGTH_8B;
//   huart3.Init.StopBits = UART_STOPBITS_1;
//   huart3.Init.Parity = UART_PARITY_ODD;
//   huart3.Init.Mode = UART_MODE_TX_RX;
//   huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//   huart3.Init.OverSampling = UART_OVERSAMPLING_16;
//   huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
//   huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
//   huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
//   if (HAL_UART_Init(&huart3) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /* USER CODE BEGIN USART3_Init 2 */

//   /* USER CODE END USART3_Init 2 */
//   /* Enable RXNE and Error interrupts */

// //     LL_USART_EnableIT_RXNE(USART3);
// //   LL_USART_EnableIT_ERROR(USART3);

// }

// void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
// {

//   GPIO_InitTypeDef GPIO_InitStruct = {0};
//   RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
//   if(uartHandle->Instance==USART3)
//   {
//   /* USER CODE BEGIN USART3_MspInit 0 */

//   /* USER CODE END USART3_MspInit 0 */

//   /** Initializes the peripherals clock
//   */
//     PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
//     PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
//     if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
//     {
//       Error_Handler();
//     }

//     /* USART3 clock enable */
//     __HAL_RCC_USART3_CLK_ENABLE();

//     __HAL_RCC_GPIOA_CLK_ENABLE();
//     /**USART3 GPIO Configuration
//     PA3     ------> USART3_RX
//     PA4     ------> USART3_TX
//     */
//     GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;
//     GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//     GPIO_InitStruct.Alternate = GPIO_AF13_USART3;
//     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//     /* USART3 interrupt Init */
//     HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
//     HAL_NVIC_EnableIRQ(USART3_IRQn);
//   /* USER CODE BEGIN USART3_MspInit 1 */

//   /* USER CODE END USART3_MspInit 1 */
//   }
// }

// void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
// {

//   if(uartHandle->Instance==USART3)
//   {
//   /* USER CODE BEGIN USART3_MspDeInit 0 */

//   /* USER CODE END USART3_MspDeInit 0 */
//     /* Peripheral clock disable */
//     __HAL_RCC_USART3_CLK_DISABLE();

//     /**USART3 GPIO Configuration
//     PA3     ------> USART3_RX
//     PA4     ------> USART3_TX
//     */
//     HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3|GPIO_PIN_4);

//     /* USART3 interrupt Deinit */
//     HAL_NVIC_DisableIRQ(USART3_IRQn);
//   /* USER CODE BEGIN USART3_MspDeInit 1 */

//   /* USER CODE END USART3_MspDeInit 1 */
//   }
// }
