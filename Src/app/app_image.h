/**
 * @file app_image.h
 * @brief Plots Image
 */

#ifndef APP_IMAGE_H_
#define APP_IMAGE_H_

/******************************************************************************/
/*                              I N C L U D E S                               */
/******************************************************************************/

#include "app_image.h"
#include "main.h"

/******************************************************************************/
/*                               D E F I N E S                                */
/******************************************************************************/

// Public defines that may be used by other files

/******************************************************************************/
/*                              T Y P E D E F S                               */
/******************************************************************************/
typedef enum AppImage_Num
{
    SQUARE = 0U,
    UPPER_TRIANGLE,
    OCTAGON,
    STAR,
    BATMAN,
    NO_IMAGE_TO_PRINT,
    NUM_APP_IMAGES
} AppImage_Num;

/******************************************************************************/
/*                       G L O B A L  V A R I A B L E S                       */
/******************************************************************************/

// Public global variables that may be used by other files
// (always declare extern)

/******************************************************************************/
/*                             F U N C T I O N S                              */
/******************************************************************************/

void AppImage_4Hz(void);
void AppImage_init(void);

void AppImage_setImage(AppImage_Num image);
uint32_t AppImage_getImageIndex(void);
float_t AppImage_getXThetaRequest(void);
float_t AppImage_getXPosition(void);
float_t AppImage_getYThetaRequest(void);
float_t AppImage_getYPosition(void);

/******************************************************************************/
/*                       I N L I N E  F U N C T I O N S                       */
/******************************************************************************/

// Inline function declarations and implementations

#endif // APP_IMAGE_H_
