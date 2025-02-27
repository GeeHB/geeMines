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
#define BOX_HEIGHT          BOX_WIDTH

// mine defines
#define LED_WIDTH           13
#define LED_HEIGHT          23
#define FACE_WIDTH          24
#define FACE_HEIGHT         24

// Positions
//
#define GRID_VIEWPORT_BUTTON_WIDTH  4
#define GRID_VIEWPORT_LEFT          2
#define GRID_VIEWPORT_TOP           2

// Game state
//
typedef enum __gameState{
    STATE_WAITING, STATE_PLAYING,
    STATE_GAMEWON, STATE_GAMELOST
} GAME_STATE;

// Type of click (action)
//
typedef enum{
    ACTION_STEP, ACTION_QUESTION, ACTION_FLAG
} ACTION, * PACTION;

// Smiley state
//
typedef enum {
    FACE_DOWN, FACE_WIN, FACE_LOSE, FACE_CAUTION, FACE_HAPPY
} FACE_STATE;

// A "small" rectangle
//
typedef struct __srect{
    int    left, top;
    int    right, bottom;
} SRECT;

#define SET_SRECT(rect, left,top,right,bottom) rect.left=left; rect.top=op; rect.right=right; rect.bottom=bottom
#define SET_SRECT_DIMS(rect, px,py,width,height) rect.left=px; rect.top=py; rect.right=px+width-1; rect.bottom=py+height-1
#define OFFSET_SRECT(rect, dx, dy)  rect.left+=dx; rect.top=dy; rect.right+=dx; rect.bottom+=dy

// A viewport - defines visible part of the grid
//
typedef struct __viewPort{
    SPOINT dimensions;      // max. box count (w x h)
    SRECT visibleFrame;     // current visible boxes window
}VIEWPORT, * PVIEWPORT;

// Game board
//
typedef struct __board{
    PGRID grid;
    VIEWPORT viewPort;

    SRECT statRect;
    SRECT counterRect;
    SRECT faceRect;
    SRECT timerRect;
    SRECT gridRect;

    GAME_STATE gameState;
    FACE_STATE faceState;

    uint8_t uMinesLeft;
    uint8_t uMines;
    uint16_t uTime;
} BOARD, * PBOARD;

//
// Board management
//

//  board_create() : Create an empty board
//
//  @return : Pointer to the board
//
PBOARD board_create();

//  board_init() : Initializes the board
//
//  @board : Pointer to the board
//  @level : Difficulty of the new board
//
//  @return : TRUE if done
//
BOOL board_init(PBOARD const board, GAME_LEVEL level);

//  board_draw() : Draw the whole board
//
//  @board : Pointer to the board
//
void board_draw(PBOARD const board);

//  board_click() : User "clicks" on the a box
//
//  @board : Pointer to the board
//  @row, @col : Box coordinates in the current viewPort
//  @action : action type
//
void board_click(PBOARD const board, uint8_t row, uint8_t col, ACTION action);

//  board_free() : Free a board
//
//  @board : Pointer to the board
//  @freeAll : if FALSE only boxes are freed. If TRUE boxes and grid memory will
//             be freed
//
void board_free(PBOARD const board, BOOL freeAll);

//
// Box management
//

//  board_drawBox() : Draw a single box
//
//  @board : Pointer to the board
//  @row, @col : Box coordinates
//
void board_drawBox(PBOARD const board, uint8_t row, uint8_t col);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // #ifndef __GEE_MINES_BOARD_h__

// EOF
