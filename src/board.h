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

// Positions et dimensions
//

#define EMPTY_SPACE         0x0002
#define PLAYGROUND_BORDER   0x0003
#define STAT_BORDER         0x0003

#define BOX_WIDTH           0x0010  // 16
#define BOX_HEIGHT          BOX_WIDTH

#define SMILEY_WIDTH        0x0018  // 24
#define SMILEY_HEIGHT       SMILEY_WIDTH

#define LED_WIDTH           0x000D  // 13
#define LED_HEIGHT          0x0017  // 23
#define LED_ZONE_WIDTH      3 * LED_WIDTH

// Counts of buttons when scroll buttons are in place
//

#define BUTTON_VERT_COL_MAX    22     // # max of button in vert. mode
#define BUTTON_VERT_ROW_MAX    10

#define BUTTON_HORZ_COL_MAX    12     // # max of button in horz. mode
#define BUTTON_HORZ_ROW_MAX    21

// LED images
//
#define SMILEY_OFFSET_V     LED_ZONE_WIDTH
#define STAT_WIDTH          (6*LED_WIDTH + SMILEY_WIDTH)
#define STAT_HEIGHT         SMILEY_HEIGHT

// LED digits IDs in the leds bitmap in reverse order
#define LED_MINUS_ID        11
#define LED_EMPTY_ID        10

// Time
//
#define TIMER_OFFSET_V      LED_ZONE_WIDTH + SMILEY_WIDTH

#define TIMER_MAX_VALUE     999      // Max. game duration in sec.

// Game state
//
typedef enum {
    STATE_WAITING, STATE_PLAYING, STATE_WON, STATE_LOST, STATE_CANCELLED
} GAME_STATE;

// Orientation
//
typedef enum {
    CALC_VERTICAL = 0, CALC_HORIZONTAL
} CALC_ORIENTATION;

// Smiley state
//
typedef enum {
    SMILEY_WIN, SMILEY_LOSE, SMILEY_CAUTION, SMILEY_HAPPY
} SMILEY_STATE;

// Scrollbar management
//
#define NO_SCROLL       0
#define HORZ_SCROLL     1
#define VERT_SCROLL     2
#define BOTH_SCROLL     (HORZ_SCROLL | VERT_SCROLL)

#define SCROLL_RADIUS   0x0005
#define SCROLL_WIDTH    2*(SCROLL_RADIUS+1)      // 12
#define SCROLL_HEIGHT   SCROLL_WIDTH

#define SCROLL_COLOUR           C_RGB(19,24,27)
#define SCROLL_COLOUR_HILITE    C_RGB(13, 16, 18)

// A viewport - defines visible part of the grid
//
typedef struct __viewPort{
    DIMS dimensions;        // max. box count (w x h)
    RECT visibleFrame;      // current visible boxes IDs
    uint8_t scrolls:2;
    RECT scrollBars[2];     // 0=>horz , 1=>vert
}VIEWPORT, * PVIEWPORT;

// Game board
//
typedef struct __board{
    PGRID grid;
    VIEWPORT viewPort;
    CALC_ORIENTATION orientation;
    GAME_STATE gameState;
    SMILEY_STATE smileyState;
    int8_t minesLeft;   // could be < 0 !
    uint8_t steps;
    uint16_t time;
    RECT playgroundRect;
    RECT gridRect;
    RECT statRect;
#ifdef _DEBUG_
    BOOL debug;
#endif // #ifdef _DEBUG_
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
//  @menu : Meu on the screen ?
//  @update : update the screen ?
//
void board_drawEx(PBOARD const board, BOOL menu, BOOL update);
#define board_draw(board, menu) board_drawEx(board, menu, TRUE)
#define board_update(board, menu) board_drawEx(board, menu, TRUE)

//  board_setGameStateEx() : Change the state of a game
//
//  @board : Pointer to the board
//  @state : new game state
//  @redraw : update screen
//
void board_setGameStateEx(PBOARD const board, GAME_STATE state, BOOL redraw);
#define board_setGameState(board, state) board_setGameStateEx(board, state, FALSE)

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

// board_isBoxVisible() : Check if box is visible
//
// Check that the box at the given position is in the viewPort visible frame
//
//  @board : pointer to the board
//  @pos : Coordindate of the box in the grid
//
//  @return TRUE the box is visible
//
BOOL board_isBoxVisible(PBOARD const board, PCOORD const pos);

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

//  board_drawBox() : Draw a single box
//
//      These 2 functions draw a given box.
//      board_drawBox checks wether a rotation needs to be done
//      whereas board_directDrawBox assumes rotation has been done by the calling function
//
//  @board : Pointer to the board
//  @pos : Box coordinates in the grid
//  @dx, @dy : Screen coordinates of the top-left corner
//
void board_drawBox(PBOARD const board, PCOORD const pos, uint16_t dx, uint16_t dy);
void board_directDrawBox(PBOARD const board, PCOORD const pos, uint16_t dx, uint16_t dy);

//  board_drawBoxAtPos() : Draw the box at a given position
//
//  @board : Pointer to the board
//  @pos : Box coordinates in the grid
//
void board_drawBoxAtPos(PBOARD const board, PCOORD const pos);

// board_drawScrollBarsEx() : Draw viewport's scrollbars
//
//  @board : pointer to the board
//  @highLight : Highlight scroll bars ?
//
void board_drawScrollBars(PBOARD board, BOOL highLight);

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
//  Draw a 3D border with the specified thickness around a rect
//  borders are outside the rectangle
//
//  @orientation : Rectangle's orientation
//  @rect : Rect. whose borders will be drawn
//  @thickness : Border thickness
//
void board_drawBorder(CALC_ORIENTATION orientation, PRECT const rect, uint8_t thickness);

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
