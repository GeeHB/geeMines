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
#define BOX_HEIGHT          16

// mine defines
#define LED_WIDTH           13
#define LED_HEIGHT          23
#define FACE_WIDTH          24
#define FACE_HEIGHT         24

// Game state
//
typedef enum {
    WAITING, PLAYING, GAMEWON, GAMELOST
    } GAME_STATE;

// Smiley state
//
typedef enum {
    FACE_DOWN, FACE_WIN, FACE_LOSE, FACE_CAUTION, FACE_HAPPY
    } FACE_STATE;

// Game board
//
typedef struct
{
    PGRID grid;
    POINT viewPort;

    RECT StatRect;
    RECT CounterRect;
    RECT FaceRect;
    RECT TimerRect;
    RECT GridRect;

    GAME_STATE gameState;
    FACE_STATE faceState;

    uint8_t uMinesLeft;
    uint8_t uMines;
    uint16_t uTime;
} BOARD, * PBOARD;


//
// Functions prototypes
//

//  board_create() : Create an empty board
//
//  return : Pointer to the board
//
PBOARD board_create();

//  board_free() : Free a board
//
//  @board : Pointer to the board
//
void board_free(PBOARD const board);


//  board_display() : Display the whole board (grid & stats)
//
//  @board : Pointer to the board
//
void board_display(PBOARD const board);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // #ifndef __GEE_MINES_BOARD_h__

// EOF
