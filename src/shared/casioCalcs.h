//----------------------------------------------------------------------
//--
//--    casioCalcs.h
//--
//--            Types and defines specific to the casio targets
//--            & shared functions
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

typedef uint8_t BOOL;

// Ensure value is in the range
#define SET_IN_RANGE(val, min, max) ((val < min)?min:((val > max)?max:val))

// Min. / Max.
#define MIN_VAL(a,b)    (a<=b?a:b)
#define MAX_VAL(a,b)    MIN_VAL(b,a)

// Point coordinates
//
typedef struct __point{
    int16_t    x;
    int16_t    y;
} POINT, * PPOINT;

// A simple rect. struct
//
typedef struct __rect{
    int    x,y;    // top left
    int    w, h;   // width and height
} RECT, * PRECT;

//
// Rect. manipulation functions
//

// setRect() : Set rect dims & pos
//
//  @rect : pointer to the dest. rect
//  @x, @y : Top left corner of the rectangle
//  @w, @h : Width and height of the rectangle
//
//  @return : TRUE if values set
//
BOOL setRect(PRECT const rect, int x, int y, int w, int h);

// offsetRect() : Translate a rect
//
//  @rect : pointer to the rect.
//  @dx, @dy : Translation units
//
//  @return : TRUE if successfully translated
//
BOOL offsetRect(PRECT const rect, int dx, int dy);

// copyRect() : Make a copy of a rect. struct.
//
//  @dest : pointer to the destination rect.
//  @src : pointer to the source rect.
//
//  @return : TRUE if successfully copied
//
BOOL copyRect(PRECT const dest, PRECT const src);

// inflateRect() : Inflate a rect
//
//  @rect : pointer to the rect.
//  @dx, @dy : Inflation units
//
//  @return : TRUE if successfully inflated
//
BOOL inflateRect(PRECT const rect, int dx, int dy);

// deflateRect() : deflate a rect
//
//  @rect : pointer to the rect.
//  @dx, @dy : Deflation units
//
//  @return : TRUE if successfully deflated
//
BOOL deflateRect(PRECT const rect, int dx, int dy);

#ifdef SCREEN_CAPTURE
#ifdef DEST_CASIO_CALC
    #include <gint/usb.h>
    #include <gint/usb-ff-bulk.h>

    //
    // Screen capture for casio calculator : if  SCREEN_CAPTURE defined
    //
    //      using as root fxlink command -[ fxlink -iw ]
    //

    // capture_install() : Set/install screen capture
    //
    void capture_install();

    // capture_remove() : Remove screen capture
    //
    void capture_remove();
#endif // #ifdef DEST_CASIO_CALC
#endif // #ifdef  SCREEN_CAPTURE

#ifdef TRACE_MODE

//
// Debug & trace primitives
//

// __coordtoa() : Format a point coord. to an output string
//
//  This specific method creates a string composed of the name of the value
//  and the value it self. It is equivalent to a sprintf(out, "%s : %d", name, value)
//
//  The base can't be changed it is always equal to 10
//
//  This method assumes the output buffer - ie. str - is large enough to contain
//  the name and the formated value.
//
//  @name : Name of the value (can be NULL)
//  @x,@y : Position to show
//  @str : Pointer to output string
//
//  @return : pointer to formated string
//
char* __coordtoa(const char* name, uint8_t x, uint8_t y, char* str);

// __atoi() : Convert a num. val to a string
//
//  @num : Numeric value to convert
//  @str : String to reverse
//
//  @return : a pointer to the string
//
char* __atoi(int num, char *str);

// __strrev() : Reverse a string
//
//  @str : String to reverse
//
void __strrev(char *str);

#endif // TRACE_MODE

#endif // #ifndef __GEE_CASIO_CALCS_h__

// EOF
