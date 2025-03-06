//----------------------------------------------------------------------
//--
//--    board.c
//--
//--            A game board
//--
//----------------------------------------------------------------------

#include "board.h"
#include "grid.h"
#include <stdint.h>

#ifdef DEST_CASIO_CALC
#include "consts.h"
#include "shared/menu.h"
#endif // #ifdef DEST_CASIO_CALC

#include <string.h>

#ifdef DEST_CASIO_CALC
    // Images
    extern bopti_image_t g_boxes;
    extern bopti_image_t g_smileys;
    extern bopti_image_t g_leds;
    extern bopti_image_t g_viewport;
#endif // #ifndef DEST_CASIO_CALC

//  board_create() : Create an empty board
//
//  @return : Pointer to the board
//
PBOARD board_create(){
    size_t size = sizeof(BOARD);
    PBOARD board = (PBOARD)malloc(size);
    if (!board){
        return NULL;
    }

    // Board is empty !
    memset(board, 0, size);
    board->grid = grid_create();
    return board;
}

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
    SET_RECT(board->viewPort.visibleFrame, 0, 0, BEGINNER_COLS, BEGINNER_ROWS);
    board->viewPort.dimensions.x = board->grid->cols;
    board->viewPort.dimensions.y = board->grid->rows;
    board->fullGrid = (LEVEL_BEGINNER == level);

    board_setOrientation(board, board->orientation);

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
#ifdef DEST_CASIO_CALC
        drect(0, 0, CASIO_WIDTH - 1, CASIO_HEIGHT - MENUBAR_DEF_HEIGHT - 1, COL_BKGROUND);
#endif // #ifdef DEST_CASIO_CALC

        board_drawTime(board);
        board_drawSmiley(board);
        board_drawGridEx(board, TRUE);  // + update
    }
}

//  board_drawGridEx() : Draw the visible grid
//
//  @board : Pointer to the board
//  @update : if TRUE screen will be updated after drawing
//
void board_drawGridEx(PBOARD const board, BOOL update){
    uint16_t origin;
    RECT rect;
    POINT offsetCol, offsetRow;

    if (!board || !board->grid){
        return;
    }

    SET_RECT(rect, board->gridPos.x, board->gridPos.y, BOX_WIDTH, BOX_HEIGHT);

    if (CALC_HORIZONTAL == board->orientation){
        rotateRect(&rect);

        offsetCol.x = 0;
        offsetCol.y = -1 * BOX_WIDTH;

        offsetRow.x = BOX_WIDTH;
        offsetRow.y = 0;

        origin = rect.y;
    }
    else{
        offsetCol.x = BOX_WIDTH;
        offsetCol.y = 0;

        offsetRow.x = 0;
        offsetRow.y = BOX_WIDTH;

        origin =rect.x;
    }

    for (uint8_t r=0; r < board->viewPort.dimensions.y; r++){
        // aligned with first "col"
        if (CALC_HORIZONTAL == board->orientation){
            rect.y = origin;
        }
        else{
            rect.x = origin;
        }

        for (uint8_t c=0; c < board->viewPort.dimensions.x; c++){
            board_drawBoxEx(board, r + board->viewPort.visibleFrame.y, c + board->viewPort.visibleFrame.x, rect.x, rect.y);
            OFFSET_RECT(rect, offsetCol.x, offsetCol.y);
        }

        OFFSET_RECT(rect, offsetRow.x, offsetRow.y);
    }

    if (!board->fullGrid){
        board_drawViewPortButtons(board);
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
//  @update : if TRUE screen will be updated after drawing
//
void board_drawTimeEx(PBOARD const board, BOOL update){
    if (board){
        uint16_t value = board->uTime;
        RECT rect;
        int8_t ox, oy;

        SET_RECT(rect, board->timerPos.x, board->timerPos.y, LED_WIDTH, LED_HEIGHT);

        if (CALC_HORIZONTAL == board->orientation){
            rotateRect(&rect);
            ox = 0;
            oy = -1 * LED_WIDTH;
        }
        else{
            ox = LED_WIDTH;
            oy = 0;
        }

        // 3 digits - total in [0, 1000[
        board_drawLed(board, (uint8_t)(value/100), &rect);
        OFFSET_RECT(rect, ox, oy);
        board_drawLed(board, (value %= 100)/10, &rect);
        OFFSET_RECT(rect, ox, oy);
        board_drawLed(board,value % 10, &rect);

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
//  @update : if TRUE screen will be updated after drawing
//
void board_drawSmileyEx(PBOARD const board, BOOL update){
    if (board){
        if (CALC_HORIZONTAL == board->orientation){
            RECT rect;
            SET_RECT(rect, board->smileyPos.x, board->smileyPos.y, SMILEY_WIDTH, SMILEY_WIDTH);
            rotateRect(&rect);

#ifdef DEST_CASIO_CALC
            dsubimage(rect.x, rect.y, &g_smileys, SMILEY_WIDTH, board->smileyState * SMILEY_HEIGHT, SMILEY_WIDTH, SMILEY_HEIGHT, DIMAGE_NOCLIP);
#endif // #ifdef DEST_CASIO_CALC
        }
        else{
#ifdef DEST_CASIO_CALC
            dsubimage(board->smileyPos.x, board->smileyPos.y, &g_smileys, 0, board->smileyState * SMILEY_HEIGHT, SMILEY_WIDTH, SMILEY_HEIGHT, DIMAGE_NOCLIP);
#endif // #ifdef DEST_CASIO_CALC
        }

        if (update){
#ifdef DEST_CASIO_CALC
            dupdate();
#endif // #ifdef DEST_CASIO_CALC
        }
    }
}

//  board_drawBoxEx() : Draw a single box
//
//      These 2 functions draw a given box.
//      board_drawBox checks wether a rotation needs to be done
//      whereas board_drawBoxEx assumes rotation has been done by the calling function
//
//  @board : Pointer to the board
//  @row, @col : Box coordinates in the grid
//  @dx, @dy : Screen coordinates of the top-left corner
//
void board_drawBoxEx(PBOARD const board, uint8_t row, uint8_t col, uint16_t dx, uint16_t dy){
    if (board){
        PBOX box = GRID_AT(board->grid, row, col);

#ifdef DEST_CASIO_CALC
        dsubimage(dx, dy, &g_boxes, board->orientation * BOX_WIDTH, box->state * BOX_HEIGHT, BOX_WIDTH, BOX_HEIGHT, DIMAGE_NOCLIP);
#endif // #ifdef DEST_CASIO_CALC
    }
}

void board_drawBox(PBOARD const board, uint8_t row, uint8_t col, uint16_t dx, uint16_t dy){
    if (board){
        if (CALC_HORIZONTAL == board->orientation){
            RECT rect = {dx, dy, dx + BOX_WIDTH - 1, dy + BOX_HEIGHT - 1};
            rotateRect(&rect);
            board_drawBox(board, row, col, rect.x, rect.y);
        }
        else{
            board_drawBox(board, row, col, dx, dy);
        }
    }
}

// board_drawViewPortButtonsEx() : Draw buttons for viewport scrolling
//
//  @board : pointer to the board
//  @highLight : Draw buttons in hightlighted state
//
void board_drawViewPortButtonsEx(PBOARD board, BOOL highLight){
    if (board){
        uint8_t sequence[4];    // Img IDs
        BOOL showButton;
        if (CALC_HORIZONTAL == board->orientation){
            memcpy(sequence, (uint8_t[]) {3, 0, 1, 2}, 4 * sizeof(uint8_t));
        }
        else{
            memcpy(sequence, (uint8_t[]) {0, 1, 2, 3}, 4 * sizeof(uint8_t));
        }

        for (uint8_t id=0; id<4; id++){
            // is the button visible ?
            switch (id){
                // top
                case 0:
                    showButton = (board->viewPort.visibleFrame.y>0);
                    break;

                // right
                case 1:
                    showButton = ((board->viewPort.visibleFrame.x + board->viewPort.visibleFrame.w) >= board->viewPort.dimensions.x);
                    break;

                // bottom
                case 2:
                    showButton = ((board->viewPort.visibleFrame.y + board->viewPort.visibleFrame.h) >= board->viewPort.dimensions.y);
                    break;

                // left
                case 3:
                    showButton = (board->viewPort.visibleFrame.x>0);
                    break;
            } // switch

            // Show / hide the button
            if (highLight || (!highLight && !showButton)){
#ifdef DEST_CASIO_CALC
                drect(
                    board->viewPort.navButtons[0].x, board->viewPort.navButtons[0].y,
                    board->viewPort.navButtons[0].x + board->viewPort.navButtons[0].w - 1,
                    board->viewPort.navButtons[0].y + board->viewPort.navButtons[0].h - 1,
                    showButton?C_INVERT:COL_BKGROUND);
#endif // #ifdef DEST_CASIO_CALC
            }
            else{
#ifdef DEST_CASIO_CALC
                dsubimage(
                    board->viewPort.navButtons[0].x, board->viewPort.navButtons[0].y,
                    &g_viewport, 0, id * GRID_VIEWPORT_BUTTON_WIDTH,
                    GRID_VIEWPORT_BUTTON_WIDTH, GRID_VIEWPORT_BUTTON_HEIGHT,
                   DIMAGE_NOCLIP);
#endif // #ifdef DEST_CASIO_CALC
            }
        } // if
    }
}

// board_drawLed() : Draw a led digit
//
//  Draw a led digit at the given position (no rotation)
//
//  @board : Pointer to the board
//  @digit : value to draw
//  @pos : Position in screen coordinates
//
void board_drawLed(PBOARD board, uint8_t digit, PRECT pos){
#ifdef DEST_CASIO_CALC
    dsubimage(pos->x, pos->y, &g_leds, board->orientation * LED_WIDTH, (11-digit)*pos->h, pos->w, pos->h, DIMAGE_NOCLIP);
#endif // #ifdef DEST_CASIO_CALC
}

//  board_changeOrientation() : Change drawing orientation
//
//  Change display orientation and update screen
//
//  @board : Pointer to the board
//
//  @return TRUE if done
//
BOOL board_changeOrientation(PBOARD const board){
    if (board){
        board_setOrientation(board, (CALC_VERTICAL == board->orientation)?CALC_HORIZONTAL:CALC_VERTICAL);
        board_update(board);
        return TRUE;
    }

    return FALSE;
}

//  board_setOrientation() : Set drawing orientation
//
//  @board : Pointer to the board
//  @orientation : Drawing orientation
//
void board_setOrientation(PBOARD const board, CALC_ORIENTATION orientation){
    if (board){
        board->orientation = orientation;

        // Grid position
        SET_RECT(board->gridPos,
            GRID_VIEWPORT_LEFT + GRID_VIEWPORT_BUTTON_WIDTH,
            GRID_VIEWPORT_TOP + GRID_VIEWPORT_BUTTON_WIDTH,
            BEGINNER_COLS * BOX_WIDTH,
            BEGINNER_ROWS * BOX_HEIGHT);

        // Viewport navigation buttons
        // top button
        SET_RECT(board->viewPort.navButtons[0],
            board->gridPos.x + (board->gridPos.w - GRID_VIEWPORT_BUTTON_WIDTH) / 2,
            board->gridPos.y - GRID_VIEWPORT_BUTTON_HEIGHT,
            GRID_VIEWPORT_BUTTON_WIDTH, GRID_VIEWPORT_BUTTON_HEIGHT);

        // right btton
        SET_RECT(board->viewPort.navButtons[1],
            board->gridPos.x + board->gridPos.w,
            board->gridPos.y + (board->gridPos.h - GRID_VIEWPORT_BUTTON_HEIGHT) / 2,
            GRID_VIEWPORT_BUTTON_WIDTH, GRID_VIEWPORT_BUTTON_HEIGHT);

        // bottom button
        SET_RECT(board->viewPort.navButtons[2],
            board->viewPort.navButtons[0].x,
            board->gridPos.y + board->gridPos.h,
            GRID_VIEWPORT_BUTTON_WIDTH, GRID_VIEWPORT_BUTTON_HEIGHT);

        // left btton
        SET_RECT(board->viewPort.navButtons[3],
            board->gridPos.x - GRID_VIEWPORT_BUTTON_WIDTH,
            board->viewPort.navButtons[1].y,
            GRID_VIEWPORT_BUTTON_WIDTH, GRID_VIEWPORT_BUTTON_HEIGHT);
    }
}

//
// Tools
//

//  rotatePoint() : Rotate a single point (trig. 90°)
//
//  @pos : Pointer to point coordinates
//
void rotatePoint(PPOINT const pos){
    int16_t ny = CASIO_HEIGHT - pos->x;
    pos->x = pos->y;
    pos->y = ny;
}

//  rotateRect() : Rotate a rectangle (trig. 90°)
//
//  @rect : Pointer to the rect
//
void rotateRect(PRECT const rect){
    // rect is valid !!!!
    // assert(rect)

    POINT topLeft = {rect->x, rect->y};
    POINT bottomRight = {rect->x + rect->w - 1, rect->y + rect->h - 1};

    rotatePoint(&topLeft);
    rotatePoint(&bottomRight);

    rect->x = topLeft.x;
    rect->y = bottomRight.y;
    rect->w = bottomRight.x - rect->x + 1;
    rect->h = topLeft.y - rect->y + 1;
}

// EOF
