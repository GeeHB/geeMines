//----------------------------------------------------------------------
//--
//--    board.h
//--
//--            A game board
//--
//----------------------------------------------------------------------

#ifndef __GEE_MINES_BOARD_h__
#define __GEE_MINES_BOARD_h__    1

#include "shared/casioCalcs.h"
#include "grid.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

// Mine dimensions
//
#define BOX_WIDTH           16
#define BOX_HEIGHT          16

// client area width = LEFT_MARGIN + LEFT_SPACE + GRID_BORDER
//  + GRID_WIDTH(uCols*BOX_WIDTH) + GRID_BORDER + RIGHT_SPACE
#define LEFT_MARGIN         3
#define LEFT_SPACE          6
#define GRID_BORDER         3
#define RIGHT_SPACE         5

// client area height = TOP_MARGIN + TOP_SPACE
//  + STAT_BORDER + STAT_HEIGHT + STAT_BORDER
//  + VCENTER_SPACE
//  + GRID_BORDER + GRID_HEIGHT(uRows*BOX_HEIGHT) + GRID_BORDER
//  + BOTTOM_SPACE
#define TOP_MARGIN          3
#define TOP_SPACE           6
#define STAT_BORDER         2
#define STAT_HEIGHT         33
#define VCENTER_SPACE       6
#define BOTTOM_SPACE        5
// STAT_WIDTH not defined since it is determined by other parameters
// GRID_BORDER*2 + GRID_WIDTH = STAT_BORDER*2 + STAT_WIDTH

// geometric parameters within the sunken area
#define STAT_TOP_SPACE      4
#define STAT_LEFT_SPACE     5
#define STAT_RIGHT_SPACE    7
// no STAT_BOTTOM_SPACE since it is determined by other parameters

#define STAT_RECT_LEFT      (LEFT_MARGIN + LEFT_SPACE + STAT_BORDER)
#define STAT_RECT_TOP       (TOP_MARGIN + TOP_SPACE + STAT_BORDER)
#define STAT_RECT_BOTTOM    (STAT_RECT_TOP + STAT_HEIGHT)

#define COUNTER_RECT_LEFT   (STAT_RECT_LEFT + STAT_LEFT_SPACE + 1)
#define COUNTER_RECT_TOP    (STAT_RECT_TOP + STAT_TOP_SPACE + 1)

#define GRID_RECT_LEFT      (LEFT_MARGIN + LEFT_SPACE + GRID_BORDER)
#define GRID_RECT_TOP       (STAT_RECT_BOTTOM + STAT_BORDER \
                            + VCENTER_SPACE + GRID_BORDER)

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // #ifndef __GEE_MINES_BOARD_h__

// EOF
