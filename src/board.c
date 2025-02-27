//----------------------------------------------------------------------
//--
//--    board.c
//--
//--            A game board
//--
//----------------------------------------------------------------------

#include "board.h"
#include "grid.h"

//  board_init() : Initializes the board
//
//  @board : Pointer to the board
//  @level : Difficulty of the new board
//
//  @return : TRUE if done
//
BOOL board_init(PBOARD const board, GAME_LEVEL level){
    if (!board || !grid_init(board->grid, level)){
        return FALSE;
    }

    // Initial viewport
    SET_SRECT_DIMS(board->viewPort.visibleFrame, 0, 0, BEGINNER_COLS, BEGINNER_ROWS);
    board->viewPort.dimensions.x = BEGINNER_COLS;   // Same size in all cases
    board->viewPort.dimensions.y = BEGINNER_ROWS;

    // Grid rectangle (it's a square)
    SET_SRECT_DIMS(board->gridRect,
        GRID_VIEWPORT_LEFT + GRID_VIEWPORT_BUTTON_WIDTH,
        GRID_VIEWPORT_TOP + GRID_VIEWPORT_BUTTON_WIDTH,
        BOX_WIDTH * BEGINNER_COLS,
        BOX_HEIGHT * BEGINNER_ROWS);

    // New game !
    board->gameState = STATE_WAITING;
    board->faceState = FACE_HAPPY;
    board->uMinesLeft = board->grid->minesCount;
    board->uMines = 0;
    board->uTime = 0;

    return TRUE;
}

//  board_free() : Free a board
//
//  @board : Pointer to the board
//  @freeAll : if FALSE only boxes are freed. If TRUE boxes and grid memory will
//             be freed
//
void board_free(PBOARD const board, BOOL freeAll){
    if (board){
        board->grid = grid_free(board->grid, TRUE);

        if (freeAll){
            free(board);
        }
    }
}

// EOF
