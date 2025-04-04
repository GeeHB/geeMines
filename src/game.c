//----------------------------------------------------------------------
//--
//--    game.c
//--
//--            the game !
//--
//----------------------------------------------------------------------

#include "shared/keys.h"
#include "shared/menu.h"
#include "shared/scrollBar.h"

#include "game.h"
#include "board.h"
#include "consts.h"
#include "scores.h"

#include <stdint.h>

#ifdef DEST_CASIO_CALC
    #include "shared/menu.h"
    #include <gint/clock.h>
    #include <string.h>

    extern bopti_image_t g_mine;   // "about" image
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

// _onAbout() : Show "about" informations
//
void _onAbout(){
#ifdef DEST_CASIO_CALC
    int w, h;
    char copyright[255];    // Should be enough !
    drect(0, 0, CASIO_WIDTH - 1, CASIO_HEIGHT - MENUBAR_DEF_HEIGHT - 1, C_WHITE);

    // Draw the logo
    dimage((CASIO_WIDTH - APP_LOGO_WIDTH) / 2 - APP_LOGO_OFFSET,
            (CASIO_HEIGHT - MENUBAR_DEF_HEIGHT - APP_LOGO_WIDTH) / 2 - APP_LOGO_OFFSET,
            &g_mine);

    // Copyright
    strcpy(copyright, APP_NAME);
    strcat(copyright, " by ");
    strcat(copyright, APP_AUTHOR);
    strcat(copyright, " v");
    strcat(copyright, APP_VERSION);
    dsize(copyright, NULL, &w, &h);

    dtext(CASIO_WIDTH - w - 5,
            CASIO_HEIGHT - MENUBAR_DEF_HEIGHT - h - 10,
            COLOUR_BLACK, copyright);

    dupdate();
#endif // #ifdef DEST_CASIO_CALC
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

// _onShowScores() : Show best scores
//
//  @scores : Array of scores
//  @level : Category of scores
//
void _onShowScores(PSCORE scores, uint8_t level){
    if (!scores || level > LEVEL_EXPERT){
        return;
    }

    POWNMENU menu = menu_create();
    if (menu){
        PMENUBAR bar = menu_getMenuBar(menu);
        BOOL end = FALSE;
        MENUACTION action;

        menubar_appendItem(bar, IDM_NEW_BEGINNER, IDS_NEW_BEGINNER, ITEM_STATE_DEFAULT, ITEM_STATUS_TEXT);
        menubar_appendItem(bar, IDM_NEW_MEDIUM, IDS_NEW_MEDIUM, ITEM_STATE_DEFAULT, ITEM_STATUS_TEXT);
        menubar_appendItem(bar, IDM_NEW_EXPERT, IDS_NEW_EXPERT, ITEM_STATE_DEFAULT, ITEM_STATUS_TEXT);
        menubar_addItem(bar, MENU_POS_RIGHT, IDM_QUIT, IDS_QUIT, ITEM_STATE_DEFAULT, ITEM_STATUS_DEFAULT);

        menubar_activateItem(bar, (IDM_NEW_BEGINNER + level), SEARCH_BY_ID, TRUE);

        menu_update(menu);

        _showScores(scores, level);

        while (!end){
            if (menu_handleKeyboard(menu, &action)){
                switch (action.value){
                    // Start a new game
                    case IDM_NEW_BEGINNER :
                    case IDM_NEW_MEDIUM :
                    case IDM_NEW_EXPERT:
                        menubar_activateItem(bar, action.value, SEARCH_BY_ID, TRUE);
                        _showScores(scores, action.value - IDM_NEW_BEGINNER);
                        menu_update(menu);
                        break;

                    case IDM_QUIT:
                        end = TRUE;
                        break;
                }
            }
        }

        menu_free(menu);    // Error while allocating submenu => no more menu
    }
}

// _showScores() : Show best scores for the given level
//
//  @scores : Array of scores
//  @level : Category of scores
//
void _showScores(PSCORE scores, uint8_t level){
    if (scores){
#ifdef DEST_CASIO_CALC
        int y, w, h;
        char line[255];
        RECT rect;

        strcpy(line, "Best scores - '");
        strcat(line, (level?(level==1?IDS_NEW_MEDIUM:IDS_NEW_EXPERT):IDS_NEW_BEGINNER));
        strcat(line,"'");
        dsize(line, NULL, &w, &h);

        setRect(&rect, 70, 15, 250, SCORES_HEIGHT * SCORE_LEVEL_COUNT + 2 * SCORES_TITLE_Y);
        drect_border(rect.x, rect.y, rect.x + rect.w - 1,  rect.y + rect.h - 1,
                BKGROUND_COLOUR, 2, COLOUR_RED);

        inflateRect(&rect, 4 , 4);
        drect_border(rect.x, rect.y, rect.x + rect.w - 1,  rect.y + rect.h - 1,
            C_NONE, 2, COLOUR_RED);

        dtext((CASIO_WIDTH - w) / 2, SCORES_TITLE_Y, COLOUR_BLACK, line);

        y = SCORES_TOP;
        for (uint id=0; id < SCORE_LEVEL_COUNT; id++){
            line[0] = '1' + id;
            line[1] = 0;
            dtext(SCORES_COL_ID, y, COLOUR_BLACK, line);
            scores_time2a(scores[level*SCORE_LEVEL_COUNT + id].time, line);
            strcat(line, " s");
            dtext(SCORES_COL_SCORE, y, COLOUR_BLACK, line);

            y += SCORES_HEIGHT;
        }

        dupdate();
#endif // DEST_CASIO_CALC
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
//  @scores : Array of scores
//
//  @return : FALSE on error
//
#ifdef SCREEN_CAPTURE
BOOL _onStartGame(PBOARD const board, PSCORE scores, BOOL capture){
#else
BOOL _onStartGame(PBOARD const board, PSCORE scores){
#endif // #ifdef SCREEN_CAPTURE
    if (!board){
        return FALSE;
    }

    uint key = KEY_NONE;
    COORD pos = {0,0}, oPos = {0, 0};
    //BOOL blinkScroll = FALSE;
    BOOL hightLighted = FALSE;
    uint16_t redraw = REDRAW_SELECTION;

#ifdef SCREEN_CAPTURE
    BOOL captureOn = capture;
#endif // #ifdef SCREEN_CAPTURE

    board_setGameStateEx(board, STATE_PLAYING, FALSE);
    board_drawEx(board, FALSE, FALSE);
    board_selectBox(board, &pos);

    // Timer for blinking effect
    int tickCount = 0;
    static volatile int tick = 1;

#ifdef DEST_CASIO_CALC
    int timerID = timer_configure(TIMER_ANY, TIMER_TICK_DURATION * 1000,
                                    GINT_CALL(__callbackTick, &tick));
    if (timerID >= 0){
        timer_start(timerID);   // set the timer
    }
    else{
        board->gameState = STATE_CANCELLED;   // No timer => no game
    }
#endif // #ifdef DEST_CASIO_CALC

    while (board->gameState == STATE_PLAYING){
        // Time management
#ifdef DEST_CASIO_CALC
        while(!tick){
            sleep();
        }
#endif // #ifdef DEST_CASIO_CALC
        tick = 0;
        tickCount++;

        if (0 == (tickCount % BLINK_CURSOR)){
            redraw |= REDRAW_SELECTION; // Time to make cursor blink
        }

        if (0 == (tickCount % TIMER_SECOND)){
            redraw |= REDRAW_TIME;  // One more second
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
                        board_setGameState(board, STATE_WON);
                    }
                }
                else{
                    board_setGameState(board, STATE_LOST);  // on a mine !
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
                redraw = REDRAW_UPDATE | REDRAW_SELECTION /*| REDRAW_SCROLLBARS*/;
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

            if (redraw & REDRAW_MINES_LEFT){
                board_drawMinesLeftEx(board, FALSE);
            }

            if (redraw & REDRAW_TIME){
                if (++board->time >= TIMER_MAX_VALUE){
                    board_setGameState(board, STATE_LOST);
                }

                board_drawTimeEx(board, FALSE);     // Time has changed
            }

#ifdef DEST_CASIO_CALC
            dupdate();      // (redraw & REDRAW_UPDATE)
#endif // #ifdef DEST_CASIO_CALC
            redraw = NO_REDRAW;
        } // if (reDraw)
    } // while (board->gameState == STATE_PLAYING)

#ifdef DEST_CASIO_CALC
    if (timerID >= 0){
        timer_stop(timerID);
    }
#endif // #ifdef DEST_CASIO_CALC

    if (board->gameState == STATE_WON){
        _gameWon(board, scores, board->grid->level, tickCount);
    }

    return TRUE;
}

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
//  @return : Drawing action to perform or NO_DRAWING
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
//  @return : Drawing action to perform or NO_DRAWING
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

// _onKeyLeftEx() : User press "left" key
//
//  @board : pointer to the game board
//  @pos : position of cursor
//  @check : if TRUE check display orientation
//
//  @return : Drawing action(s) to perform or NO_REDRAW
//
uint8_t _onKeyLeftEx(PBOARD const board, PCOORD pos, BOOL check){

    if (check && CALC_HORIZONTAL == board->orientation){
        return _onKeyUpEx(board, pos, FALSE);
    }

    uint8_t action = NO_REDRAW;

    if (pos->col){
        if (board->viewPort.visibleFrame.x == pos->col){
            scrollBar_moveTo(&board->viewPort.horzScroll,--board->viewPort.visibleFrame.x);
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
            scrollBar_moveTo(&board->viewPort.vertScroll,++board->viewPort.visibleFrame.y);
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
            scrollBar_moveTo(&board->viewPort.horzScroll,++board->viewPort.visibleFrame.x);
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
            scrollBar_moveTo(&board->viewPort.vertScroll,--board->viewPort.visibleFrame.y);
            action = REDRAW_GRID;
        }

        pos->row--;
        action |= REDRAW_MOVE;
    }

    return action;
}

// _gameWon() : The user won the game
//
//  @board : pointer to the game board
//  @scores : Array of scores
//  @level, @time : new score
//
void _gameWon(PBOARD const board, PSCORE scores, uint8_t level, int time){
    BOOL added = scores_add(scores, level, time);

    uint car = KEY_NONE;
    RECT rect = {0,0,280,94};
    centerRect(&rect, CASIO_WIDTH, CASIO_HEIGHT);

#ifdef DEST_CASIO_CALC
    drect(rect.x, rect.y, rect.x + rect.w - 1, rect.y + rect.h - 1, WINDOW_COLOUR);
    board_drawBorder(CALC_VERTICAL, &rect, 2);

    dimage(rect.x + 7, rect.y + 7, &g_mine);
    setRect(&rect, rect.x + 7, rect.y + 7, APP_LOGO_WIDTH, APP_LOGO_WIDTH);
    board_drawBorder(CALC_VERTICAL, &rect, 1);

    dtext(rect.x + APP_LOGO_WIDTH + 20 , rect.y + 40, COLOUR_BLACK, "Congratulations !!!");

    if (added){
        char out[100], score[10];
        strcpy(out, "New score : ");
        strcat(out, scores_time2a(time, score));
        strcat(out,"s");
        dtext(rect.x + APP_LOGO_WIDTH + 20 , rect.y + 60, COLOUR_RED, out);
    }

    dupdate();

    while (car != KEY_CODE_EXIT){
        car = getKey();
    }

    board_update(board, FALSE);
#endif // #ifdef DEST_CASIO_CALC
}

// EOF
