//----------------------------------------------------------------------
//--
//--    scrollBar.h
//--
//--    Functions & defines for a basic scroll-bar management
//--
//----------------------------------------------------------------------

#ifndef __GEE_CASIO_SCROLLBAR_h__
#define __GEE_CASIO_SCROLLBAR_h__    1

#include "casioCalcs.h"

#ifndef DEST_CASIO_CALC
#include <SDL2/SDL.h>       // sudo apt|dnf install SDL2-devel
#endif //   #ifndef DEST_CASIO_CALC

// Colours IDs
//
typedef enum {
    COLOUR_BKGRND, COLOUR_BAR, COLOUR_BAR_BLINK, COLOUR_MAX
}SCROLL_COLOUR_ID;

// Scrollbars shared parameters
//
typedef struct __scrollBarParams{
    CALC_ORIENTATION orientation;
    int colours[COLOUR_MAX];
    uint8_t thickness;
    uint8_t radius;
}SCROLLBAR_PARAMS, * PSCROLLBAR_PARAMS;

// Types of scrollbars
//
typedef enum {
    NO_SCROLL = 0, SCROLL_HORIZONTAL, SCROLL_VERTICAL, SCROLL_BOTH
} SCROLLBAR_TYPE;

// A scrollbar
//
typedef struct __scrollBar{
    RECT            position;
    SCROLLBAR_TYPE  type;
    uint16_t        from;
    uint16_t        length;
    uint16_t        max;
    uint8_t         space;
}SCROLLBAR, * PSCROLLBAR;

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

//
// Shared (and default) parameters by all scrollbars
//

// scroll_init() : Initializes scrollbars
//
//  Must be called before any other scrollbar function call
//
void scroll_init();

// scroll_setBarThickness() : Set the thickness in pixels of all scrollbars
//
//  @thickness : Bar thickness in pixels
//  @roundedRect : If TRUE bar will have rounded extremums
//
//  @return : the thickness in pixel (can be larger than @thickness parameter)
//              or 0 if error
//
uint8_t scroll_setBarThickness(uint8_t thickness, BOOL roundedRect);

// scroll_setColour() : Change one of the scrollbar colours
//
//  Whereas all scrollbars will be concerned by the change
//  no update of active scrollbar will be done
//
//  @colID : ID of the colour to change
//  @colour : New colour
//
//  @return : TRUE if colour has been changed
//
BOOL scroll_setColour(uint8_t colID, int colour);

// scroll_setScreenOrientation() : Change screen orientation for drawings
//
//  @orientation : could be CALC_VERTICAL (default) or CALC_VERTICCAL
//
void scroll_setScreenOrientation(CALC_ORIENTATION orientation);

//
// Scrollbar functions
//
//  Prior to call anny of these functions, shared parameters must have been initialized
//  by a scroll_init() call
//

// scrollBar_init() : Initialize a new scrollbar
//
//  @scroll : Pointer to the scrollbar
//  @type : Type of scrollbar. Could be SCROLL_VERTICAL or SCROLL_HORIZONTAL
//
BOOL scrollBar_init(PSCROLLBAR const scroll, SCROLLBAR_TYPE type);

// scrollBar_setRect() : Set scrollbar's bounding rectangle
//
//  Set / change the position and dimension of a scrollbar
//
//  @scroll : Pointer to the scrollbar
//  @left, @top : Top left corner of the rectangle
//  @width, @height : rectangle's dimensions
//
//  @return : TRUE if dimensions are valid
//
BOOL scrollBar_setRect(PSCROLLBAR const scroll, uint16_t left, uint16_t top, uint16_t width, uint16_t height);

// scrollBar_setRect() : Set scrollbar's maximum position
//
//  @scroll : Pointer to the scrollbar
//  @max :  maximum position
//
//  @return : TRUE if max value is valid
//
BOOL scrollBar_setMaxLength(PSCROLLBAR const scroll, uint16_t max);

// scrollBar_moveTo() : Change bar position in scroll
//
//  @scroll : pointer to the scrollbar
//  @pos : Bar position
//
//  @return TRUE if position is valid
//
BOOL scrollBar_moveTo(PSCROLLBAR const scroll, uint16_t pos);

// scrollBar_setLength() : Set bar's length
//
//  @scroll : pointer to the scrollbar
//  @length : New bar length
//
//  @return TRUE if length is valid
//
BOOL scrollBar_setLength(PSCROLLBAR const scroll, uint16_t length);

// scrollBar_drawEx() : Draw a scrollbar
//
//  @scroll : pointer to the scrollbar
//  @blink : TRUE if the bar should be drawn with the blink effect
//  @update : TRUE if screen should be updated
//
//  @return : TRUE if done
//
BOOL scrollBar_drawEx(PSCROLLBAR const scroll, BOOL blink, BOOL update);

//
// SDL utilities for Linux tests
//

#ifndef DEST_CASIO_CALC

//  SDL_RenderDrawCircle() : Draw a circle
//
//   Draw a circle using midpoint circle - Bresenham  algo.
//
//  @renderer : Pointer to the SDL renderer
//  @x,@y : circle's center coordinates
//  @radius : Circle's radius
//
//  @return : status
//
int SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius);

//  SDL_RenderFillCircle() : Fill a circle
//
//   Fill a circle using midpoint circle - Bresenham  algo.
//
//  @renderer : Pointer to the SDL renderer
//  @x,@y : circle's center coordinates
//  @radius : Circle's radius
//
//  @return : status
//
int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius);

// Convert RECT struct. to SDL_RECT struct.
//
#define TO_SDL_RECT(dst, src)   {dst.x = src.x; dst.y = src.y; dst.w = src.w; dst.h = src.h;}

#endif // #ifndef DEST_CASIO_CALC

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_CASIO_SCROLLBAR_h__

// EOF
