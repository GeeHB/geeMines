//----------------------------------------------------------------------
//--
//--    board.c
//--
//--            A game board
//--
//----------------------------------------------------------------------

#include "board.h"
#include "grid.h"

#ifdef DEST_CASIO_CALC
    // Images
    extern bopti_image_t g_boxes;
    extern bopti_image_t g_smileys;
    extern bopti_image_t g_leds;
#endif // #ifndef DEST_CASIO_CALC

//  board_init() : Initializes the board
//
//  @board : Pointer to the board
//  @level : Difficulty of the new board
//  @orientation : Defines display orientation
//
//  @return : TRUE if done
//
BOOL board_init(PBOARD const board, GAME_LEVEL level, ORIENTATION orientation){
    if (!board || !grid_init(board->grid, level)){
        return FALSE;
    }

    board->orientation = orientation;

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
    board->smileyState = SMILEY_HAPPY;
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

//
// Drawings
//

//  board_draw() : Draw the whole board
//
//  @board : Pointer to the board
//
void board_draw(PBOARD const board){
    if (board){
        board_drawTime(board);
        board_drawSmiley(board);
        board_drawGridEx(board, TRUE);  // + update
    }
}

//  board_drawGridEx() : Draw the visible grid
//
//  @board : Pointer to the board
//  @update : if TRUE screen will ba updated after drawing
//
void board_drawGridEx(PBOARD const board, BOOL update){
    uint16_t dx,dy;

    if (!board || !board->grid){
        return;
    }

    dy = board->gridRect.top;
    for (uint8_t r=0; r < board->viewPort.dimensions.y; r++){
        dx = board->gridRect.left;
        for (uint8_t c=0; c < board->viewPort.dimensions.x; c++){
            board_drawBox(board, r + board->viewPort.visibleFrame.top, c + board->viewPort.visibleFrame.left, dx, dy);
            dx+=BOX_WIDTH;
        }
        dy+=BOX_HEIGHT;
    }

    if (update){
#ifdef DEST_CASIO_CALC
        dupdate();
#endif // #ifdef DEST_CASIO_CALC
    }
}

//  board_drawTimeEx() : Draw time
//
//  @board : Pointer to the board
//  @update : if TRUE screen will ba updated after drawing
//
void board_drawTimeEx(PBOARD const board, BOOL update){
    if (board){

        if (update){
    #ifdef DEST_CASIO_CALC
            dupdate();
    #endif // #ifdef DEST_CASIO_CALC
        }
    }
}

//  board_drawSmileyEx() : Draw the smiley
//
//  @board : Pointer to the board
//  @update : if TRUE screen will ba updated after drawing
//
void board_drawSmileyEx(PBOARD const board, BOOL update){
    if (board){
#ifdef DEST_CASIO_CALC
        dsubimage(board->smileyPos.x, board->smileyPos.y, &g_smileys, 0, board->smileyState * SMILEY_HEIGHT, SMILEY_WIDTH, SMILEY_HEIGHT, DIMAGE_NOCLIP);
#endif // #ifdef DEST_CASIO_CALC

        if (update){
    #ifdef DEST_CASIO_CALC
            dupdate();
    #endif // #ifdef DEST_CASIO_CALC
        }
    }
}

//  board_drawBox() : Draw a single box
//
//  @board : Pointer to the board
//  @row, @col : Box coordinates
//  @dx, @dy : Screen coordinates of the top-left corner
//
void board_drawBox(PBOARD const board, uint8_t row, uint8_t col, uint16_t dx, uint16_t dy){
    if (board){
        PBOX box = GRID_AT(board->grid, row, col);

#ifdef DEST_CASIO_CALC
        dsubimage(dx, dy, &g_boxes, 0, box->state * BOX_HEIGHT, BOX_WIDTH, BOX_HEIGHT, DIMAGE_NOCLIP);
#endif // #ifdef DEST_CASIO_CALC
    }
}

// EOF
