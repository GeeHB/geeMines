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

#ifdef DEST_CASIO_CALC
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
        menubar_appendItem(bar, IDM_FLAG, IDS_FLAG, ITEM_STATE_INACTIVE, ITEM_STATUS_DEFAULT);
        menubar_appendItem(bar, IDM_QUESTION, IDS_QUESTION, ITEM_STATE_INACTIVE, ITEM_STATUS_DEFAULT);
        menubar_addItem(bar, MENU_POS_RIGHT, IDM_CANCEL, IDS_CANCEL, ITEM_STATE_DEFAULT, ITEM_STATUS_DEFAULT);
    }

    return menu;
}

#ifdef DEST_CASIO_CALC

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

    board_setGameStateEx(board, STATE_PLAYING, TRUE);
    board_selectBox(board, &pos);
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

        // A keyboard event ?
        if (menu_handleKeyboard(gMenu, &action)){
            switch (action.value){
                // Change cursor pos
                //
                case KEY_CODE_LEFT:
                    redraw != _onKeyLeft(board, &pos)?REDRAW_BOX:NO_REDRAW;
                    break;

                case KEY_CODE_DOWN:
                    redraw != _onKeyDown(board, &pos)?REDRAW_BOX:NO_REDRAW;
                    break;

                case KEY_CODE_RIGHT:
                    redraw != _onKeyRight(board, &pos)?REDRAW_BOX:NO_REDRAW;
                    break;

                case KEY_CODE_UP:
                    redraw != _onKeyUp(board, &pos)?REDRAW_BOX:NO_REDRAW;
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
                if (redraw & REDRAW_BOX){
                    board_unselectBox(board, &oPos);
                    board_selectBox(board, &pos);
                    oPos = (COORD){.row = pos.row, .col = pos.col}; // oPos = pos

                    _updateMenuItemsStates(board, gMenu, &pos);

                    if (!board->fullGrid){
                        board_drawViewPortButtons(board);
                    }
                }

                if (redraw & REDRAW_SELECTION){
                    hightLighted != hightLighted;// Blink selected box
                    board_selectBoxEx(board, &pos, hightLighted);
                }

                if (redraw & REDRAW_NAV_BUTTONS){
                    navButtons != navButtons;
                    board_drawViewPortButtonsEx(board, navButtons); // Blink nav buttons
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
//  @return : TRUE if position changed
//
BOOL _onKeyLeftEx(PBOARD const board, PCOORD pos, BOOL check){

    if (check && CALC_HORIZONTAL == board->orientation){
        return _onKeyUpEx(board, pos, FALSE);
    }

    if (pos->col){
        if (board->viewPort.visibleFrame.x == pos->col){
            board->viewPort.visibleFrame.x--;
        }

        pos->col--;
        return TRUE;
    }

    return FALSE;
}

// _onKeyDownEx() : User press "down" key
//
//  @board : pointer to the game board
//  @pos : position of cursor
//  @check : if TRUE check display orientation
//
//  @return : TRUE if position changed
//
BOOL _onKeyDownEx(PBOARD const board, PCOORD pos, BOOL check){

    if (check && CALC_HORIZONTAL == board->orientation){
        return _onKeyLeftEx(board, pos, FALSE);
    }

    if (pos->row < (board->viewPort.dimensions.row - 1)){
        if ((board->viewPort.visibleFrame.y + board->viewPort.visibleFrame.h - 1) == pos->row){
            board->viewPort.visibleFrame.y++;
        }

        pos->row++;
        return TRUE;
    }

    return FALSE;
}

// _onKeyRightEx() : User press "right" key
//
//  @board : pointer to the game board
//  @pos : position of cursor
//  @check : if TRUE check display orientation
//
//  @return : TRUE if position changed
//
BOOL _onKeyRightEx(PBOARD const board, PCOORD pos, BOOL check){
    if (check && CALC_HORIZONTAL == board->orientation){
        return _onKeyDownEx(board, pos, FALSE);
    }

    if (pos->col < (board->viewPort.dimensions.col - 1)){
        if ((board->viewPort.visibleFrame.x + board->viewPort.visibleFrame.w - 1) == pos->col){
            board->viewPort.visibleFrame.x++;
        }

        pos->col++;
        return TRUE;
    }

    return FALSE;
}

// _onKeyUpEx() : User press "up" key
//
//  @board : pointer to the game board
//  @pos : position of cursor
//  @check : if TRUE check display orientation
//
//  @return : TRUE if position changed
//
BOOL _onKeyUpEx(PBOARD const board, PCOORD pos, BOOL check){
    if (check && CALC_HORIZONTAL == board->orientation){
        return _onKeyRightEx(board, pos, FALSE);
    }

    if (pos->row){
        if (board->viewPort.visibleFrame.y == pos->row){
            board->viewPort.visibleFrame.y--;
        }

        pos->row--;
        return TRUE;
    }

    return FALSE;
}

// _updateMenuState() : Update state of items in the menu
//
//  @board : pointer to the game board
//  @menu : Pointer to the menu
//  @pos : position of cursor
//
void _updateMenuItemsStates(PBOARD const board, POWNMENU menu, PCOORD pos){
    PMENUBAR menuBar = menu_getMenuBar(menu);
    if (menuBar){
        PBOX box = BOX_AT_POS(board->grid, pos);
        BOOL flag = FALSE, question = FALSE, step = FALSE, greyAll = FALSE;

        switch (box->state){
            case BS_INITIAL:
                step = TRUE;
                break;

            case BS_FLAG:
                flag = TRUE;
                step = TRUE;
                break;

            case BS_QUESTION:
                question = TRUE;
                step = TRUE;
                break;

            case BS_MINE:
                step = TRUE;
                break;

            default:
                break;
        }

        if (greyAll){
            menubar_activateItem(menuBar, IDM_PRESS, SEARCH_BY_ID, step);
            menubar_setItemState(menuBar, IDM_FLAG, SEARCH_BY_ID, ITEM_STATE_UNCHECKED | ITEM_STATE_INACTIVE);
            menubar_setItemState(menuBar, IDM_QUESTION, SEARCH_BY_ID, ITEM_STATE_UNCHECKED | ITEM_STATE_INACTIVE);
        }
        else{
            menubar_activateItem(menuBar, IDM_PRESS, SEARCH_BY_ID, step);
            menubar_checkMenuItem(menuBar, IDM_FLAG, SEARCH_BY_ID, flag?ITEM_STATE_CHECKED:ITEM_STATE_UNCHECKED);
            menubar_checkMenuItem(menuBar, IDM_QUESTION, SEARCH_BY_ID, question?ITEM_STATE_CHECKED:ITEM_STATE_UNCHECKED);
        }

        menu_update(menu);
    }
}

#ifdef DEST_CASIO_CALC
// __callbackTick() : Call back function for timer
// This function is used during edition to make selected item blink
//
//  @pTick : pointer to blinking state indicator
//
//  @return : TIMER_CONTINUE if valid
//
int __callbackTick(volatile int *pTick){
    *pTick = 1;
    return TIMER_CONTINUE;
}
#endif // #ifdef DEST_CASIO_CALC

// EOF
