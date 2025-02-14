//----------------------------------------------------------------------
//--
//--    grid.h
//--
//--            A game grid
//--
//----------------------------------------------------------------------

#ifndef __GEE_MINES_GRID_h__
#define __GEE_MINES_GRID_h__    1

#include "shared/casioCalcs.h"

//
// Boxes
//

// States of a single box
//
typedef enum{
    BS_INITIAL, BS_FLAG, BS_DICEY, BS_BLAST,
    BS_WRONG, BS_MINE, BS_DICEY_DOWN, BS_NUM8,
    BS_NUM7, BS_NUM6, BS_NUM5, BS_NUM4,
    BS_NUM3, BS_NUM2, BS_NUM1, BS_DOWN
} BOX_STATE;

// A box ...
//
typedef struct __box{
    BOOL mine_ : 1;
    BOX_STATE state_ : 5;
} BOX, * PBOX;

// Game difficulties
//
typedef enum {
    BEGINNER = 0, INTERMEDIATE, EXPERT
} GAME_DIFFICULTY;

// Information about a game grid
//
typedef struct __grid{
    GAME_DIFFICULTY level_;
    uint8_t         minesCount_;     // count of mines at startup
    uint8_t         cols_;           // Grid dimensions
    uint8_t         rows_;
    PBOX            boxes_;
} GRID, * PGRID;

// Helper for grid access
//
#define BOX_AT(grid, colMax, row, col) (&grid->boxes_[row*colMax + col])

//  grid_create() : Intialize a new grid
//
//  @grid : Pointer to the grid
//  @level : Difficulty of the new grid
//
//  @return : TRUE if done
//
BOOL grid_create(PGRID const grid, GAME_DIFFICULTY level);

//  grid_layMines() : Put mines in the grid
//
//  @grid : Pointer to the grid
//  @level : Difficulty of the new grid
//
//  @return : count of mines in the current grid (0 if error)
//
uint8_t grid_layMines(PGRID const grid);

#ifndef DEST_CASIO_CALC
//  grid_display() : Display the grid (for tests on Linux)
//
//  @grid : Pointer to the grid
//
void grid_display(PGRID const grid);
#endif // #ifndef DEST_CASIO_CALC

//  grid_free() : Free memory allocated for a grid
//
//  @grid : Pointer to the grid
//
void grid_free(PGRID const grid);

#endif // #ifndef __GEE_MINES_GRID_h__

