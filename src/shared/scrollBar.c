//----------------------------------------------------------------------
//--
//--    scrollBar.c
//--
//--    Functions & defines for a basic scroll-bar management
//--
//----------------------------------------------------------------------

#include "scrollBar.h"
#include "casioCalcs.h"
#include <string.h>
#include <sys/types.h>

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

    uint16_t maxThickness = ((CALC_VERTICAL == g_scrollParameters.orientation)?width:height);
    if (!(maxThickness%2) || maxThickness > g_scrollParameters.thickness){
        return FALSE;   // Invalid dim.
    }

    setRect(&scroll->position, left, top, width, height);
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
    if (!g_initDone || !scroll){
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
#endif // #ifdef DEST_CASIO_CALC

    if (update){
#ifdef DEST_CASIO_CALC
        dupdate();
#endif // #ifdef DEST_CASIO_CALC
    }

    return TRUE;
}

// EOF
