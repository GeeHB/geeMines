//----------------------------------------------------------------------
//--
//--    game.c
//--
//--            the game !
//--
//----------------------------------------------------------------------

#include "game.h"
#include "board.h"

#include <stdint.h>

#ifdef DEST_CASIO_CALC
#include "shared/keys.h"
#include <gint/clock.h>

extern bopti_image_t g_pause;
#endif // #ifdef DEST_CASIO_CALC

// _onNewGame() : Create a new game
//
//  @board : pointer to the game board
//  @level : game difficulty level
//
void _onNewGame(PBOARD const board, uint8_t level){
    if (board_init(board, level)){
        board_update(board, TRUE);
    }
}

// __callbackTick() : Callback function for timer
//
//  This function is used during game to make selected box blink
//
//  @pTick : pointer to blinking state indicator
//
//  @return : TIMER_CONTINUE if valid
//
#ifdef DEST_CASIO_CALC
static int __callbackTick(volatile int *pTick){
    *pTick = 1;
    return TIMER_CONTINUE;
}
#endif // #ifdef DEST_CASIO_CALC

// _onStartGame() : Start a new game
//
//  @board : pointer to the game board
//
//  @return : FALSE on error
//
#ifdef DEST_CASIO_CALC
BOOL _onStartGame(PBOARD const board){
    if (!board){
        return FALSE;
    }

    uint key = KEY_NONE;
    COORD pos = {0,0}, oPos = {0, 0};
    BOOL showScroll = FALSE, hightLighted = FALSE;
    uint16_t redraw = NO_REDRAW;

#ifdef SCREEN_CAPTURE
    BOOL captureOn = FALSE;
#endif // #ifdef SCREEN_CAPTURE

    board_setGameStateEx(board, STATE_PLAYING, FALSE);
    board_drawEx(board, FALSE, FALSE);
    board_selectBox(board, &pos);

    // Timer for blinking effect
    int tickCount = 0;
    static volatile int tick = 1;
    int timerID = timer_configure(TIMER_ANY, TIMER_TICK_DURATION * 1000,
                                    GINT_CALL(__callbackTick, &tick));
    if (timerID >= 0){
        timer_start(timerID);   // set the timer
    }
    else{
        board->gameState = STATE_CANCELLED;   // No timer => no game
    }

    while (board->gameState == STATE_PLAYING){
        // Time management
        while(!tick){
            sleep();
        }
        tick = 0;
        tickCount++;

        if (0 == (tickCount % BLINK_CURSOR)){
            redraw |= REDRAW_SELECTION; // Time to blink ?
        }

        if (0 == (tickCount % TIMER_SECOND)){
            redraw |= REDRAW_TIME;
        }

        if (board->viewPort.scrolls && 0 == (tickCount % BLINK_SCROLL_BUTTONS)){
            redraw |= REDRAW_SCROLL_BUTTONS;
        }

        // A keyboard event ?
        key = getKeyEx(NULL);
        switch (key){
            // Change cursor pos
            //
            case KEY_CODE_LEFT:
                redraw = _onKeyLeft(board, &pos);
                break;

            case KEY_CODE_DOWN:
                redraw = _onKeyDown(board, &pos);
                break;

            case KEY_CODE_RIGHT:
                redraw = _onKeyRight(board, &pos);
                break;

            case KEY_CODE_UP:
                redraw = _onKeyUp(board, &pos);
                break;

            // User actions
            //
            case KEY_CODE_STEP:
                if (_onStep(board, &pos, &redraw)){
                    if (board->steps == board->grid->maxSteps){
                        board_gameWon(board);
                    }
                }
                else{
                    board_gameLost(board);
                }

                break;

            case KEY_CODE_FLAG:
                redraw = _onFlag(board, &pos);
                break;

            case KEY_CODE_QUESTION:
                redraw = _onQuestion(board, &pos);
                break;

            case KEY_CODE_PAUSE:
                _onPause();
                board_update(board, FALSE);    // update screen
                break;

            case KEY_CODE_ROTATE_DISPLAY:
                board_setOrientation(board, (CALC_VERTICAL == board->orientation)?CALC_HORIZONTAL:CALC_VERTICAL);
                board_drawEx(board, FALSE, FALSE);
                oPos = pos = (COORD){.row=0,.col=0};
                redraw = REDRAW_UPDATE | REDRAW_SELECTION | REDRAW_SCROLL_BUTTONS;
                break;

#ifdef SCREEN_CAPTURE
            case KEY_CODE_CAPTURE:
                if (captureOn){
                    capture_remove();
                }
                else{
                    capture_install();
		            board_drawEx(board, FALSE, TRUE);
                }

                captureOn = !captureOn;
                break;
#endif // #ifdef SCREEN_CAPTURE

            // Cancel (end) the game
            case KEY_CODE_EXIT:
                board_setGameStateEx(board, STATE_CANCELLED, TRUE);
                break;

            default:
                break;
        } // switch (key)

        if (redraw != NO_REDRAW){

            if (redraw & REDRAW_GRID){
                board_drawGridEx(board, FALSE); // no screen update

                if (board->viewPort.scrolls){
                    redraw |= REDRAW_SCROLL_BUTTONS;
                }
            }

            if (redraw & REDRAW_BOX){
                board_drawBoxAtPos(board, &pos);
            }

            if (redraw & REDRAW_MOVE){
                board_unselectBox(board, &oPos);
                board_selectBox(board, &pos);
                oPos = (COORD){.row = pos.row, .col = pos.col}; // oPos = pos
            }

            if (redraw & REDRAW_SELECTION){
                hightLighted = !hightLighted;// Blink selected box
                board_selectBoxEx(board, &pos, hightLighted);
            }

            if (redraw & REDRAW_SCROLL_BUTTONS){
                showScroll = !showScroll;
                board_drawScrollBarsEx(board, showScroll, FALSE); // Blink scroll buttons
            }

            if (redraw & REDRAW_MINES_LEFT){
                board_drawMinesLeftEx(board, FALSE);
            }

            if (redraw & REDRAW_TIME){
                if (++board->time >= TIMER_MAX_VALUE){
                    board_gameLost(board);
                }

                board_drawTimeEx(board, FALSE);     // Time has changed
            }

            dupdate();      // (redraw & REDRAW_UPDATE)
            redraw = NO_REDRAW;
        } // if (reDraw)
    } // while (board->gameState == STATE_PLAYING)

    if (timerID >= 0){
        timer_stop(timerID);
    }

    return TRUE;
}
#endif // #ifdef DEST_CASIO_CALC

//  _onStep : User steps on a box
//
//  @board : Pointer to the board
//  @pos : Position of the box
//  @redraw : pointer to the redraw indicator
//
//  @return : FALSE if stepped on a mine
//
BOOL _onStep(PBOARD const board, PCOORD const pos, uint16_t* redraw){
    uint8_t minesAround = 0;
    PBOX box = BOX_AT_POS(board->grid, pos);

    // Already stepped ???
    if (box->state > BS_QUESTION){
        (*redraw) = NO_REDRAW;
        return TRUE;
    }

    (*redraw) = REDRAW_UPDATE;

    if (box->mine){
        // stepped on a mine!
        box->state =  BS_BLAST;
        board_drawBoxAtPos(board, pos);
        return FALSE;
    }

    minesAround = grid_countMines(board->grid, pos);
    board->steps++;
    box->state =  BS_DOWN - minesAround;
    
    if (board_isBoxVisible(board, pos)){
        board_drawBoxAtPos(board, pos);
    }

    // Auto step surrounding boxes
    if (!minesAround){
        COORD nPos;
        int8_t r,c;
        for (r=-1; r<=1; r++){
            for (c=-1; c<=1; c++){
                nPos = (COORD){.col = pos->col + c, .row = pos->row + r};
                if ((r || c) &&
                    nPos.col < board->grid->size.col &&
                    nPos.row < board->grid->size.row){
                        _onStep(board, &nPos, redraw);
                }
            }
        }
    }

    return TRUE;    // No mine at this pos
}

// _onFlag() : Put / remove a flag
//
//  @board : pointer to the current board
//  @pos : Current position in the grid
//
//  @return drawing action to perform or NO_DRAWING
//
uint16_t _onFlag(PBOARD const board, PCOORD const pos){
    PBOX box = BOX_AT_POS(board->grid, pos);
    BOOL flagPresent;

    if (box->state <= BS_QUESTION){
        flagPresent = (box->state == BS_FLAG);
        box->state = (flagPresent?BS_INITIAL:BS_FLAG);
        board->minesLeft += flagPresent?+1:-1;    // mines left !!
        return REDRAW_BOX | REDRAW_MINES_LEFT;
    }

    return NO_REDRAW;
}

// _onQuestion() : Put / remove a 'question' attribute to the box
//
//  @board : pointer to the current board
//  @pos : Current position in the grid
//
//  @return drawing action to perform or NO_DRAWING
//
uint16_t _onQuestion(PBOARD const board, PCOORD const pos){
    PBOX box = BOX_AT_POS(board->grid, pos);
    BOOL questionPresent;

    if (box->state <= BS_QUESTION){
        questionPresent = (box->state == BS_QUESTION);
        box->state = (questionPresent?BS_INITIAL:BS_QUESTION);
        return REDRAW_BOX;
    }

    return NO_REDRAW;
}

// _onPause() : Show pause screen
//
void _onPause(){
#ifdef DEST_CASIO_CALC
    uint car = KEY_NONE;
    uint16_t y;

    // Top of image
    dsubimage(0, 0, &g_pause,
            0, 0, IMG_PAUSE_W, IMG_PAUSE_COPY_Y, DIMAGE_NOCLIP);

    // "middle"
    for (y = IMG_PAUSE_COPY_Y;
        y < (IMG_PAUSE_COPY_Y + IMG_PAUSE_LINES); y++){
        dsubimage(0, y, &g_pause,
            0, IMG_PAUSE_COPY_Y,
            IMG_PAUSE_W, 1, DIMAGE_NOCLIP);
    }

    // bottom
    y = CASIO_HEIGHT - IMG_PAUSE_H + IMG_PAUSE_COPY_Y - 1;
    dsubimage(0, y, &g_pause,
            0, IMG_PAUSE_COPY_Y + 1,
            IMG_PAUSE_W, IMG_PAUSE_H - IMG_PAUSE_COPY_Y - 1,
            DIMAGE_NOCLIP);

    dupdate();

    do{
        car = getKey();
    }while (KEY_CODE_PAUSE != car && KEY_CODE_EXIT != car);

    if (KEY_CODE_EXIT == car){
        // Close app.
        gint_osmenu();
    }
#endif // #ifdef DEST_CASIO_CALC
}

// _onKeyLeftEx() : User press "left" key
//
//  @board : pointer to the game board
//  @pos : position of cursor
//  @check : if TRUE check display orientation
//
//  @return : TDrawing action(s) to perform or NO_REDRAW
//
uint8_t _onKeyLeftEx(PBOARD const board, PCOORD pos, BOOL check){

    if (check && CALC_HORIZONTAL == board->orientation){
        return _onKeyUpEx(board, pos, FALSE);
    }

    uint8_t action = NO_REDRAW;

    if (pos->col){
        if (board->viewPort.visibleFrame.x == pos->col){
            board->viewPort.visibleFrame.x--;
            action = REDRAW_GRID;
        }

        pos->col--;
        action |= REDRAW_MOVE;
    }

    return action;
}

// _onKeyDownEx() : User press "down" key
//
//  @board : pointer to the game board
//  @pos : position of cursor
//  @check : if TRUE check display orientation
//
//  @return : Drawing action(s) to perform or NO_REDRAW
//
uint8_t _onKeyDownEx(PBOARD const board, PCOORD pos, BOOL check){

    if (check && CALC_HORIZONTAL == board->orientation){
        return _onKeyLeftEx(board, pos, FALSE);
    }

    uint8_t action = NO_REDRAW;

    if (pos->row < (board->viewPort.dimensions.row - 1)){
        if ((board->viewPort.visibleFrame.y + board->viewPort.visibleFrame.h - 1) == pos->row){
            board->viewPort.visibleFrame.y++;
            action = REDRAW_GRID;
        }

        pos->row++;
        action |= REDRAW_MOVE;
    }

    return action;
}

// _onKeyRightEx() : User press "right" key
//
//  @board : pointer to the game board
//  @pos : position of cursor
//  @check : if TRUE check display orientation
//
//  @return : Drawing action(s) to perform or NO_REDRAW
//
uint8_t _onKeyRightEx(PBOARD const board, PCOORD pos, BOOL check){
    if (check && CALC_HORIZONTAL == board->orientation){
        return _onKeyDownEx(board, pos, FALSE);
    }

    uint8_t action = NO_REDRAW;

    if (pos->col < (board->viewPort.dimensions.col - 1)){
        if ((board->viewPort.visibleFrame.x + board->viewPort.visibleFrame.w - 1) == pos->col){
            board->viewPort.visibleFrame.x++;
            action = REDRAW_GRID;
        }

        pos->col++;
        action |= REDRAW_MOVE;
    }

    return action;
}

// _onKeyUpEx() : User press "up" key
//
//  @board : pointer to the game board
//  @pos : position of cursor
//  @check : if TRUE check display orientation
//
//  @return : Drawing action(s) to perform or NO_REDRAW
//
uint8_t _onKeyUpEx(PBOARD const board, PCOORD pos, BOOL check){
    if (check && CALC_HORIZONTAL == board->orientation){
        return _onKeyRightEx(board, pos, FALSE);
    }

    uint8_t action = NO_REDRAW;

    if (pos->row){
        if (board->viewPort.visibleFrame.y == pos->row){
            board->viewPort.visibleFrame.y--;
            action = REDRAW_GRID;
        }

        pos->row--;
        action |= REDRAW_MOVE;
    }

    return action;
}

// EOF
