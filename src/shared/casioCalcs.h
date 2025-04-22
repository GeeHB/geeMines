//----------------------------------------------------------------------
//--
//--    casioCalcs.h
//--
//--            Types, defines & shared functions
//--            specific to the casio targets
//--
//----------------------------------------------------------------------

#ifndef __GEE_CASIO_CALCS_h__
#define __GEE_CASIO_CALCS_h__    1

#define _GEEHB_CASIO_CALC_VER_  "1.0.1"

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
    uint   w, h;  // width and height
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
BOOL setRect(PRECT const rect, int x, int y, uint w, uint h);

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

// centerRect() : center a rect
//
//  @rect : pointer to the rect.
//  @width, @height : The width and height with which
//                    the rectangle should center
//
void centerRect(PRECT const rect, uint width, uint height);

// isEmptyRect() : Is the rect empty ?
//
//  A rectangle is empty when either its
//  length or width is zero.
//
//  @rect : pointer to the rect.
//
//  @return : TRUE if the rect is empty
//
BOOL isEmptyRect(PRECT const rect);

//
// Rotation of the screen
//

// Orientation
//
typedef enum {
    CALC_VERTICAL = 0, CALC_HORIZONTAL
} CALC_ORIENTATION;

//  rotatePoint() : Rotate (trig. 90°) and translate a single point
//
//  @pos : Pointer to point coordinates
//
void rotatePoint(PPOINT const pos);

//  rotateRect() : Rotate (trig. 90°) and translate a rectangle
//
//  @rect : Pointer to the rect
//
void rotateRect(PRECT const rect);

//
// Screen capture functions for casio calculator : if  SCREEN_CAPTURE defined
//
//      using as root fxlink command : fxlink -iw
//

#ifdef SCREEN_CAPTURE
#ifdef DEST_CASIO_CALC
    #include <gint/usb.h>
    #include <gint/usb-ff-bulk.h>

    // capture_install() : Set/install screen capture
    //
    void capture_install();

    // capture_remove() : Remove screen capture
    //
    void capture_remove();
#endif // #ifdef DEST_CASIO_CALC
#endif // #ifdef  SCREEN_CAPTURE

//
// Debug & trace primitives
//

#ifdef TRACE_MODE

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

#endif // TRACE_MODE

// __itoa() : Convert a num. val to a string
//
//  @num : Numeric value to convert
//  @format : format numbers
//  @str : destination buffer
//
//  @return : a pointer to the string
//
char* __itoa(int num, BOOL format, char *str);

// __strrev() : Reverse a string
//
//  @str : String to reverse
//
void __strrev(char *str);

//
// State & status - bitwise manips
//

BOOL isBitSet(int value, int bit);
int setBit(int value, int bit);
int removeBit(int value, int bit);

//
// Utils
//

// clearScreen() : Clear the whole screen (except menu bar ?)
//
//  @bkColour : Background colour
//  @menuHeight : part of the screen (menu) to be preserved
//  @update : update screen ?
//
void clearScreenEx(int bkColour, uint8_t menuHeight, BOOL update);
#define clearScreen(bkColour)   clearScreenEx(bkColour, 0, TRUE);

#endif // #ifndef __GEE_CASIO_CALCS_h__

// EOF
