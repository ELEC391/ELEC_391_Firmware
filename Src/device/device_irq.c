/**
 * @file device_irq.c
 * @brief All functions relating to interrupts
 */

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "main.h"
#include "device_config.h"
#include "device_irq.h"
#include "device_timer.h"
#include "device_gpio.h"
#include "app_motor.h"
#include "app_bridge.h"
#include "app_motor_control.h"

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

static void pushButtonHomingCallback(void);

/******************************************************************************/
/*               P R I V A T E  G L O B A L  V A R I A B L E S                */
/******************************************************************************/

static volatile uint32_t counter_ms = 0U;

/******************************************************************************/
/*                P U B L I C  G L O B A L  V A R I A B L E S                 */
/******************************************************************************/

/******************************************************************************/
/*                       P U B L I C  F U N C T I O N S                       */
/******************************************************************************/

volatile uint32_t DeviceIrq_getCount_ms(void)
{
    return counter_ms;
}

void DeviceIrq_clearCounter(void)
{
    counter_ms = 0U;
}
/******************************************************************************/
/* STM32H5xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h5xx.s).                    */
/******************************************************************************/
void TIM3_IRQHandler(void)
{
    static uint16_t count = 0;

    // Handle IRQ
    DeviceTimer_handleIrq(SIGNAL_FILTER_TIMER);

    // Blinky Timing
    if (count >= 10000U)
    {
        count = 0;
        // DeviceGpio_toggle(TEST_LED_B_PIN);
    }
    count++;

    AppBridge_10kHz();
    AppMotor_10kHz();
}

void TIM2_IRQHandler(void)
{
    static uint16_t count = 0;
    // Handle IRQ
    DeviceTimer_handleIrq(MAIN_CONTROL_TIMER);

    // Blinky Timing
    if (count >= 500U)
    {
        count = 0;
        // DeviceGpio_toggle(TEST_LED_A_PIN);
    }
    count++;
    counter_ms++;

    AppMotorControl_1kHz();
}

void EXTI15_10_IRQHandler(void)
{
    // Only handle IRQ on push button pin
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
    pushButtonHomingCallback();
}

/******************************************************************************/
/*                      P R I V A T E  F U N C T I O N S                      */
/******************************************************************************/

static void pushButtonHomingCallback(void)
{
    AppMotor_reinitializeData();
    // static bool homingComplete = false;

    // if (!homingComplete)
    // {
    //     homingComplete = true;
    //     AppMotor_reinitializeData();
    // }
}

/******************************************************************************/
/*                  Cortex-M7 Processor Exceptions Handlers                   */
/******************************************************************************/

/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
   while (1)
  {
  }
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  while (1)
  {

  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{

}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{

}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{

}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
    HAL_IncTick();
}
