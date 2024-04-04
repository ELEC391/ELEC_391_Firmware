/**
 * @file device_gpio.c
 * @brief GPIO device configuration
 */

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "main.h"
#include "device_gpio.h"

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

// Defines that are only used in this file

/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/
typedef struct GpioConfig
{
    GPIO_TypeDef* gpioPort;
    GPIO_InitTypeDef gpioInitConfig;

}GpioConfig;

/******************************************************************************/
/*            P R I V A T E  F U N C T I O N  P R O T O T Y P E S             */
/******************************************************************************/

// Prototypes for functions only used in this file (always declare static)

/******************************************************************************/
/*               P R I V A T E  G L O B A L  V A R I A B L E S                */
/******************************************************************************/

static GpioConfig gpioConfig[NUM_GPIO_PINS] =
{
    [RED_LED_PIN] =
    {
        .gpioPort = GPIOB,
        .gpioInitConfig =
        {
            .Pin = GPIO_PIN_14,
            .Mode = GPIO_MODE_OUTPUT_PP,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_LOW,
        }
    },
    [YELLOW_LED_PIN] =
    {
        .gpioPort = GPIOE,
        .gpioInitConfig =
        {
            .Pin = GPIO_PIN_1,
            .Mode = GPIO_MODE_OUTPUT_PP,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_LOW
        }
    },
    [GREEN_LED_PIN] =
    {
        .gpioPort = GPIOB,
        .gpioInitConfig =
        {
            .Pin = GPIO_PIN_0,
            .Mode = GPIO_MODE_OUTPUT_PP,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_LOW
        }
    },
    [X_AXIS_MOTOR_ENABLE] =
    {
        .gpioPort = GPIOG,
        .gpioInitConfig =
        {
            .Pin = GPIO_PIN_14,
            .Mode = GPIO_MODE_OUTPUT_PP,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_LOW,
        }
    },
    [Y_AXIS_MOTOR_ENABLE] =
    {
        .gpioPort = GPIOC,
        .gpioInitConfig =
        {
            .Pin = GPIO_PIN_2,
            .Mode = GPIO_MODE_OUTPUT_PP,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_LOW
        }
    },
    [DEBUG_PIN_D64] =
    {
        .gpioPort = GPIOG,
        .gpioInitConfig =
        {
            .Pin = GPIO_PIN_1,
            .Mode = GPIO_MODE_OUTPUT_PP,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_LOW
        }
    },
    [DEBUG_PIN_D65] =
    {
        .gpioPort = GPIOG,
        .gpioInitConfig =
        {
            .Pin = GPIO_PIN_0,
            .Mode = GPIO_MODE_OUTPUT_PP,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_LOW
        }
    },
    [BLUE_BUTTON_PIN] =
    {
        .gpioPort = GPIOC,
        .gpioInitConfig =
        {
            .Pin = GPIO_PIN_13,
            .Mode = GPIO_MODE_IT_FALLING,
            .Pull = GPIO_NOPULL
        }
    }
};

/******************************************************************************/
/*                P U B L I C  G L O B A L  V A R I A B L E S                 */
/******************************************************************************/



/******************************************************************************/
/*                       P U B L I C  F U N C T I O N S                       */
/******************************************************************************/

void DeviceGpio_init(void)
{
    // GPIO Ports Clock Enable
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    for(DeviceGpio_Pin i = (DeviceGpio_Pin) 0U; i < NUM_GPIO_PINS; i++)
    {
        HAL_GPIO_WritePin(gpioConfig[i].gpioPort, gpioConfig[i].gpioInitConfig.Pin, GPIO_PIN_RESET);
        HAL_GPIO_Init(gpioConfig[i].gpioPort, &gpioConfig[i].gpioInitConfig);
    }
}

void DeviceGpio_toggle(DeviceGpio_Pin pin_num)
{
    if (pin_num < NUM_GPIO_PINS)
    {
        HAL_GPIO_TogglePin(gpioConfig[pin_num].gpioPort, gpioConfig[pin_num].gpioInitConfig.Pin);
    }
}

void DeviceGpio_enable(DeviceGpio_Pin pin_num)
{
    if (pin_num < NUM_GPIO_PINS)
    {
        HAL_GPIO_WritePin(gpioConfig[pin_num].gpioPort, gpioConfig[pin_num].gpioInitConfig.Pin, GPIO_PIN_SET);
    }
}

void DeviceGpio_disable(DeviceGpio_Pin pin_num)
{
    if (pin_num < NUM_GPIO_PINS)
    {
        HAL_GPIO_WritePin(gpioConfig[pin_num].gpioPort, gpioConfig[pin_num].gpioInitConfig.Pin, GPIO_PIN_RESET);
    }
}

/******************************************************************************/
/*                      P R I V A T E  F U N C T I O N S                      */
/******************************************************************************/

// Implementations for functions only used in this file
