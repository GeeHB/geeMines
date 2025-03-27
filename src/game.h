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
#include "board.h"
#include "scores.h"

#ifdef DEST_CASIO_CALC
#include <gint/timer.h>
#endif // #ifdef DEST_CASIO_CALC

// Blinking
//
#define TIMER_TICK_DURATION     100 // Basic 'tick' in ms
#define TIMER_SECOND            10  // a sec. in 'tick' count
#define TIMER_HALF_SECOND       5

#define BLINK_CURSOR            TIMER_HALF_SECOND   // "duration" of cursor blinking
//#define BLINK_SCROLLBARS        12

// Redraw mode (any combinaison of)
//
#define NO_REDRAW               0
#define REDRAW_MINES_LEFT       1
#define REDRAW_MOVE             2
#define REDRAW_TIME             4
#define REDRAW_BOX              8       // Redraw current box and menu
#define REDRAW_SELECTION        16
//#define REDRAW_SCROLLBARS       32
#define REDRAW_GRID             64

#define REDRAW_UPDATE           128     // Just update

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

// _onNewGame() : Create a new game
//
//  @board : pointer to the game board
//  @level : game difficulty level
//
void _onNewGame(PBOARD const board, uint8_t level);

// _onAbout() : Show "about" informations
//
void _onAbout();

// _onPause() : Show pause screen
//
void _onPause();

// _onStartGame() : Start a new game
//
//  @board : pointer to the game board
//  @scores : Array of scores
//
//  @return : FALSE on error
//
BOOL _onStartGame(PBOARD const board, PSCORE scores);

//  _onStep : User steps on a box
//
//  @board : Pointer to the board
//  @pos : Position of the box
//  @redraw : pointer to the redraw indicator
//
//  @return : FALSE if stepped on a mine
//
BOOL _onStep(PBOARD const board, PCOORD const pos, uint16_t* redraw);

// _onFlag() : Put / remove a flag
//
//  @board : pointer to the current board
//  @pos : Current position in the grid
//
//  @return drawing action to perform or NO_DRAWING
//
uint16_t _onFlag(PBOARD const board, PCOORD const pos);

// _onQuestion() : Put / remove a 'question' attribute to the box
//
//  @board : pointer to the current board
//  @pos : Current position in the grid
//
//  @return drawing action to perform or NO_DRAWING
//
uint16_t _onQuestion(PBOARD const board, PCOORD const pos);

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

// _gameWon() : The user won the game
//
//  @board : pointer to the game board
//  @scores : Array of scores
//  @level, @time : new score
//
void _gameWon(PBOARD const board, PSCORE scores, uint8_t level, int time);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // #ifndef __GEE_MINES_GAME_h__

// EOF
