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
#include "consts.h"
#include "grid.h"

#ifdef TRACE_MODE
#include "shared/trace.h"
#endif // TRACE_MODE

#ifdef DEST_CASIO_CALC
#include <gint/timer.h>
#endif // #ifdef DEST_CASIO_CALC

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

// Images dims.
//
#define BOX_WIDTH           0x0010  // 16
#define BOX_HEIGHT          BOX_WIDTH
#define SMILEY_WIDTH        0x0018  // 24
#define SMILEY_HEIGHT       SMILEY_WIDTH

// Positions & dims.
//
#define GRID_VIEWPORT_BUTTON_WIDTH  0x000C      // 12
#define GRID_VIEWPORT_BUTTON_HEIGHT GRID_VIEWPORT_BUTTON_WIDTH
#
#define PLAYGROUND_BORDER   0x0003
#define STAT_BORDER         0x0003

#define GRID_VIEWPORT_LEFT  (STAT_BORDER + 2)
#define GRID_VIEWPORT_TOP   GRID_VIEWPORT_LEFT

// LED images
//
#define LED_WIDTH           0x000D  // 13
#define LED_HEIGHT          0x0017  // 23
#define LED_ZONE_WIDTH      3 * LED_WIDTH

#define SMILEY_OFFSET_V     LED_ZONE_WIDTH

#define STAT_LEFT_V         0x00D4     // (212) Centered in the right part of the screen
#define STAT_TOP_V          GRID_VIEWPORT_TOP
#define STAT_WIDTH          (6*LED_WIDTH + SMILEY_WIDTH)
#define STAT_HEIGHT         SMILEY_HEIGHT

#define LED_MINUS_ID        11      // reverse order
#define LED_EMPTY_ID        10

// Time
//
#define TIMER_OFFSET_V      LED_ZONE_WIDTH + SMILEY_WIDTH

#define TIMER_MAX_VALUE     99      // Max. game duration in sec.

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
    SMILEY_WIN, SMILEY_LOSE, SMILEY_CAUTION, SMILEY_HAPPY
} SMILEY_STATE;

// A viewport - defines visible part of the grid
//
typedef struct __viewPort{
    DIMS dimensions;        // max. box count (w x h)
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
    GAME_STATE gameState;
    SMILEY_STATE smileyState;
    int8_t minesLeft;   // could be < 0 !
    uint8_t steps;
    uint16_t time;
    RECT playgroundRect;
    RECT gridRect;
    RECT statRect;
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
//  @update : update the screen ?
//
void board_drawEx(PBOARD const board, BOOL update);
#define board_draw(board) board_drawEx(board, TRUE)
#define board_update(board) board_drawEx(board, TRUE)

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

// board_Pos2Point() : Get Box coordinates in the current viewport
//
//  @board : pointer to the board
//  @pos : Coordindate of the box in the grid
//  @pt : Pointer to the position in screen corrdinates
//
//  @return TRUE if doone successfully
//
BOOL board_Pos2Point(PBOARD const board, PCOORD const pos, PPOINT pt);

//  board_drawGridEx() : Draw the visible grid
//
//  @board : Pointer to the board
//  @update : if TRUE screen will be updated after drawing
//
void board_drawGridEx(PBOARD const board, BOOL update);
#define board_drawGrid(board) board_drawGridEx(board, TRUE)

//  board_DrawMinesLeftEx() : Draw the number of mines left
//
//  This number can be negative when the user 'puts' to many flags
//
//  @board : Pointer to the board
//  @update : if TRUE screen will be updated after drawing
//
void board_drawMinesLeftEx(PBOARD const board, BOOL update);
#define board_drawMinesLeft(board) board_drawMinesLeftEx(board, TRUE)

//  board_drawTimeEx() : Draw elapsed time
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

//  board_drawBoxAtPos() : Draw the box at a given position
//
//  @board : Pointer to the board
//  @pos : Box coordinates in the grid
//
void board_drawBoxAtPos(PBOARD const board, PCOORD const pos);

// board_drawViewPortButtonsEx() : Draw buttons for viewport scrolling
//
//  @board : pointer to the board
//  @highLight : Draw buttons in hightlighted state
//  @update : Update screen ?
//
void board_drawViewPortButtonsEx(PBOARD board, BOOL highLight, BOOL update);
#define board_drawViewPortButtons(board, highLight) board_drawViewPortButtonsEx(board, highLight, TRUE)

// board_drawLed() : Draw a led digit
//
//  Draw a led digit at the given position (no rotation)
//
//  @board : Pointer to the board
//  @digit : value to draw
//  @pos : Position in screen coordinates
//
void board_drawLed(PBOARD board, uint8_t digit, PRECT pos);

// board_drawBorder() : Draw a border
//
//  Draw a 3-d border with the specified thickness around a rect
//  borders are outside the rectangle
//
//  @board : Pointer to the board
//  @rect : Rect. whose borders will be drawn
//  @thickness : Border thickness
//
void board_drawBorder(PBOARD board, PRECT const rect, uint8_t thickness);

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

//  board_selectBoxEx() : Select a box
//
//  @board : Pointer to the board
//  @pos : Box coordinates of the box in the grid
//  @select : TRUE if box is selected, FALSE if unselected
//
void board_selectBoxEx(PBOARD const board, PCOORD const pos, BOOL select);
#define  board_selectBox(board, pos) board_selectBoxEx(board, pos, TRUE)
#define  board_unselectBox(board, pos) board_selectBoxEx(board, pos, FALSE)

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
