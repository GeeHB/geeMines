//----------------------------------------------------------------------
//--
//--    viewPort.c
//--
//--    Functions & defines for a basic viewPort with scrollbars
//--
//----------------------------------------------------------------------

#include "viewPort.h"

#ifdef DEST_CASIO_CALC
    extern bopti_image_t g_scroll;
#endif // #ifndef DEST_CASIO_CALC

// viewport_isBoxVisible() : Check is box is visible
//
// Check that the box at the given position is in the viewPort visible frame
//
//  @vp : pointer to the viewport
//  @pos : Coordindate of the box in the grid
//
//  @return TRUE the box is visible
//
BOOL viewport_isBoxVisible(PVIEWPORT const vp, PCOORD const pos){
    return (pos->col >= vp->visibleFrame.x &&
            pos->col < (vp->visibleFrame.x + vp->visibleFrame.w) &&
            pos->row >= vp->visibleFrame.y &&
            pos->row < (vp->visibleFrame.y + vp->visibleFrame.h));
}

// viewport_drawScrollBars() : draaw the scrollbars
//
//  @vp : pointer to the viewport
//  @orientation : screen orientation
//  @highLight : Draw buttons in hightlighted state
//
void viewport_drawScrollBars(PVIEWPORT vp, ORIENTATION orientation,  BOOL highLight){
    BOOL showButton;
    RECT rect;

    for (uint8_t id=0; id<2; id++){

        copyRect(&rect, &vp->scrollBars[id]);

        if (orientation == CALC_HORIZONTAL){
            rotateRect(&rect);
        }

        // Show / hide the button
        if (highLight || !showButton){
#ifdef DEST_CASIO_CALC
            drect(
                rect.x, rect.y,
                rect.x + rect.w - 1, rect.y + rect.h - 1,
                COL_BKGROUND);
#endif // #ifdef DEST_CASIO_CALC
        }
        else{
#ifdef DEST_CASIO_CALC
            dsubimage(
                rect.x, rect.y,
                &g_scroll, 0, sequence[id] * SCROLL_HEIGHT,
                SCROLL_HEIGHT, SCROLL_HEIGHT,
                DIMAGE_NOCLIP);
#endif // #ifdef DEST_CASIO_CALC
           }
    } // if
}

// EOF
