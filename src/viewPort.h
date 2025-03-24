//----------------------------------------------------------------------
//--
//--    viewPort.h
//--
//--    Functions & defines for a basic viewPort with scrollbars
//--
//----------------------------------------------------------------------

#ifndef __GEE_VIEWPORT_C_h__
#define __GEE_VIEWPORT_C_h__    1

#include "shared/casioCalcs.h"
#include "grid.h"

// Scrollbar dimensions en pixels
//
#define SCROLL_WIDTH  0x000C            // 12
#define SCROLL_HEIGHT SCROLL_WIDTH

// Scrollbar IDs
//
#define SCROLL_HORZ     0
#define SCROLL_VERT     1

// A viewport - defines visible part of the grid
//
typedef struct __viewPort{
    uint8_t status;
    DIMS    dimensions;        // max. box count (w x h)
    RECT    visibleFrame;      // current visible boxes
    RECT    scrollBars[2];
}VIEWPORT, * PVIEWPORT;

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

// viewport_isBoxVisible() : Check is box is visible
//
// Check that the box at the given position is in the viewPort visible frame
//
//  @vp : pointer to the viewport
//  @pos : Coordindate of the box in the grid
//
//  @return TRUE the box is visible
//
BOOL viewport_isBoxVisible(PVIEWPORT const vp, PCOORD const pos);

// viewport_drawScrollBars() : draaw the scrollbars
//
//  @vp : pointer to the viewport
//  @orientation : screen orientation
//  @highLight : Draw buttons in hightlighted state
//
void viewport_drawScrollBars(PVIEWPORT vp, ORIENTATION orientation,  BOOL highLight);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_VIEWPORT_C_h____

// EOF
