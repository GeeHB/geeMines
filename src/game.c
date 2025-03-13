//----------------------------------------------------------------------
//--
//--    game.c
//--
//--            the game !
//--
//----------------------------------------------------------------------

#include "game.h"
#include "board.h"
#include "shared/menu.h"
#include <stdint.h>

#ifdef DEST_CASIO_CALC
#include <gint/clock.h>

extern bopti_image_t g_pause;
#endif // #ifdef DEST_CASIO_CALC

// _onNewGame() : Create a new game
//
//  @board : pointer to the game board
//  @level : game difficulty level
//
void _onNewGame(PBOARD const board, uint8_t level){
    board_init(board, level);
    board_setGameState(board, STATE_WAITING);
    board_update(board);
}

// _createGameMenu : Create the game menu
//
//  return : Pointer the the menu (or NULL on error)
//
POWNMENU _createGameMenu(){
    POWNMENU menu = menu_create();
    if (menu){
        // Create menu bar
        PMENUBAR bar = menu_getMenuBar(menu);
        menubar_appendItem(bar, IDM_PRESS, IDS_PRESS, ITEM_STATE_INACTIVE, ITEM_STATUS_DEFAULT);
        menubar_appendItem(bar, IDM_FLAG, IDS_FLAG, ITEM_STATE_INACTIVE, ITEM_STATUS_CHECKBOX);
        menubar_appendItem(bar, IDM_QUESTION, IDS_QUESTION, ITEM_STATE_INACTIVE, ITEM_STATUS_CHECKBOX);
        menubar_addItem(bar, MENU_POS_RIGHT, IDM_CANCEL, IDS_CANCEL, ITEM_STATE_DEFAULT, ITEM_STATUS_DEFAULT);
    }

    return menu;
}

#ifdef DEST_CASIO_CALC

// __callbackTick() : Call back function for timer
// This function is used during edition to make selected item blink
//
//  @pTick : pointer to blinking state indicator
//
//  @return : TIMER_CONTINUE if valid
//
static int __callbackTick(volatile int *pTick){
    *pTick = 1;
    return TIMER_CONTINUE;
}

// _onStartGame() : Start a new game
//
//  @board : pointer to the game board
//
//  @return : FALSE on error
//
BOOL _onStartGame(PBOARD const board){
    POWNMENU gMenu = _createGameMenu();
    if (!gMenu){
        return FALSE;
    }

    MENUACTION action;
    COORD pos = {0,0}, oPos = {0, 0};
    BOOL cont = TRUE, navButtons = FALSE, hightLighted = FALSE;
    uint8_t redraw = NO_REDRAW;

    board_setGameStateEx(board, STATE_PLAYING, FALSE);
    board_selectBox(board, &pos);

    _updateMenuItemsStates(board, gMenu, &pos);
    menu_update(gMenu);

    // Timer for blinking effect
    int tickCount = 0;
    static volatile int tick = 1;
    int timerID = timer_configure(TIMER_ANY, TIMER_TICK_DURATION * 1000,
                                    GINT_CALL(__callbackTick, &tick));
    if (timerID >= 0){
        timer_start(timerID);   // set the timer
    }
    else{
        cont = FALSE;   // No timer => no game
    }

    while (cont){
        // Time management
        while(!tick){
            sleep();
        }
        tick = 0;

        // Time to blink ?
        tickCount++;

        if (0 == (tickCount % BLINK_CURSOR)){
            redraw |= REDRAW_SELECTION;
        }

        if (0 == (tickCount % TIMER_SECOND)){
            redraw |= REDRAW_TIME;
        }

        if (0 == (tickCount % BLINK_NAV_BUTTONS)){
            redraw |= REDRAW_NAV_BUTTONS;
        }

        // A keyboard event ?
        if (menu_handleKeyboard(gMenu, &action)){
            switch (action.value){
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

                case IDM_FLAG:
                    redraw = onFlag(board, menu, pos);
                    break;

                case IDM_QUESTION:
                    redraw = onQuestion(board, menu, pos);
                    break;

                // Pause
                case KEY_CODE_PAUSE:
                    _onPause();
                    board_update(board);    // update screen and menu
                    menu_update(gMenu);
                    break;

                // Cancel (end) the game
                case IDM_CANCEL:
                    board_setGameStateEx(board, STATE_LOST, TRUE);
                    cont = FALSE;
                    break;

                default:
                    break;
            } // switch (action.value)

            if (redraw != NO_REDRAW){

                if (redraw & REDRAW_GRID){
                    board_drawEx(board, FALSE); // no screen update
                }

                if (redraw & REDRAW_BOX){
                    POINT scrPos;
                    board_Pos2Point(board, &pos, &scrPos);
                    board_drawBox(board, &pos, srcPos.x, srcPos.y);
                }

                if (redraw & REDRAW_MOVE){
                    board_unselectBox(board, &oPos);
                    board_selectBox(board, &pos);
                    oPos = (COORD){.row = pos.row, .col = pos.col}; // oPos = pos

                    _updateMenuItemsStates(board, gMenu, &pos);
                    menu_updateEx(gMenu, FALSE);

                    if (!board->fullGrid){
                        board_drawViewPortButtons(board);
                    }
                }

                if (redraw & REDRAW_SELECTION){
                    hightLighted = !hightLighted;// Blink selected box
                    board_selectBoxEx(board, &pos, hightLighted);
                }

                if (redraw & REDRAW_NAV_BUTTONS){
                    navButtons = !navButtons;
                    board_drawViewPortButtonsEx(board, navButtons, FALSE); // Blink nav buttons
                }

                if (redraw & REDRAW_TIME){
                    cont = board_incTime(board);
                    board_drawTimeEx(board, FALSE);     // Time has changed
                }

                dupdate();
                redraw = NO_REDRAW;
            } // if (reDraw)
        } // if (menu_handleKeyboard
    } // while (cont)

    // stop the timer
    if (timerID >= 0){
        timer_stop(timerID);
    }

    // Display end status board
    board_draw(board);
    menu_free(gMenu);

    return TRUE;
}
#endif // #ifdef DEST_CASIO_CALC

// _onFlag() : Put / remove a flag
//
//  @board : pointer to the current board
//  @pos : Current position in the grid
//  @menu : Pointer to the menu
//
//  @return drawing action to perform or NO_DRAWING
//
uint8_t _onFlag(PBOARD const board, POWNMENU const menu, PCOORD const pos){
    PBOX box = BOX_AT_POS(board->grid, pos);
    BOOL flag = (box->state == BS_FLAG);
    box->state = (flag?BS_INITIAL:BS_FLAG);
    menubar_checkMenuItem(menu_getMenuBar(menu), IDM_FLAG, SEARCH_BY_ID, flag?ITEM_STATE_UNCHECKED:ITEM_STATE_CHECKED);
    return REDRAW_BOX;
}

// _onQuestion() : Put / remove a 'question' attribute to the box
//
//  @board : pointer to the current board
//  @pos : Current position in the grid
//  @menu : Pointer to the menu
//
//  @return drawing action to perform or NO_DRAWING
//
uint8_t _onQuestion(PBOARD const board, POWNMENU const menu, PCOORD const pos){
    PBOX box = BOX_AT_POS(board->grid, pos);
    BOOL question = (box->state == BS_FLAG);
    box->state = (question?BS_INITIAL:BS_QUESTION);
    menubar_checkMenuItem(menu_getMenuBar(menu), IDM_QUESTION, SEARCH_BY_ID, question?ITEM_STATE_UNCHECKED:ITEM_STATE_CHECKED);
    return REDRAW_BOX;
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

// _updateMenuState() : Update state of items in the menu
//
//  @board : pointer to the game board
//  @menu : Pointer to the menu
//  @pos : position of cursor
//
void _updateMenuItemsStates(PBOARD const board, POWNMENU const menu, PCOORD const pos){
    PMENUBAR menuBar = menu_getMenuBar(menu);
    if (menuBar){
        PBOX box = BOX_AT_POS(board->grid, pos);
        BOOL flag = FALSE, question = FALSE, step = FALSE, greyAll = FALSE;
        BOOL fState = FALSE, qState = FALSE;
        SMILEY_STATE smiley = SMILEY_HAPPY, current = board->smileyState;

        switch (box->state){
            case BS_INITIAL:
                step = TRUE;
                flag = TRUE;
                question = TRUE;
                smiley = SMILEY_CAUTION;                break;

            case BS_FLAG:
                step = TRUE;
                flag = TRUE;
                fState = TRUE;
                break;

            case BS_QUESTION:
                step = TRUE;
                question = TRUE;
                qState = TRUE;
                break;

            case BS_MINE:
                step = TRUE;
                break;

            default:
                greyAll = TRUE;
                break;
        }

        if (greyAll){
            menubar_activateItem(menuBar, IDM_PRESS, SEARCH_BY_ID, step);
            menubar_setItemState(menuBar, IDM_FLAG, SEARCH_BY_ID, ITEM_STATE_UNCHECKED | ITEM_STATE_INACTIVE);
            menubar_setItemState(menuBar, IDM_QUESTION, SEARCH_BY_ID, ITEM_STATE_UNCHECKED | ITEM_STATE_INACTIVE);
        }
        else{
            menubar_activateItem(menuBar, IDM_PRESS, SEARCH_BY_ID, step);
            menubar_activateItem(menuBar, IDM_FLAG, SEARCH_BY_ID, flag);
            menubar_checkMenuItem(menuBar, IDM_FLAG, SEARCH_BY_ID, fState?ITEM_STATE_CHECKED:ITEM_STATE_UNCHECKED);
            menubar_activateItem(menuBar, IDM_QUESTION, SEARCH_BY_ID, question);
            menubar_checkMenuItem(menuBar, IDM_QUESTION, SEARCH_BY_ID, qState?ITEM_STATE_CHECKED:ITEM_STATE_UNCHECKED);
        }

        if (current != smiley){
            board_setSmileyEx(board, smiley, FALSE);
        }
    }
}

// EOF
