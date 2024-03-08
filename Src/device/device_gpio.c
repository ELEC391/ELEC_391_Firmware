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

typedef struct device_gpio_config
{
    GPIO_TypeDef* gpio_port;
    GPIO_InitTypeDef gpio_init_config;

}device_gpio_config;

/******************************************************************************/
/*            P R I V A T E  F U N C T I O N  P R O T O T Y P E S             */
/******************************************************************************/

// Prototypes for functions only used in this file (always declare static)

/******************************************************************************/
/*               P R I V A T E  G L O B A L  V A R I A B L E S                */
/******************************************************************************/

static device_gpio_config gpio_config[NUM_GPIO_PINS] =
{
    [BOARD_LED_PIN] =
    {
        .gpio_port = GPIOA,
        .gpio_init_config =
        {
            .Pin = GPIO_PIN_5,
            .Mode = GPIO_MODE_OUTPUT_PP,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_LOW,
        }
    },
    [DEBUG_PIN_D7] =
    {
        .gpio_port = GPIOC,
        .gpio_init_config =
        {
            .Pin = GPIO_PIN_7,
            .Mode = GPIO_MODE_OUTPUT_PP,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_LOW
        }
    },
    [DEBUG_PIN_D8] =
    {
        .gpio_port = GPIOC,
        .gpio_init_config =
        {
            .Pin = GPIO_PIN_8,
            .Mode = GPIO_MODE_OUTPUT_PP,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_LOW
        }
    },
    [BLUE_BUTTON_PIN] =
    {
        .gpio_port = GPIOC,
        .gpio_init_config =
        {
            .Pin = GPIO_PIN_13,
            .Mode = GPIO_MODE_IT_RISING,
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

void device_gpio_init(void)
{
    for(device_gpio_pin i = (device_gpio_pin) 1U; i < NUM_GPIO_PINS; i++)
    {
        HAL_GPIO_Init(gpio_config[i].gpio_port, &gpio_config[i].gpio_init_config);
    }
}

void device_gpio_toggle(device_gpio_pin pin_num)
{
    HAL_GPIO_TogglePin(gpio_config[pin_num].gpio_port, gpio_config[pin_num].gpio_init_config.Pin);
}

/******************************************************************************/
/*                      P R I V A T E  F U N C T I O N S                      */
/******************************************************************************/

// Implementations for functions only used in this file
