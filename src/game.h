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
BOOL _onStartGame(PBOARD const board);

// _onPause() : Show pause screen
//
void _onPause();

// _onKeyLeftEx() : User press "left" key
//
//  @board : pointer to the game board
//  @pos : position of cursor
//  @check : if TRUE check display orientation
//
//  @return : TRUE if position changed
//
BOOL _onKeyLeftEx(PBOARD const board, PCOORD pos, BOOL check);
#define _onKeyLeft(board, pos) _onKeyLeftEx(board, pos, TRUE)

// _onKeyDownEx() : User press "down" key
//
//  @board : pointer to the game board
//  @pos : position of cursor
//  @check : if TRUE check display orientation
//
//  @return : TRUE if position changed
//
BOOL _onKeyDownEx(PBOARD const board, PCOORD pos, BOOL check);
#define _onKeyDown(board, pos) _onKeyDownEx(board, pos, TRUE)

// _onKeyRightEx() : User press "right" key
//
//  @board : pointer to the game board
//  @pos : position of cursor
//  @check : if TRUE check display orientation
//
//  @return : TRUE if position changed
//
BOOL _onKeyRightEx(PBOARD const board, PCOORD pos, BOOL check);
#define _onKeyRight(board, pos) _onKeyRightEx(board, pos, TRUE)

// _onKeyUpEx() : User press "up" key
//
//  @board : pointer to the game board
//  @pos : position of cursor
//  @check : if TRUE check display orientation
//
//  @return : TRUE if position changed
//
BOOL _onKeyUpEx(PBOARD const board, PCOORD pos, BOOL check);
#define _onKeyUp(board, pos) _onKeyUpEx(board, pos, TRUE)

// _updateMenuItemsStates() : Update state of items in the menu
//
//  @board : pointer to the game board
//  @menu : Pointer to the menu
//  @pos : position of cursor
//
void _updateMenuItemsStates(PBOARD const board, POWNMENU menu, PCOORD pos);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // #ifndef __GEE_MINES_GAME_h__

// EOF
