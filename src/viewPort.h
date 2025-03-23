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

#define SCROLL_WIDTH  0x000C      // 12
#define SCROLL_HEIGHT SCROLL_WIDTH

// A viewport - defines visible part of the grid
//
typedef struct __viewPort{
    DIMS dimensions;        // max. box count (w x h)
    RECT visibleFrame;      // current visible boxes IDs
    RECT scrollButtons[4];
}VIEWPORT, * PVIEWPORT;

#ifdef DEST_CASIO_CALC
#endif // #ifdef DEST_CASIO_CALC

#endif // __GEE_VIEWPORT_C_h____

// EOF
