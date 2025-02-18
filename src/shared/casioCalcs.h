//----------------------------------------------------------------------
//--
//--    casioCalcs.h
//--
//--            Types and defines specific to the casio targets
//--
//----------------------------------------------------------------------

#ifndef __GEE_CASIO_CALCS_h__
#define __GEE_CASIO_CALCS_h__    1

#ifdef DEST_CASIO_CALC
// Specific includes for calculators
#include <gint/gint.h>
#include <gint/display.h>

// Screen dimensions in pixels
#define CASIO_WIDTH     DWIDTH
#define CASIO_HEIGHT    DHEIGHT
#else
// Screen dimensions in pixels for tests
#define CASIO_WIDTH     384
#define CASIO_HEIGHT    192
#endif // #ifdef DEST_CASIO_CALC

#include <stdint.h>
#include <stdlib.h>

//
// A few basic colours
//

#ifndef DEST_CASIO_CALC
// 24 bits RGB (for tests only on Windows and Linux)
#define C_RGB(r,g,b)    ((uint32_t)(((uint8_t)(r)|((uint16_t)((uint8_t)(g))<<8))|(((uint32_t)(uint8_t)(b))<<16)))
#endif // #ifndef DEST_CASIO_CALC

#ifdef FX9860G
#define COLOUR_BLACK    C_BLACK
#define COLOUR_WHITE    C_WHITE
#define COLOUR_RED      C_DARK
#define COLOUR_GREEN    C_BLACK
#define COLOUR_YELLOW   C_BLACK
#define COLOUR_BLUE     C_DARK
#define COLOUR_LT_BLUE  C_BLACK
#define COLOUR_PURPLE   C_DARK
#define COLOUR_CYAN     C_BLACK
#define COLOUR_ORANGE   C_DARK
#define COLOUR_LT_GREY  C_LIGHT
#define COLOUR_GREY     C_WHITE
#define COLOUR_DK_GREY  C_DARK
#define NO_COLOR        -1
#else
#define COLOUR_BLACK    C_RGB(0, 0, 0)
#define COLOUR_WHITE    C_RGB(31, 31, 31)
#define COLOUR_RED      C_RGB(31, 0, 0)
#define COLOUR_GREEN    C_RGB(0, 31, 0)
#define COLOUR_YELLOW   C_RGB(31, 31, 0)
#define COLOUR_BLUE     C_RGB(0, 0, 31)
#define COLOUR_LT_BLUE  C_RGB(6, 6, 31)
#define COLOUR_PURPLE   C_RGB(31, 0, 31)
#define COLOUR_CYAN     C_RGB(0, 31, 31)
#define COLOUR_ORANGE   C_RGB(31, 16, 0)
#define COLOUR_LT_GREY  C_RGB(29, 29, 29)
#define COLOUR_GREY     C_RGB(16, 16, 16)
#define COLOUR_DK_GREY  C_RGB(8, 8, 8)
#define NO_COLOR        -1
#endif // #ifdef FX9860G

// BOOLEANS
//
#ifndef FALSE
    #define FALSE   0
    #define TRUE    !FALSE
#endif // #ifndef FALSE

typedef int BOOL;

// Point coordinates
//
typedef struct __point{
    int16_t    x;
    int16_t    y;
} POINT;

// A simple rect. struct
//
typedef struct __rect{
    int    x,y;    // top left
    int    w, h;   // width and height
} RECT;

#define SET_RECT(rect, px,py,width,height) rect.x=px; rect.y=py; rect.w=width; rect.h=height

#endif // #ifndef __GEE_CASIO_CALCS_h__

// EOF
