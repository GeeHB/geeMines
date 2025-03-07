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

// _onStartGame() : Start a new game
//
//  @board : pointer to the game board
//
//  @return : FALSE on error
//
BOOL _onStartGame(PBOARD const board){
    MENUACTION action;
    COORD pos = {0,0}, oPos = {0, 0};
    BOOL cont = TRUE, reDraw;
    POWNMENU gMenu = _createGameMenu();
    if (!gMenu){
        return FALSE;
    }

    board_setGameStateEx(board, STATE_PLAYING, TRUE);
    board_selectBox(board, &pos);
    menu_update(gMenu);

    while (cont){
        if (menu_handleKeyboard(gMenu, &action)){
            switch (action.value){

                // Change cursor pos
                //
                case KEY_CODE_LEFT:
                    reDraw = _onKeyLeft(board, &pos);
                    break;

                case KEY_CODE_DOWN:
                    reDraw = _onKeyDown(board, &pos);
                    break;

                case KEY_CODE_RIGHT:
                    reDraw = _onKeyRight(board, &pos);
                    break;

                case KEY_CODE_UP:
                    reDraw = _onKeyUp(board, &pos);
                    break;

                // Cancel (end) the game
                case IDM_CANCEL:
                    board_setGameStateEx(board, STATE_LOST, TRUE);
                    cont = FALSE;
                    break;

                default:
                    break;
            } // switch (action.value)

            if (reDraw){
                board_unselectBox(board, &oPos);
                board_selectBox(board, &pos);
                oPos = (COORD){.row = pos.row, .col = pos.col};

                _updateMenuState(board, menu_getMenuBar(gMenu), &pos);

#ifdef DEST_CASIO_CALC
                dupdate();
#endif // #ifdef DEST_CASIO_CALC
            }

        } // if (menu_handleKeyboard
    } // while (cont)

    // Game is over
    menu_free(gMenu);
    return TRUE;
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
//  @menuBar : Pointer to the menubar
//  @pos : position of cursor
//
void _updateMenuState(PBOARD const board, PMENUBAR menuBar, PCOORD pos){
}

// EOF
