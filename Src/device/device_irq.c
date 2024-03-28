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

void SampleComputeLoad(void);

/******************************************************************************/
/*               P R I V A T E  G L O B A L  V A R I A B L E S                */
/******************************************************************************/

// Global variables that are only used in this file (always declare static)

/******************************************************************************/
/*                P U B L I C  G L O B A L  V A R I A B L E S                 */
/******************************************************************************/

/******************************************************************************/
/*                       P U B L I C  F U N C T I O N S                       */
/******************************************************************************/

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
        DeviceGpio_toggle(YELLOW_LED_PIN);
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
        DeviceGpio_toggle(RED_LED_PIN);
    }
    count++;

    // Control execution timing
    SampleComputeLoad();
}

// TODO Determine if these auto Gen functions are required
/* Cortex Processor Interruption and Exception Handlers */
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

/******************************************************************************/
/*                      P R I V A T E  F U N C T I O N S                      */
/******************************************************************************/

void SampleComputeLoad(void)
{
    float_t x = 1234.5678f;
    float_t avg = 0.0f;
    uint8_t count = 50U;
    for(uint8_t i = 0U; i < count; i++)
    {
        avg += (float_t) i * x;
    }
    avg = avg / ((float_t) count);
}