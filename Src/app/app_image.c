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
#define NUM_POINTS_INTERPOLATE 2.0F

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
    uint32_t imageNextIndex;
    AppImage_Num activeImage;
}App_ImagePlaneData;

typedef struct{
    float_t nextX;
    float_t nextY;
    float_t stepX;
    float_t stepY;
    float_t maxY;
    float_t maxX;
    bool xDone;
    bool yDone;
}App_ImageInterpolate;

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
    .imageNextIndex = 0U,
    .xPos = 0.0F,
    .xThetaReq = 0.0F,
    .yPos = 0.0F,
    .yThetaReq = 0.0F
};
App_ImageInterpolate InterpolateData = {0};

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
    if (ImageData[PlaneData.activeImage].numDataPoints >= 2U)
    {
        PlaneData.imageNextIndex = 1U;
        PlaneData.xPos = ImageData[PlaneData.activeImage].xData[PlaneData.imageIndex];
        PlaneData.yPos = ImageData[PlaneData.activeImage].yData[PlaneData.imageIndex];

        InterpolateData.maxX = ImageData[PlaneData.activeImage].xData[PlaneData.imageNextIndex];
        InterpolateData.maxY = ImageData[PlaneData.activeImage].yData[PlaneData.imageNextIndex];


        InterpolateData.stepX =  (ImageData[PlaneData.activeImage].xData[PlaneData.imageNextIndex] - ImageData[PlaneData.activeImage].xData[PlaneData.imageIndex]) / NUM_POINTS_INTERPOLATE;
        InterpolateData.nextX = PlaneData.xPos + InterpolateData.stepX;


        InterpolateData.stepY =  (ImageData[PlaneData.activeImage].yData[PlaneData.imageNextIndex] - ImageData[PlaneData.activeImage].yData[PlaneData.imageIndex]) / NUM_POINTS_INTERPOLATE;
        InterpolateData.nextY = PlaneData.yPos + InterpolateData.stepY;

        InterpolateData.xDone = false;
        InterpolateData.yDone = false;


        if (InterpolateData.stepX == 0.0F)
        {
            InterpolateData.xDone = true;
        }

        if (InterpolateData.stepY == 0.0F)
        {
            InterpolateData.xDone = true;
        }

        AppMotorControl_configureController(Y_AXIS_CONTROLLER, true);
        AppMotorControl_configureController(X_AXIS_CONTROLLER, true);
    }
    else
    {
        // ERROR Image has less than two data points
        PlaneData.activeImage = NO_IMAGE_TO_PRINT;
    }

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
    float_t yTheta, xTheta;

    yTheta = (MAX_Y / 2.0F) * (1 - PlaneData.yPos);
    xTheta = (MAX_X / 2.0F) * (1 + PlaneData.xPos);

    AppMotorControl_requestSetPoint(Y_AXIS_CONTROLLER, yTheta);
    AppMotorControl_requestSetPoint(X_AXIS_CONTROLLER, xTheta);

    // update Plane Data Struct
    PlaneData.xThetaReq = xTheta;
    PlaneData.yThetaReq = yTheta;

    // Update Y Data
    if (InterpolateData.maxY >= 0.0F)
    {
        if (InterpolateData.nextY > InterpolateData.maxY)
        {
            InterpolateData.yDone = true;
        }
        else
        {
            PlaneData.yPos = InterpolateData.nextY;
            InterpolateData.nextY = PlaneData.yPos + InterpolateData.stepY;
        }
    }
    else
    {
        if (InterpolateData.nextY < InterpolateData.maxY)
        {
            InterpolateData.yDone = true;
        }
        else
        {
            PlaneData.yPos = InterpolateData.nextY;
            InterpolateData.nextY = PlaneData.yPos + InterpolateData.stepY;
        }
    }

    // Update X Data
    if (InterpolateData.maxX >= 0.0F)
    {
        if (InterpolateData.nextX > InterpolateData.maxX)
        {
            InterpolateData.xDone = true;
        }
        else
        {
            PlaneData.xPos = InterpolateData.nextX;
            InterpolateData.nextX = PlaneData.xPos + InterpolateData.stepX;
        }
    }
    else
    {
        if (InterpolateData.nextX < InterpolateData.maxX)
        {
            InterpolateData.xDone = true;
        }
        else
        {
            PlaneData.xPos = InterpolateData.nextX;
            InterpolateData.nextX = PlaneData.xPos + InterpolateData.stepX;
        }
    }


    if (InterpolateData.xDone && InterpolateData.yDone)
    {
        // Update Indicies
        if (PlaneData.imageNextIndex == (ImageData[PlaneData.activeImage].numDataPoints - 1))
        {
            PlaneData.imageIndex = PlaneData.imageNextIndex;
            PlaneData.imageNextIndex = 0U;
        }
        else
        {
            PlaneData.imageIndex = PlaneData.imageNextIndex;
            PlaneData.imageNextIndex = 0U;
        }

        PlaneData.xPos = ImageData[PlaneData.activeImage].xData[PlaneData.imageIndex];
        PlaneData.yPos = ImageData[PlaneData.activeImage].yData[PlaneData.imageIndex];

        InterpolateData.maxX = ImageData[PlaneData.activeImage].xData[PlaneData.imageNextIndex];
        InterpolateData.maxY = ImageData[PlaneData.activeImage].yData[PlaneData.imageNextIndex];


        InterpolateData.stepX =  (ImageData[PlaneData.activeImage].xData[PlaneData.imageNextIndex] - ImageData[PlaneData.activeImage].xData[PlaneData.imageIndex]) / NUM_POINTS_INTERPOLATE;
        InterpolateData.nextX = PlaneData.xPos + InterpolateData.stepX;


        InterpolateData.stepY =  (ImageData[PlaneData.activeImage].yData[PlaneData.imageNextIndex] - ImageData[PlaneData.activeImage].yData[PlaneData.imageIndex]) / NUM_POINTS_INTERPOLATE;
        InterpolateData.nextY = PlaneData.yPos + InterpolateData.stepY;

        InterpolateData.xDone = false;
        InterpolateData.yDone = false;


        if (InterpolateData.stepX == 0.0F)
        {
            InterpolateData.xDone = true;
        }

        if (InterpolateData.stepY == 0.0F)
        {
            InterpolateData.xDone = true;
        }
    }
}
