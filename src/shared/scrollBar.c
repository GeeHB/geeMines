//----------------------------------------------------------------------
//--
//--    scrollBar.c
//--
//--    Functions & defines for a basic scroll-bar management
//--
//----------------------------------------------------------------------

#include "scrollBar.h"
#include "casioCalcs.h"
#include <stdint.h>
#include <string.h>
#include <sys/types.h>

#ifndef DEST_CASIO_CALC
extern SDL_Window* g_window;
extern SDL_Renderer* g_renderer;
#endif //   #ifndef DEST_CASIO_CALC

// Gobal variables
//
SCROLLBAR_PARAMS g_scrollParameters;
BOOL g_initDone = FALSE;

// Default values
//
#define DEF_SCROLL_THICKNESS        0x09

#define DEF_SCROLL_BK_COLOUR        COLOUR_WHITE
#define DEF_SCROLL_BAR_COLOUR       C_RGB(19,24,27)
#define DEF_SCROLL_BLINK_COLOUR     C_RGB(23, 29, 31)

#define MIN_SCROLLBAR_THICKNESS     3
#define MAX_SCROLLBAR_THICKNESS     (CASIO_HEIGHT / 2 - 1)

// scroll_init() : Initializes scrollbars
//
//  Must be called before any other scrollbar function call
//
void scroll_init(){
    g_scrollParameters.orientation = CALC_VERTICAL;
    g_scrollParameters.thickness = MIN_SCROLLBAR_THICKNESS;
    g_scrollParameters.radius = 0;
    g_scrollParameters.colours[0] = DEF_SCROLL_BK_COLOUR;
    g_scrollParameters.colours[1] = DEF_SCROLL_BAR_COLOUR;
    g_scrollParameters.colours[2] = DEF_SCROLL_BLINK_COLOUR;
    g_initDone = TRUE;
};

// scroll_setBarThickness() : Set the thickness in pixels of all scrollbars
//
//  @thickness : Bar thickness in pixels
//  @roundedRect : If TRUE bar will have rounded extremums
//
//  @return : the thickness in pixel (can be larger than @thickness parameter)
//              or 0 if error
//
uint8_t scroll_setBarThickness(uint8_t thickness, BOOL roundedRect){
    if (thickness >= MIN_SCROLLBAR_THICKNESS && thickness <= MAX_SCROLLBAR_THICKNESS){
        g_scrollParameters.thickness = ((thickness%2)?thickness:thickness+1);
        g_scrollParameters.radius = roundedRect?(g_scrollParameters.thickness - 1) / 2 : 0;
        return g_scrollParameters.thickness;
    }

    return 0;
}

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
BOOL scroll_setColour(uint8_t colID, int colour){
    if (colID < COLOUR_MAX){
        g_scrollParameters.colours[colID] = colour;
        return TRUE;
    }

    return FALSE;
}

// scroll_setScreenOrientation() : Change screen orientation for drawings
//
//  @orientation : could be CALC_VERTICAL (default) or CALC_VERTICCAL
//
void scrollBar_setScreenOrientation(CALC_ORIENTATION orientation){
    g_scrollParameters.orientation = orientation;
}

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
BOOL scrollBar_init(PSCROLLBAR const scroll, SCROLLBAR_TYPE type){
    if (!g_initDone || !scroll || type >= SCROLL_BOTH){
        return FALSE;
    }

    memset(scroll, 0, sizeof(SCROLLBAR));
    scroll->type = type;

    return TRUE;
}

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
BOOL scrollBar_setRect(PSCROLLBAR const scroll, uint16_t left, uint16_t top, uint16_t width, uint16_t height){
    if (!g_initDone || !scroll){
        return FALSE;
    }

    uint16_t maxThickness, w, h;

    if (SCROLL_VERTICAL == scroll->type){
        maxThickness = width;
        if (!(maxThickness%2)){
            maxThickness++;
        }
        w = maxThickness;
        h = height;
    }
    else{
        maxThickness = height;
        if (!(maxThickness%2)){
            maxThickness++;
        }
        w = width;
        h = maxThickness;
    }

    if (maxThickness < g_scrollParameters.thickness){
        return FALSE;   // Invalid dim.
    }

    setRect(&scroll->position, left, top, w, h);
    scroll->space = (maxThickness - g_scrollParameters.thickness) / 2;
    return TRUE;
}

// scrollBar_setRect() : Set scrollbar's maximum position
//
//  @scroll : Pointer to the scrollbar
//  @max :  maximum position
//
//  @return : TRUE if max value is valid
//
BOOL scrollBar_setMaxLength(PSCROLLBAR const scroll, uint16_t max){
    if (!g_initDone || !scroll || !max){
        return FALSE;
    }

    scroll->max = max;

    if (max < scroll->length){
        scroll->length = max;
    }

    if (max <= scroll->from){
        scroll->from = (max - 1);
    }

    return TRUE;
}

// scrollBar_moveTo() : Change bar position in scroll
//
//  @scroll : pointer to the scrollbar
//  @pos : Bar position
//
//  @return TRUE if position is valid
//
BOOL scrollBar_moveTo(PSCROLLBAR const scroll, uint16_t pos){
    if (!g_initDone || !scroll || pos >= scroll->max){
        return FALSE;
    }

    scroll->from = pos;

    return TRUE;
}

// scrollBar_setLength() : Set bar's length
//
//  @scroll : pointer to the scrollbar
//  @length : New bar length
//
//  @return TRUE if length is valid
//
BOOL scrollBar_setLength(PSCROLLBAR const scroll, uint16_t length){
    if (!g_initDone || !scroll || !length){
        return FALSE;
    }

    scroll->length = length;
    if (scroll->from >= length){
        scroll->from = length - 1;
    }

    return TRUE;
}

// scrollBar_drawEx() : Draw a scrollbar
//
//  @scroll : pointer to the scrollbar
//  @blink : TRUE if the bar should be drawn with the blink effect
//  @update : TRUE if screen should be updated
//
//  @return : TRUE if done
//
BOOL scrollBar_drawEx(PSCROLLBAR const scroll, BOOL blink, BOOL update){
    if (!g_initDone || !scroll || isEmptyRect(&scroll->position)){
        return FALSE;
    }

    RECT rectBk, rectBar;
    POINT ptBegin, ptEnd;
    uint16_t dimension;

#ifdef DEST_CASIO_CALC
    int colour = g_scrollParameters.colours[blink?COLOUR_BAR_BLINK:COLOUR_BAR];
#endif // #ifdef DEST_CASIO_CALC

    copyRect(&rectBk, &scroll->position);
    copyRect(&rectBar, &scroll->position);
    deflateRect(&rectBar, scroll->space, scroll->space);

    if (SCROLL_HORIZONTAL == scroll->type){
        dimension = rectBar.w;  // = 100%
        rectBar.w = rectBar.w * scroll->length / scroll->max;
        rectBar.x += (dimension * scroll->from / scroll->max + g_scrollParameters.radius);
        rectBar.w -= 2 * g_scrollParameters.radius;

        ptBegin = (POINT){.x=rectBar.x,.y=rectBar.y+g_scrollParameters.radius};
        ptEnd = (POINT){.x=rectBar.x + rectBar.w - 1,.y=rectBar.y+g_scrollParameters.radius};
    }
    else{
        dimension = rectBar.h;  // = 100%
        rectBar.h = rectBar.h * scroll->length /  scroll->max;
        rectBar.y += (dimension * scroll->from / scroll->max + g_scrollParameters.radius);
        rectBar.h -= 2 * g_scrollParameters.radius;

        ptBegin = (POINT){.x=rectBar.x+g_scrollParameters.radius,.y=rectBar.y};
        ptEnd = (POINT){.x=rectBar.x+g_scrollParameters.radius,.y=rectBar.y+rectBar.w-1};
    }

    if (CALC_HORIZONTAL == g_scrollParameters.orientation){
        rotateRect(&rectBk);
        rotateRect(&rectBar);

        rotatePoint(&ptBegin);   // centers of circles
        rotatePoint(&ptEnd);
    }

#ifdef DEST_CASIO_CALC
    drect(rectBk.x, rectBk.y, rectBk.x + rectBk.w -1, rectBk.y + rectBk.h -1, g_scrollParameters.colours[COLOUR_BKGRND]);     // Erase scroll. bckgrnd

    if (g_scrollParameters.radius){
        // Rounded rectangle
        dcircle(ptBegin.x, ptBegin.y, g_scrollParameters.radius, colour, colour);
        dcircle(ptEnd.x, ptEnd.y, g_scrollParameters.radius, colour, colour);
    }

    drect(rectBar.x, rectBar.y , rectBar.x + rectBar.w - 1, rectBar.y + rectBar.h - 1, colour);
#else
    SDL_Rect sRect;

    SDL_SetRenderDrawColor(g_renderer, 192, 192, 192, 255);

    TO_SDL_RECT(sRect, rectBk);
    SDL_RenderDrawRect(g_renderer, &sRect);
	SDL_RenderFillRect(g_renderer, &sRect);

    if (g_scrollParameters.radius){
    	SDL_SetRenderDrawColor(g_renderer, 128, 128, 128, 255);
        SDL_RenderFillCircle(g_renderer, ptBegin.x, ptBegin.y, g_scrollParameters.radius);
        SDL_RenderFillCircle(g_renderer, ptEnd.x, ptEnd.y, g_scrollParameters.radius);
    }

    TO_SDL_RECT(sRect, rectBar);
    SDL_RenderDrawRect(g_renderer, &sRect);
	SDL_RenderFillRect(g_renderer, &sRect);
#endif // #ifdef DEST_CASIO_CALC

    if (update){
#ifdef DEST_CASIO_CALC
        dupdate();
#endif // #ifdef DEST_CASIO_CALC
    }

    return TRUE;
}

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
int SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius){
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

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
int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius){
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {

        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                                     x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                                     x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

#endif // #ifndef DEST_CASIO_CALC

// EOF
