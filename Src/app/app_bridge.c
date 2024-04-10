/**
 * @file app_bridge.c
 * @brief app that interfaces with H-Bridges
 */

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "app_bridge.h"
#include "main.h"
#include "device_timer.h"
#include "device_gpio.h"

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

#define MAX_DUTY 100.0F
#define MIN_DUTY 0.0F

/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/

typedef struct{
    DeviceTimer_Num timer;
    AppBridge_State state;
    float_t dutyCycle;
}App_BridgeData;

/******************************************************************************/
/*            P R I V A T E  F U N C T I O N  P R O T O T Y P E S             */
/******************************************************************************/

static volatile void setBridge(App_BridgeData bridgeHandle);


/******************************************************************************/
/*               P R I V A T E  G L O B A L  V A R I A B L E S                */
/******************************************************************************/

static volatile App_BridgeData BridgeRequest[NUM_APP_BRIDGE] =
{
    // Initialize all data to 0
    [X_AXIS_BRIDGE] =
    {
        .timer = X_AXIS_PWM_TIMER,
        .state = BRIDGE_OFF,
        .dutyCycle = 0.0F,
    },
    [Y_AXIS_BRIDGE] =
    {
        .timer = Y_AXIS_PWM_TIMER,
        .state = BRIDGE_OFF,
        .dutyCycle = 0.0F,
    },
};

static volatile App_BridgeData BridgeData[NUM_APP_BRIDGE] =
{
    // Initialize all data to 0
    [X_AXIS_BRIDGE] =
    {
        .timer = X_AXIS_PWM_TIMER,
        .state = BRIDGE_OFF,
        .dutyCycle = 0.0F,
    },
    [Y_AXIS_BRIDGE] =
    {
        .timer = Y_AXIS_PWM_TIMER,
        .state = BRIDGE_OFF,
        .dutyCycle = 0.0F,
    },
};

/******************************************************************************/
/*                P U B L I C  G L O B A L  V A R I A B L E S                 */
/******************************************************************************/

// Global variable definitions matching the extern definitions in the header

/******************************************************************************/
/*                       P U B L I C  F U N C T I O N S                       */
/******************************************************************************/

void AppBridge_init(void)
{
    // Turn set bridges into off state
    for (AppBridge_Num i = 0U; i < NUM_APP_BRIDGE; i++)
    {
        setBridge(BridgeData[i]);
    }
}

void AppBridge_10kHz(void)
{
    for (AppBridge_Num i = 0U; i < NUM_APP_BRIDGE; i++)
    {
        // Get request bridge values
        AppBridge_State reqState = BridgeRequest[i].state;
        float_t reqDuty = BridgeRequest[i].dutyCycle;


        // Check for direction change
        AppBridge_State presentState = BridgeData[i].state;
        bool dirChange = ((reqState == BRIDGE_FORWARD) & (presentState == BRIDGE_REVERSE));
        dirChange |= ((reqState == BRIDGE_REVERSE) & (presentState == BRIDGE_FORWARD));

        if (dirChange)
        {
            // Wait one control cycle before changing direction -- Pseudo Dead-time
            BridgeData[i].state = BRIDGE_STOP; // Slow down before change
        }
        else
        {
            BridgeData[i].state = reqState;
            BridgeData[i].dutyCycle = reqDuty;
        }

        setBridge(BridgeData[i]);
    }
}

void AppBridge_setBridge(AppBridge_Num bridge, AppBridge_State state, float_t dutyCycle)
{
    if (bridge < NUM_APP_BRIDGE)
    {
        BridgeRequest[bridge].dutyCycle = saturate(dutyCycle, MAX_DUTY, MIN_DUTY);
        BridgeRequest[bridge].state = state;
    }
}

/******************************************************************************/
/*                      P R I V A T E  F U N C T I O N S                      */
/******************************************************************************/

static volatile void setBridge(App_BridgeData bridgeHandle)
{
    switch (bridgeHandle.state)
    {
        case BRIDGE_OFF:
            // Ignore provided Duty
            DeviceTimer_setPwmDutyCycle(bridgeHandle.timer, PWM_CHANNEL_A, 0.0F);
            DeviceTimer_setPwmDutyCycle(bridgeHandle.timer, PWM_CHANNEL_B, 0.0F);
            break;

        case BRIDGE_STOP:
            // Ignore provided Duty
            DeviceTimer_setPwmDutyCycle(bridgeHandle.timer, PWM_CHANNEL_A, 0.0F);
            DeviceTimer_setPwmDutyCycle(bridgeHandle.timer, PWM_CHANNEL_B, 0.0F);
            break;

        case BRIDGE_FORWARD:
            // Motor Channel A set High Channel B ground
            DeviceTimer_setPwmDutyCycle(bridgeHandle.timer, PWM_CHANNEL_A, bridgeHandle.dutyCycle);
            DeviceTimer_setPwmDutyCycle(bridgeHandle.timer, PWM_CHANNEL_B, 0.0F);
            break;

        case BRIDGE_REVERSE:
            // Motor Channel A set High Channel B ground
            DeviceTimer_setPwmDutyCycle(bridgeHandle.timer, PWM_CHANNEL_A, 0.0F);
            DeviceTimer_setPwmDutyCycle(bridgeHandle.timer, PWM_CHANNEL_B, bridgeHandle.dutyCycle);
            break;

        default:
            // Bridge off for invalid inputs
            DeviceTimer_setPwmDutyCycle(bridgeHandle.timer, PWM_CHANNEL_A, 0.0F);
            DeviceTimer_setPwmDutyCycle(bridgeHandle.timer, PWM_CHANNEL_B, 0.0F);
    }
}
