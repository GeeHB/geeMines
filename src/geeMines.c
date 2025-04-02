//----------------------------------------------------------------------
//--
//--    geeMines.c
//--
//--            App. entry-point
//--
//----------------------------------------------------------------------

#include "consts.h"
#include "shared/menu.h"

#include "game.h"

//
// Functions
//

// _createMenu : Create the applicaiton menu
//
//  return : Pointer the the menu (or NULL on error)
//
POWNMENU _createMenu(){
    POWNMENU menu = menu_create();
    if (menu){
        PMENUBAR sub = menubar_create();
        if (sub){
            // Fill the sub-menu
            menubar_appendItem(sub, IDM_NEW_BEGINNER, IDS_NEW_BEGINNER, ITEM_STATE_DEFAULT, ITEM_STATUS_TEXT);
            menubar_appendItem(sub, IDM_NEW_MEDIUM, IDS_NEW_MEDIUM, ITEM_STATE_DEFAULT, ITEM_STATUS_TEXT);
            menubar_appendItem(sub, IDM_NEW_EXPERT, IDS_NEW_EXPERT, ITEM_STATE_DEFAULT, ITEM_STATUS_TEXT);

            // Create menu bar
            PMENUBAR bar = menu_getMenuBar(menu);
            menubar_appendSubMenu(bar, sub, IDM_NEW, IDS_NEW, ITEM_STATE_DEFAULT, ITEM_STATUS_DEFAULT);
            menubar_appendItem(bar, IDM_START, IDS_START, ITEM_STATE_INACTIVE, ITEM_STATUS_DEFAULT);
#ifdef _DEBUG_
            menubar_appendItem(bar, IDM_DEBUG, IDS_DEBUG, ITEM_STATE_UNCHECKED, ITEM_STATUS_CHECKBOX);
#endif // #ifdef _DEBUG_
            menubar_addItem(bar, MENU_POS_RIGHT - 1, IDM_SCORES, IDS_SCORES, ITEM_STATE_DEFAULT, ITEM_STATUS_DEFAULT);
            menubar_addItem(bar, MENU_POS_RIGHT, IDM_QUIT, IDS_QUIT, ITEM_STATE_DEFAULT, ITEM_STATUS_DEFAULT);
        }
        else{
            menu_free(menu);    // Error while allocating submenu => no more menu
            menu = NULL;
        }
    }

    return menu;
}

int main(void){
//#ifdef DEST_CASIO_CALC
    POWNMENU menu = _createMenu();
    PBOARD board = board_create();
    if (menu && board){
        BOOL end = FALSE;
        MENUACTION action;
        PSCORE scores = NULL;
#ifdef SCREEN_CAPTURE
        BOOL captureOn = FALSE;
#endif // #ifdef SCREEN_CAPTURE

        _onAbout();

        if (NULL != (scores = scores_load())){
            menubar_activateItem(menu_getMenuBar(menu), IDM_SCORES, SEARCH_BY_ID, TRUE);
        }

        menu_update(menu);

        while (!end){
            if (menu_handleKeyboard(menu, &action)){
                switch (action.value){
                    // Start a new game
                    case IDM_NEW_BEGINNER :
                    case IDM_NEW_MEDIUM :
                    case IDM_NEW_EXPERT:
                        _onNewGame(board, action.value - IDM_NEW_BEGINNER);

                        menu_showParentBar(menu, TRUE);
                        menubar_activateItem(menu_getMenuBar(menu), IDM_START, SEARCH_BY_ID, TRUE);
                        menu_update(menu);
                        break;

                    // Start the game
                    case IDM_START:
#ifdef SCREEN_CAPTURE
                        _onStartGame(board, scores, captureOn);
#else
                        _onStartGame(board, scores);
#endif // #ifdef SCREEN_CAPTURE
                        menubar_activateItem(menu_getMenuBar(menu), IDM_START, SEARCH_BY_ID, FALSE);
                        menu_update(menu);  // back to current menu
                        break;

#ifdef _DEBUG_
                    case IDM_DEBUG:
                        board->debug = !board->debug;
                        board_update(board, TRUE);

                        menubar_checkMenuItem(menu_getMenuBar(menu), IDM_DEBUG, SEARCH_BY_ID, board->debug?ITEM_CHECKED:ITEM_UNCHECKED);
                        menu_update(menu);
                        break;
#endif // #ifdef _DEBUG_

#ifdef SCREEN_CAPTURE
                    case KEY_CODE_CAPTURE:
                        if (captureOn){
                            capture_remove();
                        }
                        else{
                            capture_install();
                      		menu_update(menu);
                        }

                        captureOn = !captureOn;
                        break;
#endif // #ifdef SCREEN_CAPTURE

                    case KEY_CODE_PAUSE:
                        _onPause();
                        board_update(board, TRUE);    // update screen and menu
                        menu_update(menu);
                        break;

                    case KEY_CODE_ROTATE_DISPLAY:
                        board_setOrientation(board, (CALC_VERTICAL == board->orientation)?CALC_HORIZONTAL:CALC_VERTICAL);
                        board_drawEx(board, TRUE, FALSE);
                        menu_update(menu);
                        break;

                    // Show high scores
                    case IDM_SCORES:
                        _onShowScores(scores, LEVEL_BEGINNER);
                        _onAbout();
                        menu_update(menu);
                        break;

                    // End app.
                    case IDM_QUIT :
                        end = TRUE;
                        break;

                    default :
                        break;
                } // swicth
            } // menu_handleKeyboard
        } // while (!end)

        menu_free(menu);

        if (scores){
            scores_save(scores);
            scores_free(scores);
        }

    }   // if (menu)

    if (board){
        board_free(board, TRUE);
    }

#ifdef DEST_CASIO_CALC
    //gint_setrestart(0);
    gint_osmenu();
#endif // #ifdef DEST_CASIO_CALC

//#endif // #ifdef DEST_CASIO_CALC
	return 1;
}

// EOF
