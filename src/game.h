//----------------------------------------------------------------------
//--
//--    game.h
//--
//--            The game !
//--
//----------------------------------------------------------------------

#ifndef __GEE_MINES_GAME_h__
#define __GEE_MINES_GAME_h__    1

#include "consts.h"
#include "shared/menu.h"
#include "board.h"

// Blinking
//
#define TIMER_TICK_DURATION     100 // Basic 'tick' in ms
#define TIMER_SECOND            10  // a sec. in 'tick' count
#define TIMER_HALF_SECOND       5

#define BLINK_CURSOR            TIMER_HALF_SECOND   // "duration" of cursor blinking

// Redraw mode (any combinaison of)
//
#define NO_REDRAW               0
#define REDRAW_TIME             1
#define REDRAW_SELECTION        2
#define REDRAW_NAV_BUTTONS      4       // viewport navigation buttons
#define REDRAW_BOX              8       // Box at current and prev. positions
#define REDRAW_GRID             16

#ifdef DEST_CASIO_CALC
#include <gint/timer.h>
#endif // #ifdef DEST_CASIO_CALC

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

// _onNewGame() : Create a new game
//
//  @baord : pointer to the game board
//  @level : game difficulty level
//
void _onNewGame(PBOARD const board, uint8_t level);

// _createGameMenu : Create the game menu
//
//  return : Pointer the the menu (or NULL on error)
//
POWNMENU _createGameMenu();

// _onStartGame() : Start a new game
//
//  @board : pointer to the game board
//
//  @return : FALSE on error
//
#ifdef DEST_CASIO_CALC
BOOL _onStartGame(PBOARD const board);
#endif // #ifdef DEST_CASIO_CALC

// _onPause() : Show pause screen
//
void _onPause();

// _onKeyLeftEx() : User press "left" key
//
//  @board : pointer to the game board
//  @pos : position of cursor
//  @check : if TRUE check display orientation
//
//  @return : Drawing action(s) to perform or NO_REDRAW
//
uint8_t _onKeyLeftEx(PBOARD const board, PCOORD pos, BOOL check);
#define _onKeyLeft(board, pos) _onKeyLeftEx(board, pos, TRUE)

// _onKeyDownEx() : User press "down" key
//
//  @board : pointer to the game board
//  @pos : position of cursor
//  @check : if TRUE check display orientation
//
//  @return : Drawing action(s) to perform or NO_REDRAW
//
uint8_t _onKeyDownEx(PBOARD const board, PCOORD pos, BOOL check);
#define _onKeyDown(board, pos) _onKeyDownEx(board, pos, TRUE)

// _onKeyRightEx() : User press "right" key
//
//  @board : pointer to the game board
//  @pos : position of cursor
//  @check : if TRUE check display orientation
//
//  @return : Drawing action(s) to perform or NO_REDRAW
//
uint8_t _onKeyRightEx(PBOARD const board, PCOORD pos, BOOL check);
#define _onKeyRight(board, pos) _onKeyRightEx(board, pos, TRUE)

// _onKeyUpEx() : User press "up" key
//
//  @board : pointer to the game board
//  @pos : position of cursor
//  @check : if TRUE check display orientation
//
//  @return : Drawing action(s) to perform or NO_REDRAW
//
uint8_t _onKeyUpEx(PBOARD const board, PCOORD pos, BOOL check);
#define _onKeyUp(board, pos) _onKeyUpEx(board, pos, TRUE)

// _updateMenuItemsStates() : Update state of items in the menu
//
//  @board : pointer to the game board
//  @menu : Pointer to the menu
//  @pos : position of cursor
//
void _updateMenuItemsStates(PBOARD const board, POWNMENU menu, PCOORD pos);

#ifdef DEST_CASIO_CALC
// __callbackTick() : Call back function for timer
// This function is used during edition to make selected item blink
//
//  @pTick : pointer to blinking state indicator
//
//  @return : TIMER_CONTINUE if valid
//
//static int __callbackTick(volatile int *pTick);
#endif // #ifdef DEST_CASIO_CALC

//
// Utils
//

#ifdef TRACE_MODE

// __coordtoa() : Format a CCORD struct to an output string
//
//  This specific method creates a string composed of the name of the value
//  and the value it self. It is equivalent to a sprintf(out, "%s : %d", name, value)
//
//  The base can't be changed it is always equal to 10
//
//  This method assumes the output buffer - ie. str - is large enough to contain
//  the name and the formated value.
//
//  @name : Name of the value (can be NULL)
//  @pos : Position to show
//  @str : Pointer to output string
//
//  @return : pointer to formated string
//
char* __corrdtoa(const char* name, PCOORD const pos, char* str);

// __atoi() : Convert a num. val to a string
//
//  @num : Numeric value to convert
//  @str : String to reverse
//
//  @return : a pointer to the string
//
char* __atoi(int num, char *str);

// __strrev() : Reverse a string
//
//  @str : String to reverse
//
void __strrev(char *str);

#endif // TRACE_MODE

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // #ifndef __GEE_MINES_GAME_h__

// EOF
