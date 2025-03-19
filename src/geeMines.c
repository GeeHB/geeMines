//----------------------------------------------------------------------
//--
//--    geeMines.c
//--
//--            App. entry-point
//--
//----------------------------------------------------------------------

#include "consts.h"
#include "shared/menu.h"

#ifdef DEST_CASIO_CALC
    #include "game.h"
    //#include "shared/casioCalcs.h"
    #include <string.h>

    extern bopti_image_t g_mine;   // "about" image
#endif // #ifdef DEST_CASIO_CALC

//
// Functions
//

// _about() : Show "about" informations
//
void _about(){
#ifdef DEST_CASIO_CALC
    int w, h;
    char copyright[255];    // Should be enough !
    drect(0, 0, CASIO_WIDTH - 1, CASIO_HEIGHT - MENUBAR_DEF_HEIGHT - 1, C_WHITE);

    // Draw the logo
    dimage((CASIO_WIDTH - APP_LOGO_WIDTH) / 2,
            (CASIO_HEIGHT - MENUBAR_DEF_HEIGHT - APP_LOGO_WIDTH) / 2,
            &g_mine);

    // Copyright
    strcpy(copyright, APP_NAME);
    strcat(copyright, " par ");
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
            menubar_appendItem(bar, IDM_DEBUG, IDS_DEBUG, ITEM_STATE_CHECKED, ITEM_STATUS_CHECKBOX);
#endif // #ifdef _DEBUG_
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
#ifdef DEST_CASIO_CALC
    POWNMENU menu = _createMenu();
    PBOARD board = board_create();
    if (menu && board){
        BOOL end = FALSE;
        MENUACTION action;

        _about();

        // Main menu
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
                        _onStartGame(board);
                        menu_update(menu);  // back to current menu
                        break;

#ifdef _DEBUG_
                    case IDM_DEBUG:
                        board->debug = !board->debug;
                        board_update(board);

                        menubar_checkMenuItem(menu_getMenuBar(menu), IDM_DEBUG, SEARCH_BY_ID, board->debug?ITEM_CHECKED:ITEM_UNCHECKED);
                        menu_update(menu);
                        break;
#endif // #ifdef _DEBUG_

                    case KEY_CODE_PAUSE:
                        _onPause();
                        board_update(board);    // update screen and menu
                        menu_update(menu);
                        break;

                    case KEY_CODE_ROTATE_DISPLAY:
                        board_setOrientation(board, (CALC_VERTICAL == board->orientation)?CALC_HORIZONTAL:CALC_VERTICAL);
                        board_update(board);
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

        // Quit app.
        //
        menu_free(menu);
    }   // if (menu)

    if (board){
        board_free(board, TRUE);
    }

    //gint_setrestart(0);
    gint_osmenu();

#endif // #ifdef DEST_CASIO_CALC
	return 1;
}

// EOF
