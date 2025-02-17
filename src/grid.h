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
    BS_INITIAL, BS_FLAG, BS_QUESTION, BS_BLAST,
    BS_WRONG, BS_MINE, BS_DICEY_DOWN, BS_NUM8,
    BS_NUM7, BS_NUM6, BS_NUM5, BS_NUM4,
    BS_NUM3, BS_NUM2, BS_NUM1, BS_DOWN
} BOX_STATE;

// A single box ...
//
typedef struct __box{
    uint8_t mine_       : 1;    // BOOL
    BOX_STATE state_    : 5;
} BOX, * PBOX;

//
// Game grid
//

// Game difficulties
//
typedef enum {
    BEGINNER = 0, MEDIUM, EXPERT
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

// Helpers for box access in the grid
//
#define GRID_AT(grid, row, col) (&grid->boxes_[row * grid->cols_ + col])
#define GRID_IS_VALID_POS(grid, r, c) (r < (int8_t)grid->cols_ && c < (int8_t)grid->cols_)

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

//  grid_countMines() : Count the mines surrounding the box
//
//  @grid : Pointer to the grid
//  @row, @col : Position of the box
//
//  @return : count of mines surrounding
//
uint8_t grid_countMines(PGRID const grid, uint8_t row, uint8_t col);

//  grid_stepBox : steps on a box
//
//  @grid : Pointer to the grid
//  @row, @col : Position of the box
//
//  @return : TRUE if pos is safe and FALSE if stepped on a bomb
//
BOOL grid_stepBox(PGRID const grid, uint8_t row, uint8_t col);

//  grid_free() : Free memory allocated for a grid
//
//  @grid : Pointer to the grid
//
void grid_free(PGRID const grid);

#endif // #ifndef __GEE_MINES_GRID_h__
