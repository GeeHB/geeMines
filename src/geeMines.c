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
    drect(0, 0, CASIO_WIDTH - 1, CASIO_HEIGHT - MENUBAR_DEF_HEIGHT - 1, C_WHITE);

    // Draw the image and copyright
    dimage(0, 0, &g_about);

    char copyright[255];    // Should be enough !
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

    // Top of image
    dsubimage(0, 0, &g_pause,
            0, 0, IMG_PAUSE_W, IMG_PAUSE_COPY_Y, DIMAGE_NOCLIP);

    // "middle"
    uint16_t y;
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
            menubar_appendItem(bar, IDM_PRESS, IDS_PRESS, ITEM_STATE_INACTIVE, ITEM_STATUS_DEFAULT);
            menubar_appendItem(bar, IDM_FLAG, IDS_FLAG, ITEM_STATE_UNCHECKED | ITEM_STATE_INACTIVE, ITEM_STATUS_CHECKBOX);
            menubar_appendItem(bar, IDM_QUESTION, IDS_QUESTION, ITEM_STATE_UNCHECKED | ITEM_STATE_INACTIVE, ITEM_STATUS_CHECKBOX);
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

    if (menu){
        _about();
        menu_update(menu);

        //game_start(menu);

        // Quit app.
        //
        menu_free(menu);
    }

//gint_setrestart(0);
gint_osmenu();

#endif // #ifdef DEST_CASIO_CALC
	return 1;
}
