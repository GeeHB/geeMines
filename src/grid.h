//----------------------------------------------------------------------
//--
//--    grid.h
//--
//--            A game grid
//--
//----------------------------------------------------------------------

#include <stdint.h>
#ifndef __GEE_MINES_GRID_h__
#define __GEE_MINES_GRID_h__    1

#include "shared/casioCalcs.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

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
    BOOL mine          : 1;
    BOX_STATE state    : 5;
} BOX, * PBOX;

// Types of grids
//
#define BEGINNER_MINES      10
#define BEGINNER_COLS       9
#define BEGINNER_ROWS       9

#define MEDIUM_MINES        40
#define MEDIUM_COLS         16
#define MEDIUM_ROWS         16

#define EXPERT_MINES        99
#define EXPERT_COLS         30
#define EXPERT_ROWS         16

// Box coordinates (in the grid)
//
typedef struct __coord{
    uint8_t col;
    uint8_t row;
} COORD, DIMS, * PCOORD, * PDIMS;

//
// Game grid
//

// Game level
//
typedef enum {
    LEVEL_BEGINNER, LEVEL_MEDIUM, LEVEL_EXPERT
} GAME_LEVEL;

// Information about a game grid
//
typedef struct __grid{
    GAME_LEVEL  level;
    uint8_t     mines;     // count of mines
    DIMS        size;
    PBOX        boxes;
    uint8_t     maxSteps;
} GRID, * PGRID;

// Helpers for box access in the grid
//
#define BOX_AT(grid, r, c) (&grid->boxes[r * grid->size.col + c])
#define BOX_AT_POS(grid, pos) (&grid->boxes[pos->row * grid->size.col + pos->col])
//#define GRID_IS_VALID_POS(grid, r, c) (r < (int8_t)grid->size.col && c < (int8_t)grid->size.col)

//  grid_create() :Create a grid
//
//  @return : pointer to the new created grid
//
PGRID grid_create();

//  grid_init() : Intialize an existing grid
//
//  @grid : Pointer to the grid
//  @level : Difficulty of the new grid
//
//  @return : TRUE if done
//
BOOL grid_init(PGRID const grid, GAME_LEVEL level);

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
//  @pos : Position of the box
//
//  @return : count of mines surrounding
//
uint8_t grid_countMines(PGRID const grid, PCOORD const pos);

//  grid_free() : Free memory allocated for a grid
//
//  @grid : Pointer to the grid
//  @freeAll : if FALSE only boxes are freed. If TRUE boxes and grid memory will
//              be freed
//
//  @return : pointer to grid or NULL if freed
//
PGRID grid_free(PGRID const grid, BOOL freeAll);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // #ifndef __GEE_MINES_GRID_h__

// EOF
