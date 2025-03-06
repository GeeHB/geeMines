//----------------------------------------------------------------------
//--
//--    geeMines.c
//--
//--            App. entry-point
//--
//----------------------------------------------------------------------

#include "consts.h"
#include "shared/menu.h"

#include "board.h"

#ifdef DEST_CASIO_CALC
    //#include "shared/casioCalcs.h"
    #include <string.h>

    extern bopti_image_t g_about;   // "about" image
    extern bopti_image_t g_pause;
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

    // Draw the image and copyright
    dimage(0, 0, &g_about);

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
            menubar_addItem(bar, MENU_POS_RIGHT, IDM_QUIT, IDS_QUIT, ITEM_STATE_DEFAULT, ITEM_STATUS_DEFAULT);
        }
        else{
            menu_free(menu);    // Error while allocating submenu => no more menu
            menu = NULL;
        }
    }

    return menu;
}

// Start a new game
//
void _onNewGame(PBOARD const board, uint8_t level){
    board_init(board, level);
    board_update(board);
}

int main(void){
#ifdef DEST_CASIO_CALC
    POWNMENU menu = _createMenu();
    PBOARD board = board_create();
    if (menu && board){
        BOOL end = FALSE;
        MENUACTION action;

        _about();
        menu_update(menu);

        while (!end){
            if (menu_handleKeyboard(menu, &action)){
                switch (action.value){
                    // Start a new game
                    case IDM_NEW_BEGINNER :
                    case IDM_NEW_MEDIUM :
                    case IDM_NEW_EXPERT:{
                        _onNewGame(board, action.value - IDM_NEW_BEGINNER);
                        menu_showParentBar(menu, TRUE);
                        menubar_activateItem(menu_getMenuBar(menu), IDM_START, SEARCH_BY_ID, TRUE);
                        break;
                    }

                    // Pause
                    case KEY_CODE_PAUSE:
                        _onPause();
                        //game_.display();
                        menu_update(menu);
                        break;

                    case IDM_QUIT :
                        end = TRUE;
                        break;

                    default :
                        break;
                } // swicth
            }
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
