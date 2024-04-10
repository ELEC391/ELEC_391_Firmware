/**
 * @file app_image.h
 * @brief Plots Image
 */

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "app_image.h"
#include "main.h"
#include "app_motor_control.h"
#include <stdint.h>

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

#define MAX_Y -15.0F
#define MAX_X -15.0F

// Image Defines
#define UPPER_TRIANGLE_POINTS 3
#define SQUARE_POINTS 4

/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/

typedef struct{
    float_t* xData;
    float_t* yData;
    uint32_t numDataPoints;
}App_ImageData;

typedef struct{
    float_t xPos;
    float_t xThetaReq;
    float_t yPos;
    float_t yThetaReq;
    uint32_t imageIndex;
    AppImage_Num activeImage;
}App_ImagePlaneData;


/******************************************************************************/
/*            P R I V A T E  F U N C T I O N  P R O T O T Y P E S             */
/******************************************************************************/

static void requestNewSetpoint(uint32_t index, AppImage_Num image);

/******************************************************************************/
/*               P R I V A T E  G L O B A L  V A R I A B L E S                */
/******************************************************************************/

// Upper Triangle
static float_t UpperTriagleX[UPPER_TRIANGLE_POINTS] = {-1.0F, 1.0F, 0.0F};
static float_t UpperTriagleY[UPPER_TRIANGLE_POINTS] = {1.0F, 1.0F, -1.0};

// Square
static float_t SquareX[SQUARE_POINTS] = {-1.0F, 1.0F, 1.0F, -1.0F};
static float_t SquareY[SQUARE_POINTS] = {1.0F, 1.0F, -1.0F, -1.0F};


// App Data
static App_ImageData ImageData[NUM_APP_IMAGES] =
{
    [UPPER_TRIANGLE] =
    {
        .xData = UpperTriagleX,
        .yData = UpperTriagleY,
        .numDataPoints = UPPER_TRIANGLE_POINTS,
    },
    [SQUARE] =
    {
        .xData = SquareX,
        .yData = SquareY,
        .numDataPoints = SQUARE_POINTS,
    },
    [NO_IMAGE_TO_PRINT] =
    {
        .xData = NULL,
        .yData = NULL,
        .numDataPoints = 0U,
    }

};

static App_ImagePlaneData PlaneData =
{
    .activeImage = NO_IMAGE_TO_PRINT,
    .imageIndex = 0U,
    .xPos = 0.0F,
    .xThetaReq = 0.0F,
    .yPos = 0.0F,
    .yThetaReq = 0.0F
};


/******************************************************************************/
/*                P U B L I C  G L O B A L  V A R I A B L E S                 */
/******************************************************************************/

// Global variable definitions matching the extern definitions in the header

/******************************************************************************/
/*                       P U B L I C  F U N C T I O N S                       */
/******************************************************************************/

void AppImage_init(void)
{
    // Just Set Active Image to triangle -- ensure image index is zeroed
    PlaneData.activeImage = SQUARE;
    PlaneData.imageIndex = 0U;

    AppMotorControl_configureController(Y_AXIS_CONTROLLER, true);
    AppMotorControl_configureController(X_AXIS_CONTROLLER, true);
}


void AppImage_4Hz(void)
{
    AppImage_Num image = PlaneData.activeImage;

    if (image >= NO_IMAGE_TO_PRINT)
    {
        // Do not update image
    }
    else
    {
        uint32_t maxIndex = ImageData[image].numDataPoints;

        // Ensure image data is in bounds
        if (PlaneData.imageIndex < maxIndex)
        {
            requestNewSetpoint(PlaneData.imageIndex, image);
        }

        // Update Index
        if (PlaneData.imageIndex < (maxIndex - 1U))
        {
            PlaneData.imageIndex++;
        }
        else
        {
            PlaneData.imageIndex = 0U;
        }
    }
}

void AppImage_setImage(AppImage_Num image)
{
    if (image < NUM_APP_IMAGES)
    {
        PlaneData.activeImage = image;
        PlaneData.imageIndex = 0U;
    }
}

uint32_t AppImage_getImageIndex(void)
{
    return PlaneData.imageIndex;
}

float_t AppImage_getXPosition(void)
{
    return PlaneData.xPos;
}

float_t AppImage_getXThetaRequest(void)
{
    return PlaneData.xThetaReq;
}

float_t AppImage_getYPosition(void)
{
    return PlaneData.yPos;
}

float_t AppImage_getYThetaRequest(void)
{
    return PlaneData.yThetaReq;
}

/******************************************************************************/
/*                      P R I V A T E  F U N C T I O N S                      */
/******************************************************************************/

static void requestNewSetpoint(uint32_t index, AppImage_Num image)
{
    float_t yTheta, xTheta, yPos, xPos;

    xPos = ImageData[image].xData[index];
    yPos = ImageData[image].yData[index];

    yTheta = (MAX_Y / 2.0F) * (1 - yPos);
    xTheta = (MAX_X / 2.0F) * (1 + xPos);

    AppMotorControl_requestSetPoint(Y_AXIS_CONTROLLER, yTheta);
    AppMotorControl_requestSetPoint(X_AXIS_CONTROLLER, xTheta);

    // update Plane Data Struct
    PlaneData.xPos = xPos;
    PlaneData.xThetaReq = xTheta;
    PlaneData.yPos = yPos;
    PlaneData.yThetaReq = yTheta;
}
