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

#ifdef DEST_CASIO_CALC
#include <gint/timer.h>
#endif // #ifdef DEST_CASIO_CALC

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

// Images dims.
//
#define BOX_WIDTH           16
#define BOX_HEIGHT          BOX_WIDTH
#define LED_WIDTH           13
#define LED_HEIGHT          23
#define SMILEY_WIDTH        24
#define SMILEY_HEIGHT       24

// Positions
//
#define GRID_VIEWPORT_BUTTON_WIDTH  12
#define GRID_VIEWPORT_BUTTON_HEIGHT GRID_VIEWPORT_BUTTON_WIDTH
#define GRID_VIEWPORT_LEFT          2
#define GRID_VIEWPORT_TOP           2

#define SMILEY_VERT_X               200
#define SMILEY_VERT_Y               GRID_VIEWPORT_TOP

#define TIMER_VERT_X                200     // Game timer
#define TIMER_VERT_Y                40

#define TIMER_MAX_VALUE             99      // Max. game duration in sec.

// Game state
//
typedef enum {
    STATE_WAITING, STATE_PLAYING,
    STATE_WON, STATE_LOST
} GAME_STATE;

// Orientation
//
typedef enum {
    CALC_VERTICAL = 0, CALC_HORIZONTAL
} CALC_ORIENTATION;

// Type of click (action)
//
typedef enum{
    ACTION_STEP, ACTION_QUESTION, ACTION_FLAG
} ACTION, * PACTION;

// Smiley state
//
typedef enum {
    SMILEY_DOWN, SMILEY_WIN, SMILEY_LOSE, SMILEY_CAUTION, SMILEY_HAPPY
} SMILEY_STATE;

// A viewport - defines visible part of the grid
//
typedef struct __viewPort{
    DIMS dimensions;      // max. box count (w x h)
    RECT visibleFrame;      // current visible boxes window
    RECT navButtons[4];
}VIEWPORT, * PVIEWPORT;

// Game board
//
typedef struct __board{
    PGRID grid;
    VIEWPORT viewPort;
    BOOL fullGrid;            // Is the whole grid visible ?
    CALC_ORIENTATION orientation;

    RECT statRect;
    RECT counterRect;
    POINT smileyPos;
    POINT timerPos;
    RECT gridPos;

    GAME_STATE gameState;
    SMILEY_STATE smileyState;

    uint8_t uMinesLeft;
    uint8_t uMines;
    uint16_t time;
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
#define board_update(board) board_draw(board)

//  board_click() : User "clicks" on the a box
//
//  @board : Pointer to the board
//  @pos : Box coordinates in the current viewPort
//  @action : action type
//
void board_click(PBOARD const board, PCOORD const pos, ACTION action);

//  board_setGameStateEx() : Change the state of a game
//
//  @board : Pointer to the board
//  @state : new game state
//  @redraw : update screen
//
void board_setGameStateEx(PBOARD const board, GAME_STATE state, BOOL redraw);
#define board_setGameState(board, state) board_setGameStateEx(board, state, FALSE)
#define board_gameLost(board) board_setGameStateEx(board, STATE_LOST, FALSE)
#define board_gameWon(board) board_setGameStateEx(board, STATE_WON, FALSE)

//  board_setSmileyEx() : Change the state of a game
//
//  @board : Pointer to the board
//  @smiley : new smiley to draw
//  @redraw : update screen
//
void board_setSmileyEx(PBOARD const board, SMILEY_STATE smiley, BOOL redraw);
#define board_setSmiley(board, smiley) board_setSmileyEx(board, smiley, FALSE)

//  board_incTime() : Increase game timer of 1 sec.
//
//  @board : Pointer to the board
//
//  @return : FALSE if the game is over
//
BOOL board_incTime(PBOARD const board);

//  board_free() : Free a board
//
//  @board : Pointer to the board
//  @freeAll : if FALSE only boxes are freed. If TRUE boxes and grid memory will
//             be freed
//
void board_free(PBOARD const board, BOOL freeAll);

//
// Drawings
//

//  board_drawGridEx() : Draw the visible grid
//
//  @board : Pointer to the board
//  @update : if TRUE screen will be updated after drawing
//
void board_drawGridEx(PBOARD const board, BOOL update);
#define board_drawGrid(board) board_drawGridEx(board, TRUE)

//  board_drawTimeEx() : Draw time
//
//  @board : Pointer to the board
//  @update : if TRUE screen will be updated after drawing
//
void board_drawTimeEx(PBOARD const board, BOOL update);
#define board_drawTime(board) board_drawTimeEx(board, TRUE)

//  board_drawSmileyEx() : Draw the smiley
//
//  @board : Pointer to the board
//  @update : if TRUE screen will be updated after drawing
//
void board_drawSmileyEx(PBOARD const board, BOOL update);
#define board_drawSmiley(board) board_drawSmileyEx(board, TRUE)

//  board_drawBoxEx() : Draw a single box
//
//      These 2 functions draw a given box.
//      board_drawBox checks wether a rotation needs to be done
//      whereas board_drawBoxEx assumes rotation has been done by the calling function
//
//  @board : Pointer to the board
//  @pos : Box coordinates in the grid
//  @dx, @dy : Screen coordinates of the top-left corner
//
void board_drawBoxEx(PBOARD const board, PCOORD const pos, uint16_t dx, uint16_t dy);
void board_drawBox(PBOARD const board, PCOORD const pos, uint16_t dx, uint16_t dy);

//  board_selectBoxEx() : Select a box
//
//  @board : Pointer to the board
//  @pos : Box coordinates of the box in the grid
//  @select : TRUE if box is selected, FALSe if unselected
//
void board_selectBoxEx(PBOARD const board, PCOORD const pos, BOOL select);
#define  board_selectBox(board, pos) board_selectBoxEx(board, pos, TRUE)
#define  board_unselectBox(board, pos) board_selectBoxEx(board, pos, FALSE)

// board_drawLed() : Draw a led digit
//
//  Draw a led digit at the given position (no rotation)
//
//  @board : Pointer to the board
//  @digit : value to draw
//  @pos : Position in screen coordinates
//
void board_drawLed(PBOARD board, uint8_t digit, PRECT pos);

// board_drawViewPortButtonsEx() : Draw buttons for viewport scrolling
//
//  @board : pointer to the board
//  @highLight : Draw buttons in hightlighted state
//
void board_drawViewPortButtonsEx(PBOARD board, BOOL highLight);
#define board_drawViewPortButtons(board) board_drawViewPortButtonsEx(board, FALSE)

//  board_changeOrientation() : Change drawing orientation
//
//  Change display orientation and update screen
//
//  @board : Pointer to the board
//
//  @return TRUE if done
//
BOOL board_changeOrientation(PBOARD const board);

//  board_setOrientation() : Set drawing orientation
//
//  @board : Pointer to the board
//  @orientation : Drawing orientation
//
void board_setOrientation(PBOARD const board, CALC_ORIENTATION orientation);

//
//  tools
//

//  rotatePoint() : Rotate (trig. 90°) and translate a single point
//
//  @pos : Pointer to point coordinates
//
void rotatePoint(PPOINT const pos);

//  rotateRect() : Rotate (trig. 90°) and translate a rectangle
//
//  @rect : Pointer to the rect
//
void rotateRect(PRECT const rect);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // #ifndef __GEE_MINES_BOARD_h__

// EOF
