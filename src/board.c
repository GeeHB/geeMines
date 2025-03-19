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
#else
#include <stdio.h>
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

#ifdef _DEBUG_
    board->debug = TRUE;    // In debug mode, show mines by default !
#endif // #ifdef _DEBUG_

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

    grid_layMines(board->grid); // Put mines
    board_setOrientation(board, board->orientation);

    // New game !
    board_setGameStateEx(board, STATE_WAITING, FALSE);
    board->minesLeft = (int8_t)board->grid->mines;
    board->time = 0;
    board->steps = 0;

    return TRUE;
}

//  board_setSmileyEx() : Change the state of a game
//
//  @board : Pointer to the board
//  @smiley : new smiley to draw
//  @redraw : update screen
//
void board_setSmileyEx(PBOARD const board, SMILEY_STATE smiley, BOOL redraw){
    if (smiley != board->smileyState){
        board->smileyState = smiley;
        board_drawSmileyEx(board, redraw);
    }
}

//  board_setGameStateEx() : Change the state of a game
//
//  @board : Pointer to the board
//  @state : new game state
//  @redraw : update screen
//
void board_setGameStateEx(PBOARD const board, GAME_STATE state, BOOL redraw){
    if (state == board->gameState){
        return;
    }

    uint8_t r,c;
    BOOL redrawGrid = FALSE;

    board->gameState = state;

    switch (state){
        case STATE_WON:
        {
            PBOX box;
            board->minesLeft = 0;
            board_setSmileyEx(board, SMILEY_WIN, FALSE);
            for (r = 0; r < board->grid->size.row; r++)
                for (c = 0; c < board->grid->size.col; c++){
                    box = BOX_AT(board->grid, c, r);
                    if (box->mine && box->state != BS_FLAG){
                        box->state = BS_FLAG;
                    }
                }

            redrawGrid = TRUE;
            break;
        }

        case STATE_LOST:
        case STATE_CANCELLED:
        {
            PBOX box;
            board_setSmileyEx(board, SMILEY_LOSE, FALSE);
            for (r = 0; r < board->grid->size.row; r++)
                for (c = 0; c < board->grid->size.col; c++){
                    box = BOX_AT(board->grid, c, r);
                    if (box->mine && box->state != BS_BLAST){
                        box->state = BS_MINE;
                    }
                    else{
                        if (box->state == BS_FLAG  && !box->mine){
                            box->state = BS_WRONG;
                        }
                    }
                }

            redrawGrid = TRUE;
            break;
        }

        case STATE_WAITING:
        case STATE_PLAYING:
            board_setSmileyEx(board, SMILEY_HAPPY, redraw);
            break;
    } // switch

    if (redrawGrid){
        board_drawGridEx(board, redraw);
    }
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

// board_Pos2Point() : Get Box coordinates in the current viewport
//
//  @board : pointer to the board
//  @pos : Coordindate of the box in the grid
//  @pt : Pointer to the position in screen corrdinates
//
//  @return TRUE if doone successfully
//
BOOL board_Pos2Point(PBOARD const board, PCOORD const pos, PPOINT pt){
    if (board && pos && pt){
        pt->x = board->gridRect.x + BOX_WIDTH * (pos->col - board->viewPort.visibleFrame.x);
        pt->y = board->gridRect.y + BOX_HEIGHT * (pos->row - board->viewPort.visibleFrame.y);
        return TRUE;
    }

    return FALSE;
}

//  board_drawEx() : Draw the whole board
//
//  @board : Pointer to the board
//  @update : update the screen ?
//
void board_drawEx(PBOARD const board, BOOL update){
#ifdef DEST_CASIO_CALC
    drect(0, 0, CASIO_WIDTH - 1, CASIO_HEIGHT - MENUBAR_DEF_HEIGHT - 1, COL_BKGROUND);
#endif // #ifdef DEST_CASIO_CALC

    if (board->grid){
        // Stats
        board_drawBorder(board, &board->statRect, STAT_BORDER);
        board_drawMinesLeftEx(board, FALSE);
        board_drawSmileyEx(board, FALSE);
        board_drawTimeEx(board, FALSE);

        // Grid
        board_drawBorder(board, &board->playgroundRect, PLAYGROUND_BORDER);
        board_drawGridEx(board, update);  // + update
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
    COORD pos;
    uint8_t r, c;

    if (!board || !board->grid){
        return;
    }

    setRect(&rect, board->gridRect.x, board->gridRect.y, BOX_WIDTH, BOX_HEIGHT);

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

    for (r = 0; r < board->viewPort.visibleFrame.h; r++){
        // aligned with first "col"
        if (CALC_HORIZONTAL == board->orientation){
            rect.y = origin;
        }
        else{
            rect.x = origin;
        }

        for (c = 0; c < board->viewPort.visibleFrame.w; c++){
            pos = (COORD){.col = c + board->viewPort.visibleFrame.x , .row = r + board->viewPort.visibleFrame.y};

/*
#ifdef TRACE_MODE
            if (!r && !c){
                char trace[250];
                __coordtoa("Pos : ", rect.x, rect.y, trace);
                TRACE(trace, C_BLACK, COL_BKGROUND);
            }
#endif // TRACE_MODE
*/
            board_directDrawBox(board, &pos, rect.x, rect.y);
            offsetRect(&rect, offsetCol.x, offsetCol.y);
        }

        offsetRect(&rect, offsetRow.x, offsetRow.y);
#ifndef DEST_CASIO_CALC
        printf("|\n");       // EOL
#endif // #ifndef DEST_CASIO_CALC
    }

    if (!board->fullGrid){
        board_drawNavButtonsEx(board, FALSE, FALSE);
    }

    if (update){
#ifdef DEST_CASIO_CALC
        dupdate();
#endif // #ifdef DEST_CASIO_CALC
    }
}

//  board_DrawMinesLeftEx() : Draw the number of mines left
//
//  This number can be negative when the user 'puts' to many flags
//
//  @board : Pointer to the board
//  @update : if TRUE screen will be updated after drawing
//
void board_drawMinesLeftEx(PBOARD const board, BOOL update){
    int8_t value = board->minesLeft;
    uint8_t ids[3];
    BOOL negative = FALSE;
    RECT rect;
    int ox, oy;

    if (value < 0){
        value = -1 * value;
        negative = TRUE;
    }

    if (value >= 100){
        value = 99; // Should never be executed
    }

    setRect(&rect, board->statRect.x, board->statRect.y, LED_WIDTH, LED_HEIGHT);

    if (CALC_HORIZONTAL == board->orientation){
        rotateRect(&rect);
        ox = 0;
        oy = -1 * LED_WIDTH;
    }
    else{
        ox = LED_WIDTH;
        oy = 0;
    }

    // up to 3 digits - total in ]-100, 100[
    ids[0] = LED_EMPTY_ID;
    ids[1] = value / 10;  // dec.
    ids[2] = value % 10;  // units

    // A sign ?
    if (negative){
        ids[ids[1]?0:1] =LED_MINUS_ID;
    }

    for (uint8_t id = 0; id < 3; id++){
        board_drawLed(board, (uint8_t)ids[id], &rect);
        offsetRect(&rect, ox, oy);  // next pos
    }

    if (update){
#ifdef DEST_CASIO_CALC
        dupdate();
#endif // #ifdef DEST_CASIO_CALC
    }
}

//  board_drawTimeEx() : Draw elapsed time
//
//  @board : Pointer to the board
//  @update : if TRUE screen will be updated after drawing
//
void board_drawTimeEx(PBOARD const board, BOOL update){
    uint16_t value = board->time;
    RECT rect;
    int ox, oy;

    setRect(&rect, board->statRect.x + TIMER_OFFSET_V, board->statRect.y, LED_WIDTH, LED_HEIGHT);

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
    offsetRect(&rect, ox, oy);
    board_drawLed(board, (value %= 100)/10, &rect);
    offsetRect(&rect, ox, oy);
    board_drawLed(board,value % 10, &rect);

    if (update){
#ifdef DEST_CASIO_CALC
        dupdate();
#endif // #ifdef DEST_CASIO_CALC
    }
}

//  board_drawSmileyEx() : Draw the smiley
//
//  @board : Pointer to the board
//  @update : if TRUE screen will be updated after drawing
//
void board_drawSmileyEx(PBOARD const board, BOOL update){
    if (CALC_HORIZONTAL == board->orientation){
        RECT rect;
        setRect(&rect, board->statRect.x + SMILEY_OFFSET_V, board->statRect.y, SMILEY_WIDTH, SMILEY_WIDTH);
        rotateRect(&rect);

#ifdef DEST_CASIO_CALC
        dsubimage(rect.x, rect.y, &g_smileys, SMILEY_WIDTH, board->smileyState * SMILEY_HEIGHT, SMILEY_WIDTH, SMILEY_HEIGHT, DIMAGE_NOCLIP);
#endif // #ifdef DEST_CASIO_CALC
    }
    else{
#ifdef DEST_CASIO_CALC
        dsubimage(board->statRect.x + SMILEY_OFFSET_V, board->statRect.y, &g_smileys, 0, board->smileyState * SMILEY_HEIGHT, SMILEY_WIDTH, SMILEY_HEIGHT, DIMAGE_NOCLIP);
#endif // #ifdef DEST_CASIO_CALC
    }

    if (update){
#ifdef DEST_CASIO_CALC
        dupdate();
#endif // #ifdef DEST_CASIO_CALC
    }
}

///  board_drawBox() : Draw a single box
//
//      These 2 functions draw a given box.
//      board_drawBox checks wether a rotation needs to be done
//      whereas board_directDrawBox assumes rotation has been done by the calling function
//
//  @board : Pointer to the board
//  @pos : Box coordinates in the grid
//  @dx, @dy : Screen coordinates of the top-left corner
//
void board_drawBox(PBOARD const board, PCOORD const pos, uint16_t dx, uint16_t dy){
    if (CALC_HORIZONTAL == board->orientation){
        RECT rect = {dx, dy, dx + BOX_WIDTH - 1, dy + BOX_HEIGHT - 1};
        rotateRect(&rect);
        board_directDrawBox(board, pos, rect.x, rect.y);
    }
    else{
        board_directDrawBox(board, pos, dx, dy);
    }
}

void board_directDrawBox(PBOARD const board, PCOORD const pos, uint16_t dx, uint16_t dy){
    PBOX box = BOX_AT_POS(board->grid, pos);

#ifdef DEST_CASIO_CALC
#ifdef _DEBUG_
    int ID = ((board->debug && box->mine && box->state!=BS_BLAST)?BS_MINE:box->state);  // Always show mines in DEBUG mode
    dsubimage(dx, dy, &g_boxes, board->orientation * BOX_WIDTH, ID * BOX_HEIGHT, BOX_WIDTH, BOX_HEIGHT, DIMAGE_NOCLIP);
#else
    dsubimage(dx, dy, &g_boxes, board->orientation * BOX_WIDTH, box->state * BOX_HEIGHT, BOX_WIDTH, BOX_HEIGHT, DIMAGE_NOCLIP);
#endif // #ifdef _DEBUG_
#else
    //printf("| %c ", box->mine?'x':'0' + grid_countMines(board->grid, pos));
    if (box->mine){
        printf("| x ");
    }
    else{
        printf("| %c ", box->state>BS_DICEY_DOWN?'0' + (15 - box->state):' ');
    }
#endif // #ifdef DEST_CASIO_CALC
}

//  board_drawBoxAtPos() : Draw the box at a given position
//
//  @board : Pointer to the board
//  @pos : Box coordinates in the grid
//
void board_drawBoxAtPos(PBOARD const board, PCOORD const pos){
    POINT scrPos; // Position in screen coordinates
    board_Pos2Point(board, pos, &scrPos);
    board_drawBox(board, pos, scrPos.x, scrPos.y);
}

// board_drawNavButtonsEx() : Draw buttons for viewport scrolling
//
//  @board : pointer to the board
//  @highLight : Draw buttons in hightlighted state
//  @update : Update screen ?
//
void board_drawNavButtonsEx(PBOARD board, BOOL highLight, BOOL update){
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
                showButton = ((board->viewPort.visibleFrame.x + board->viewPort.visibleFrame.w) < board->viewPort.dimensions.col);
                break;

            // bottom
            case 2:
                showButton = ((board->viewPort.visibleFrame.y + board->viewPort.visibleFrame.h) < board->viewPort.dimensions.row);
                break;

            // left
            case 3:
                showButton = (board->viewPort.visibleFrame.x>0);
                break;
        } // switch

        // Show / hide the button
        if (highLight || !showButton){
#ifdef DEST_CASIO_CALC
            drect(
                board->viewPort.navButtons[id].x, board->viewPort.navButtons[id].y,
                board->viewPort.navButtons[id].x + board->viewPort.navButtons[id].w - 1,
                board->viewPort.navButtons[id].y + board->viewPort.navButtons[id].h - 1,
                //showButton?C_INVERT:COL_BKGROUND);
                COL_BKGROUND);
#endif // #ifdef DEST_CASIO_CALC
        }
        else{
#ifdef DEST_CASIO_CALC
            dsubimage(
                board->viewPort.navButtons[id].x, board->viewPort.navButtons[id].y,
                &g_viewport, 0, id * NAV_BUTTON_HEIGHT,
                NAV_BUTTON_HEIGHT, NAV_BUTTON_HEIGHT,
                DIMAGE_NOCLIP);
#endif // #ifdef DEST_CASIO_CALC
           }
    } // if

    if (update){
#ifdef DEST_CASIO_CALC
        dupdate();
#endif // #ifdef DEST_CASIO_CALC
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

// board_drawBorder() : Draw a 3D border
//
//  Draw a 3D border with the specified thickness around a rect
//  borders are outside the rectangle
//
//  @board : Pointer to the board
//  @rect : Rect. whose borders will be drawn
//  @thickness : Border thickness
//
void board_drawBorder(PBOARD board, PRECT const rect, uint8_t thickness){
    RECT rc;

#ifdef DEST_CASIO_CALC
    BOOL vertical = (board->orientation == CALC_VERTICAL);
    int light, dark;
    light = COLOUR_LT_GREY;
    dark = COLOUR_GREY;
#endif // #ifdef DEST_CASIO_CALC

    copyRect(&rc, rect);

    for (uint8_t count = 0; count < thickness; count++){
        inflateRect(&rc, 1, 1);
#ifdef DEST_CASIO_CALC
        // top (if CALC_VERTICAL)
        dline(rc.x, rc.y, rc.x + rc.w - 1, rc.y, vertical?light:dark);

        // left
        dline(rc.x, rc.y, rc.x, rc.y + rc.h - 1, light);

        // bottom
        dline(rc.x, rc.y + rc.h - 1, rc.x + rc.w - 1, rc.y + rc.h - 1, vertical?dark:light);

        // right
        dline(rc.x + rc.w - 1, rc.y, rc.x + rc.w - 1, rc.y + rc.h - 1, dark);
#endif // #ifdef DEST_CASIO_CALC
    }
}

//  board_setOrientation() : Set drawing orientation
//
//  @board : Pointer to the board
//  @orientation : Drawing orientation
//
void board_setOrientation(PBOARD const board, CALC_ORIENTATION orientation){
    if (board && board->grid){
        board->orientation = orientation;

        // no nav. buttons on 'beginner' mode
        board->fullGrid = (LEVEL_BEGINNER == board->grid->level);

        board->viewPort.dimensions.col = board->grid->size.col;
        board->viewPort.dimensions.row = board->grid->size.row;

        // Put widgets on the screen
        if (board->orientation == CALC_VERTICAL){
            // Playground (grid + viewport buttons)
            setRect(&board->playgroundRect,
                GRID_VIEWPORT_LEFT, GRID_VIEWPORT_TOP,
                BEGINNER_COLS * BOX_WIDTH, BEGINNER_ROWS * BOX_HEIGHT);
            copyRect(&board->gridRect, &board->playgroundRect);

            if (!board->fullGrid){
                board->playgroundRect.w += 2 * NAV_BUTTON_WIDTH;
                board->playgroundRect.h += 2 * NAV_BUTTON_HEIGHT;

                board->gridRect.x += NAV_BUTTON_WIDTH;
                board->gridRect.y += NAV_BUTTON_HEIGHT;
            }

            // Stat rect ( = [mines][smiley][timer] )
            setRect(&board->statRect,
                STAT_LEFT_V + (int)(board->fullGrid ? 0 : (2 * NAV_BUTTON_WIDTH)),
                STAT_TOP_V,
                STAT_WIDTH, STAT_HEIGHT);

            // Viewport (only 'beginner' size in vertical mode)
            setRect(&board->viewPort.visibleFrame, 0, 0, BEGINNER_COLS, BEGINNER_ROWS);
        }
        else{

            // In horizontal mode, stats and grids are aligned (just like in the original game)
            // and centered "horizontally"

            // Stat rect ( = [mines][smiley][timer] )
            setRect(&board->statRect,
                (CASIO_HEIGHT - STAT_WIDTH) / 2,
                STAT_TOP_V, STAT_WIDTH, STAT_HEIGHT);

            // 1 - Viewport <= count of visible boxes
            switch (board->grid->level){
                case LEVEL_BEGINNER:
                    setRect(&board->viewPort.visibleFrame, 0, 0, BEGINNER_COLS, BEGINNER_ROWS);
                    break;

                case LEVEL_MEDIUM:
                case LEVEL_EXPERT:
                    setRect(&board->viewPort.visibleFrame, 0, 0,
                        MIN_VAL(board->grid->size.col, BUTTON_H_MAX),
                        MIN_VAL(board->grid->size.row, BUTTON_V_MAX));
                    break;
            }

            // 2 - Dimensions of playground and grid rects
            setRect(&board->playgroundRect,
                (CASIO_HEIGHT - BOX_WIDTH * board->playgroundRect.w) / 2,
                board->statRect.y + STAT_HEIGHT + STAT_BORDER + PLAYGROUND_BORDER,
                board->viewPort.visibleFrame.w * BOX_WIDTH,
                board->viewPort.visibleFrame.h * BOX_HEIGHT);
            copyRect(&board->gridRect, &board->playgroundRect);

            if (!board->fullGrid){
                board->playgroundRect.x -= NAV_BUTTON_WIDTH;
                board->playgroundRect.w += 2 * NAV_BUTTON_WIDTH;
                board->playgroundRect.h += 2 * NAV_BUTTON_HEIGHT;

                board->gridRect.y += NAV_BUTTON_HEIGHT;
            }

        }   // if (board->orientation == CALC_VERTICAL)

        // Viewport navigation buttons
        //
        if (!board->fullGrid){
            // top button
            setRect(&board->viewPort.navButtons[0],
                board->gridRect.x + (board->gridRect.w - NAV_BUTTON_WIDTH) / 2,
                board->gridRect.y - NAV_BUTTON_HEIGHT,
                NAV_BUTTON_WIDTH, NAV_BUTTON_HEIGHT);

            // right btton
            setRect(&board->viewPort.navButtons[1],
                board->gridRect.x + board->gridRect.w,
                board->gridRect.y + (board->gridRect.h - NAV_BUTTON_HEIGHT) / 2,
                NAV_BUTTON_WIDTH, NAV_BUTTON_HEIGHT);

            // bottom button
            setRect(&board->viewPort.navButtons[2],
                board->viewPort.navButtons[0].x,
                board->gridRect.y + board->gridRect.h,
                NAV_BUTTON_WIDTH, NAV_BUTTON_HEIGHT);

            // left button
            setRect(&board->viewPort.navButtons[3],
                board->gridRect.x - NAV_BUTTON_WIDTH,
                board->viewPort.navButtons[1].y,
                NAV_BUTTON_WIDTH, NAV_BUTTON_HEIGHT);
        }
    } // if (grid)
}

//  board_selectBoxEx() : Select a box
//
//  @board : Pointer to the board
//  @pos : Box coordinates of the box in the grid
//  @select : TRUE if box is selected, FALSE if unselected
//
void board_selectBoxEx(PBOARD const board, PCOORD const pos, BOOL select){
    POINT base;
    board_Pos2Point(board, pos, &base);

    if (select){
#ifdef DEST_CASIO_CALC
        drect(base.x + 2, base.y + 2, base.x + BOX_WIDTH - 3, base.y + BOX_HEIGHT - 3, C_INVERT);
#endif // #ifdef DEST_CASIO_CALC
    }
    else{
        board_drawBox(board, pos, base.x, base.y);
    }

#ifdef TRACE_MODE
        //char trace[250];
        //__coordtoa(select?"Sel : ":"Uns : ", base.x, base.y, trace);
        //TRACE(trace, C_BLACK, COL_BKGROUND);
#endif // TRACE_MODE
}

//
// Tools
//

//  rotatePoint() : Rotate (trig. 90°) and translate a single point
//
//  @pos : Pointer to point coordinates
//
void rotatePoint(PPOINT const pos){
    int16_t ny = CASIO_HEIGHT - pos->x;
    pos->x = pos->y;
    pos->y = ny;
}

//  rotateRect() : Rotate (trig. 90°) and translate a rectangle
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
