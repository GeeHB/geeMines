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
    extern bopti_image_t g_scroll;
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
    board_setGameStateEx(board, STATE_WAITING, TRUE);
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
    board_setGameStateEx(board, STATE_WAITING, TRUE);
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
    uint8_t r,c;
    BOOL redrawGrid = FALSE;
    PBOX box = NULL;

    board->gameState = state;

    switch (state){
        case STATE_WON:
        {
            board->minesLeft = 0;
            board_setSmileyEx(board, SMILEY_WIN, FALSE);
            for (r = 0; r < board->grid->size.row; r++)
                for (c = 0; c < board->grid->size.col; c++){
                    box = BOX_AT(board->grid, r, c);
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
            board_setSmileyEx(board, SMILEY_LOSE, FALSE);
            for (r = 0; r < board->grid->size.row; r++)
                for (c = 0; c < board->grid->size.col; c++){
                    box = BOX_AT(board->grid, r, c);
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

// board_isBoxVisible() : Check if box is visible
//
// Check that the box at the given position is in the viewPort visible frame
//
//  @board : pointer to the board
//  @pos : Coordindate of the box in the grid
//
//  @return TRUE the box is visible
//
BOOL board_isBoxVisible(PBOARD const board, PCOORD const pos){
    return (pos->col >= board->viewPort.visibleFrame.x &&
            pos->col < (board->viewPort.visibleFrame.x + board->viewPort.visibleFrame.w) &&
            pos->row >= board->viewPort.visibleFrame.y &&
            pos->row < (board->viewPort.visibleFrame.y + board->viewPort.visibleFrame.h));
}

//  board_drawEx() : Draw the whole board
//
//  @board : Pointer to the board
//  @menu : Menu present ?
//  @update : update the screen ?
//
void board_drawEx(PBOARD const board, BOOL menu, BOOL update){
#ifdef DEST_CASIO_CALC
    drect(0, 0, CASIO_WIDTH - 1, CASIO_HEIGHT - 1 - (menu?MENUBAR_DEF_HEIGHT:0), BKGROUND_COLOUR);
#endif // #ifdef DEST_CASIO_CALC

    if (board->grid && board->grid->boxes){
        // Stats
        board_drawBorder(board->orientation, &board->statRect, STAT_BORDER);
        board_drawMinesLeftEx(board, FALSE);
        board_drawSmileyEx(board, FALSE);
        board_drawTimeEx(board, FALSE);

        // Grid
        board_drawBorder(board->orientation, &board->gridRect, GRID_BORDER);
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

    if (!board || !board->grid || !board->grid->boxes){
        return;
    }

    setRect(&rect, board->gridRect.x, board->gridRect.y, BOX_WIDTH, BOX_HEIGHT);

    if (CALC_HORIZONTAL == board->orientation){
        rotateRect(&rect);

        offsetCol.x = 0;
        offsetCol.y = -1 * BOX_HEIGHT;

        offsetRow.x = BOX_WIDTH;
        offsetRow.y = 0;

        origin = rect.y;
    }
    else{
        offsetCol.x = BOX_WIDTH;
        offsetCol.y = 0;

        offsetRow.x = 0;
        offsetRow.y = BOX_HEIGHT;

        origin = rect.x;
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
            board_directDrawBox(board, &pos, rect.x, rect.y);
            offsetRect(&rect, offsetCol.x, offsetCol.y);
        }

        offsetRect(&rect, offsetRow.x, offsetRow.y);
#ifndef DEST_CASIO_CALC
        printf("|\n");       // EOL
#endif // #ifndef DEST_CASIO_CALC
    }

    if (board->viewPort.scrolls != NO_SCROLL){
        board_drawScrollBars(board, FALSE);
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
        ids[ids[1]?0:1] = LED_MINUS_ID;
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
        RECT rect = {dx, dy, BOX_WIDTH, BOX_HEIGHT};
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
    int ID = ((board->debug && box->mine && (box->state==BS_INITIAL || box->state>=BS_MINE ))?BS_MINE:box->state);  // Always show mines in DEBUG mode
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
        printf("| %c ", box->state>BS_DICEY_DOWN?'0' + (BS_DOWN - box->state):'A' + box->state);
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

// board_drawScrollBar() : Draw a viewport's scrollbar
//
//  @board : pointer to the board
//  @scrollID   : SCROLL_HORIZONTAL if scrollbar is horizontal,
//                SCROLL_VERTICAL for a vertical scrollbar
//  @blink : Make the scroll bars blink ?
//
void board_drawScrollBar(PBOARD board, uint8_t scrollID, BOOL blink){
    if (scrollID && scrollID < SCROLL_BOTH){
        RECT rectBk, rectBar;
        POINT ptBegin, ptEnd;
        uint16_t dimension;
#ifdef DEST_CASIO_CALC
        int colour = (blink?SCROLL_COLOUR_BLINK:SCROLL_COLOUR);
#endif // #ifdef DEST_CASIO_CALC

        copyRect(&rectBk, &board->viewPort.scrollBars[scrollID - 1]);
        copyRect(&rectBar, &board->viewPort.scrollBars[scrollID - 1]);
        deflateRect(&rectBar, SCROLL_SPACE, SCROLL_SPACE);

        if (SCROLL_HORIZONTAL == scrollID){
            dimension = rectBar.w;  // = 100%
            rectBar.w = rectBar.w * board->viewPort.visibleFrame.w / board->viewPort.dimensions.col;
            rectBar.x += (dimension * board->viewPort.visibleFrame.x / board->viewPort.dimensions.col + SCROLL_RADIUS);
            rectBar.w -= 2*SCROLL_RADIUS;

            ptBegin = (POINT){.x=rectBar.x,.y=rectBar.y+SCROLL_RADIUS};
            ptEnd = (POINT){.x=rectBar.x + rectBar.w - 1,.y=rectBar.y+SCROLL_RADIUS};
        }
        else{
            dimension = rectBar.h;  // = 100%
            rectBar.h = rectBar.h * board->viewPort.visibleFrame.h /  board->viewPort.dimensions.row;
            rectBar.y += (dimension * board->viewPort.visibleFrame.y / board->viewPort.dimensions.row + SCROLL_RADIUS);
            rectBar.h -= 2*SCROLL_RADIUS;

            ptBegin = (POINT){.x=rectBar.x+SCROLL_RADIUS,.y=rectBar.y};
            ptEnd = (POINT){.x=rectBar.x+SCROLL_RADIUS,.y=rectBar.y+rectBar.w-1};
        }

        if (CALC_HORIZONTAL == board->orientation){
            rotateRect(&rectBk);
            rotateRect(&rectBar);

            rotatePoint(&ptBegin);   // centers of circles
            rotatePoint(&ptEnd);
        }

#ifdef DEST_CASIO_CALC
        drect(rectBk.x, rectBk.y, rectBk.x + rectBk.w -1, rectBk.y + rectBk.h -1, BKGROUND_COLOUR);     // Erase scroll. bckgrnd

        // Rounded rectangle
        dcircle(ptBegin.x, ptBegin.y, SCROLL_RADIUS, colour, colour);
        dcircle(ptEnd.x, ptEnd.y, SCROLL_RADIUS, colour, colour);
        drect(rectBar.x, rectBar.y , rectBar.x + rectBar.w - 1, rectBar.y + rectBar.h - 1, colour);
#endif // #ifdef DEST_CASIO_CALC
    } // if (scrollID < SCROLL_BOTH){
}

// board_drawScrollBars() : Draw viewport's scrollbars
//
//  Draw any of viewport's scrollbars, or both, or none ...
//
//  @board : pointer to the board
//  @blink : Make the scroll bars blink ?
//
void board_drawScrollBars(PBOARD board, BOOL blink){
    if (board->viewPort.scrolls & SCROLL_HORIZONTAL){
        board_drawScrollBar(board, SCROLL_HORIZONTAL, blink);
    }

    if (board->viewPort.scrolls & SCROLL_VERTICAL){
        board_drawScrollBar(board, SCROLL_VERTICAL, blink);
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
//  @orientation : Rectangle's orientation
//  @rect : Rect. whose borders will be drawn
//  @thickness : Border thickness
//
void board_drawBorder(CALC_ORIENTATION orientation, PRECT const rect, uint8_t thickness){
    RECT rc;
    BOOL vertical = (orientation == CALC_VERTICAL);

#ifdef DEST_CASIO_CALC
    int light, dark;
    light = COLOUR_LT_GREY;
    dark = COLOUR_GREY;
#endif // #ifdef DEST_CASIO_CALC

    copyRect(&rc, rect);

    if (!vertical){
        rotateRect(&rc);
    }

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
        int16_t gridWidth, gridHeight;

        board->orientation = orientation;

        board->viewPort.dimensions.col = board->grid->size.col;     // Real dims of the grid
        board->viewPort.dimensions.row = board->grid->size.row;

        // Count of visible boxes
        switch (board->grid->level){
            case LEVEL_BEGINNER:
                board->viewPort.scrolls = NO_SCROLL;    // no scroll buttons on 'beginner' mode
                setRect(&board->viewPort.visibleFrame, 0, 0, BEGINNER_COLS, BEGINNER_ROWS);
                break;

            case LEVEL_MEDIUM:
                board->viewPort.scrolls = (board->orientation==CALC_HORIZONTAL?SCROLL_HORIZONTAL:SCROLL_VERTICAL);
                setRect(&board->viewPort.visibleFrame, 0, 0,
                    MIN_VAL(board->grid->size.col, (board->orientation==CALC_HORIZONTAL?BUTTON_HORZ_COL_MAX:BUTTON_VERT_COL_MAX)),
                    MIN_VAL(board->grid->size.row, (board->orientation==CALC_HORIZONTAL?BUTTON_HORZ_ROW_MAX:BUTTON_VERT_ROW_MAX)));
                break;

            case LEVEL_EXPERT:
                board->viewPort.scrolls = SCROLL_BOTH;
                setRect(&board->viewPort.visibleFrame, 0, 0,
                    MIN_VAL(board->grid->size.col, (board->orientation==CALC_HORIZONTAL?BUTTON_HORZ_COL_MAX:BUTTON_VERT_COL_MAX)),
                    MIN_VAL(board->grid->size.row, (board->orientation==CALC_HORIZONTAL?BUTTON_HORZ_ROW_MAX:BUTTON_VERT_ROW_MAX)));
                break;
        }

        // Rectangles positions
        //

        // 1 - Stat rect ( = [mines][smiley][timer] )
        setRect(&board->statRect,
            ((board->orientation==CALC_VERTICAL?CASIO_WIDTH:CASIO_HEIGHT)
                - STAT_WIDTH - 2*STAT_BORDER ) / 2,
            EMPTY_SPACE + STAT_BORDER,
            STAT_WIDTH, STAT_HEIGHT);

        // 2 - Dimensions of grid rect
        gridWidth = BOX_WIDTH * board->viewPort.visibleFrame.w;
        gridHeight = BOX_HEIGHT * board->viewPort.visibleFrame.h;
        setRect(&board->gridRect,
            ((board->orientation==CALC_VERTICAL?CASIO_WIDTH:CASIO_HEIGHT)
               - gridWidth -2*GRID_BORDER) / 2,
            board->statRect.y + STAT_HEIGHT + 2 * STAT_BORDER + GRID_BORDER + EMPTY_SPACE,
            gridWidth, gridHeight);

        // 3 - Viewport scrollbars
        if (board->viewPort.scrolls & SCROLL_HORIZONTAL){
            setRect(&board->viewPort.scrollBars[0],
                board->gridRect.x - GRID_BORDER,
                board->gridRect.y + board->gridRect.h + GRID_BORDER,
                board->gridRect.w + 2 * GRID_BORDER,
                SCROLL_HEIGHT);
        }

        if (board->viewPort.scrolls & SCROLL_VERTICAL){
            setRect(&board->viewPort.scrollBars[1],
                board->gridRect.x + board->gridRect.w + GRID_BORDER,
                board->gridRect.y - GRID_BORDER,
                SCROLL_WIDTH,
                board->gridRect.h + 2 * GRID_BORDER);
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
        if (CALC_HORIZONTAL == board->orientation){
            RECT rect = {base.x, base.y, BOX_WIDTH, BOX_HEIGHT};
            rotateRect(&rect);
            drect(rect.x + 2, rect.y + 2, rect.x + BOX_WIDTH - 3, rect.y + BOX_HEIGHT - 3, C_INVERT);
        }
        else{
            drect(base.x + 2, base.y + 2, base.x + BOX_WIDTH - 3, base.y + BOX_HEIGHT - 3, C_INVERT);
        }
#endif // #ifdef DEST_CASIO_CALC
    }
    else{
        board_drawBox(board, pos, base.x, base.y);
    }

#ifdef TRACE_MODE
        char trace[250];
        __coordtoa(select?"Sel : ":"Uns : ", base.x, base.y, trace);
        TRACE(trace, C_BLACK, BKGROUND_COLOUR);
#endif // TRACE_MODE
}

// EOF
