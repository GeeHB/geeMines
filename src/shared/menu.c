//----------------------------------------------------------------------
//--
//--    menu.c
//--
//--        Implementation of menuBar - A bar of menu (or a submenu)
//--
//----------------------------------------------------------------------

#include "menu.h"
#include "keys.h"
#include <string.h>

#ifdef DEST_CASIO_CALC
// Background image
extern bopti_image_t g_menu;

// Images index in the image
//
#define MENU_IMG_BACK_ID        0
#define MENU_IMG_CHECKED_ID     1
#define MENU_IMG_UNCHECKED_ID   2
#endif // #ifndef DEST_CASIO_CALC

// menu_create() : Create a new menu
//
// @return : pointer to menu or NULL on error
//
POWNMENU menu_create(){
    POWNMENU menu =(POWNMENU)malloc(sizeof(OWNMENU));
    if (menu){
        menubar_clear(&menu->current_);
        menu->visible_ = &menu->current_; // show current bar
        menu_setHeight(menu, MENUBAR_DEF_HEIGHT, FALSE);

        // Default colours
        menu->colours_[COLOUR_TXT_SELECTED] = ITEM_COLOUR_SELECTED;
        menu->colours_[COLOUR_TXT_UNSELECTED] = ITEM_COLOUR_UNSELECTED;
        menu->colours_[COLOUR_TXT_INACTIVE] = ITEM_COLOUR_INACTIVE;
        menu->colours_[COLOUR_ITEM_BACKGROUND] = ITEM_COLOUR_BACKGROUND;
        menu->colours_[COLOUR_ITEM_BACKGROUND_SELECTED] = ITEM_COLOUR_BACKGROUND;
        menu->colours_[COLOUR_ITEM_BORDER] = ITEM_COLOUR_BORDER;
    }
    return menu;
}

// menu_free() : Free a menu
//
// @menu : pointer to the menu
//
void menu_free(POWNMENU menu){
    if (menu){
        free(menu);
    }
}

// menu_getMenuBar() : Get a pointer to the default menubar
//
//  @menu : pointer to the menu
//
//  @return : A pointer to the bar or NULL on error
//
PMENUBAR menu_getMenuBar(POWNMENU menu){
    return (menu?&menu->current_:NULL);
}

//  menu_getHeight() : Get menu height
//
//  @menu : Pointer to the menu
//
//  @return : Height of menu bar in pixels
//
uint16_t menu_getHeight(POWNMENU menu){
    if (menu){
        return menu->rect_.h;
    }

    return 0;
}

//  setHeight() : change menu bar height
//
//  @barHeight : New height in pixels
//  @updateBar : Update the menubar ?
//
//  @return : true if hieght has changed
//
BOOL menu_setHeight(POWNMENU menu, uint16_t barHeight, BOOL update){
    if (!menu){
        return FALSE;
    }

    SET_RECT(menu->rect_,0, CASIO_HEIGHT - barHeight, CASIO_WIDTH, barHeight);
    if (update){
        menu_update(menu);
    }
    return TRUE;
}

//  menu_getRect() : Get bounding rect of current menu bar
//
//  @menu : Pointer to the menu
//  @barRect : Pointer to a RECT struct. This struct. will contin the menu's dimensions and position
//
//  @return : TRUE if done
//
BOOL menu_getRect(POWNMENU menu, RECT* barRect){
    if (!menu || !barRect){
        return FALSE;
    }

    memcpy(barRect, &menu->rect_, sizeof(RECT));
    return TRUE;
}

//  menu_update() : Update the menu bar
//
//  All items will be drawn according to their state et status
//
//  @menu : Pointer to the menu
//
void menu_update(POWNMENU menu){
    if (menu){
        // First item's rect
        RECT anchor;
        SET_RECT(anchor, menu->rect_.x, menu->rect_.y, MENUBAR_DEF_ITEM_WIDTH, menu->rect_.h);

        //  Draw all items (even empty ones)
        for (uint8_t index = 0; index < MENUBAR_MAX_ITEM_COUNT; index++){
            menu_drawItem(menu, menu->visible_->items[index], &anchor);
            anchor.x += anchor.w;   // Next item's position
        } // for
#ifdef DEST_CASIO_CALC
        dupdate();
#else
        printf("\n");
#endif // #ifdef DEST_CASIO_CALC
    }
}

// menu_setMenuDrawingCallBack() : Set function for ownerdraw drawings
//
// When an item has the ITEM_STATUS_OWNERDRAWN status bit set, the
// ownerdraw callback function will be called each time the menubar
// needs to redraw the item
//
//  @menu : Pointer to the menu
//  @pF : Pointer to the callback function or NULL if no ownerdraw
//
//  @return : pointer to the default drawing function or NULL if not set
//
MENUDRAWINGCALLBACK menu_setMenuDrawingCallBack(POWNMENU menu, MENUDRAWINGCALLBACK pF){
    if (!menu){
        return NULL;
    }

    menu->drawingFunc = (void*)pF;
    return (MENUDRAWINGCALLBACK)menu_defDrawItem;   // Def. function if needed by calling function
}

//  menu_drawItem() : Draw an item
//
//  @menu : Pointer to the menu containing the item to be drawn
//  @item : Pointer to a MENUITEM strcut containing informations
//          concerning the item to draw
//  @anchor : Position of the item in screen coordinates
//
//  @return : FALSE on error(s)
//
BOOL menu_drawItem(POWNMENU menu, PMENUITEM const item, RECT* const anchor){
    if (NULL == menu || NULL == anchor){
        return FALSE;
    }

    MENUDRAWINGCALLBACK ownerFunction;
    if (item && isBitSet(item->status, ITEM_STATUS_OWNERDRAWN) &&
        NULL != (ownerFunction = ((MENUDRAWINGCALLBACK)menu->drawingFunc))){
        return ownerFunction(menu, item, anchor, MENU_DRAW_ALL); // Call ownerdraw func.
    }

    // Call default method
    return menu_defDrawItem(menu, item, anchor, MENU_DRAW_ALL);
}

//  menu_defDrawItem() : Draw an item
//
//  @menu : Pointer to the menu containing the item to be drawn
//  @item : Pointer to a MENUITEM strcut containing informations
//          concerning the item to draw
//  @anchor : Position of the item in screen coordinates
//  @style : Drawing style ie. element(s) to draw
//
//  @return : FALSE on error(s)
//
BOOL menu_defDrawItem(POWNMENU const menu, PMENUITEM const item, RECT* const anchor, int style){
    if (NULL == menu || NULL == anchor){
        return FALSE;
    }

#ifdef DEST_CASIO_CALC
    BOOL selected = FALSE;

    // Draw background
    if (isBitSet(style, MENU_DRAW_BACKGROUND)){
        drect(anchor->x, anchor->y, anchor->x + anchor->w - 1,
                anchor->y + anchor->h - 1,
                menu->colours_[COLOUR_ITEM_BACKGROUND]);
    }

    if (item){
        int colour;
        int x, w, h;
        int imgID = -1;  // No image

        selected = isBitSet(item->state, ITEM_STATE_SELECTED);

        // Text
        if (isBitSet(item->status, ITEM_STATUS_TEXT)){
            dsize(item->text, NULL, &w, &h);
        }

        // An image ?
        if (IDM_RESERVED_BACK == item->id){
            imgID = MENU_IMG_BACK_ID;
        }
        else{
            // Is the item a checkbox ?
            if (isBitSet(item->status, ITEM_STATUS_CHECKBOX)){
                imgID = (isBitSet(item->state, ITEM_STATE_CHECKED)?
                            MENU_IMG_CHECKED_ID:MENU_IMG_UNCHECKED_ID);
            }
        }

        if (imgID > -1){
            // New text position
            x = anchor->x + 2;

            // Draw the image on left of text
            if (isBitSet(style, MENU_DRAW_IMAGE)){
                dsubimage(x, anchor->y + (anchor->h - MENU_IMG_HEIGHT) / 2,
                        &g_menu, imgID * MENU_IMG_WIDTH,
                        0, MENU_IMG_WIDTH, MENU_IMG_HEIGHT, DIMAGE_NOCLIP);
            }
            x+=(MENU_IMG_WIDTH + 2);
        }
        else{
            x = anchor->x + (anchor->w - w) / 2;
        }

       if (isBitSet(style, MENU_DRAW_TEXT) &&
            isBitSet(item->status, ITEM_STATUS_TEXT)){

            // text too large ?

            // text colour ID
            colour = (selected?
                        COLOUR_TXT_SELECTED:
                        (isBitSet(item->state, ITEM_STATE_INACTIVE)?
                        COLOUR_TXT_INACTIVE:COLOUR_TXT_UNSELECTED));

            // draw the text
            dtext_opt(x, anchor->y + (anchor->h - h) / 2,
                    menu->colours_[colour],
                    menu->colours_[COLOUR_ITEM_BACKGROUND],
                    DTEXT_LEFT, DTEXT_TOP,
                     item->text, -1);
        }

        // Borders
        if (isBitSet(style, MENU_DRAW_BORDERS) && selected){
            colour = menu->colours_[COLOUR_ITEM_BORDER];
            dline(anchor->x, anchor->y,
                anchor->x, anchor->y + anchor->h - 2, colour); // Left
            dline(anchor->x+1, anchor->y + anchor->h - 1,
                anchor->x + anchor->w -1 - ITEM_ROUNDED_DIM,
                anchor->y + anchor->h - 1, colour);  // top
            dline(anchor->x + anchor->w -1 - ITEM_ROUNDED_DIM, // bottom
                anchor->y + anchor->h - 1,
                anchor->x + anchor->w - 1,
                anchor->y + anchor->h - 1 - ITEM_ROUNDED_DIM, colour);
            dline(anchor->x + anchor->w - 1, anchor->y,    // right
                anchor->x + anchor->w - 1,
                anchor->y + anchor->h - 1 - ITEM_ROUNDED_DIM, colour);
        }
    } // if (item)

    if (isBitSet(style, MENU_DRAW_BORDERS) && !selected){
        dline(anchor->x, anchor->y,
                anchor->x + anchor->w -1, anchor->y,
                menu->colours_[COLOUR_ITEM_BORDER]);
    }
#else
    if (item){
        printf("|");
        printf(isBitSet(item->state, ITEM_STATE_SELECTED)?">" : " ");
        if (isBitSet(item->status, ITEM_STATUS_CHECKBOX)){
            printf(isBitSet(item->state, ITEM_STATE_CHECKED)?"[x] ":"[ ] ");
        }

        if (isBitSet(item->status, ITEM_STATUS_TEXT)){
            if (isBitSet(item->state,ITEM_STATE_INACTIVE)){
                printf("_");
                printf("%s", item->text+1);
            }
            else {
                printf("%s", item->text);
            }
        }
        else{
            printf("[no text]");
        }

        printf(isBitSet(item->state,ITEM_STATE_SELECTED)?"<" : " ");
        printf("|");
    }
    else{
        printf("| [empty] |");
    }
#endif // #ifdef DEST_CASIO_CALC
    return TRUE;    // Done
}


// menu_getColour() : Get the colour used for item's drawings in the
//              active menu bar
//
//  @menu : Pointer to the menu
//  @index : index of the colour to retreive
//
//  @return : colour or -1 if error
//
int menu_getColour(POWNMENU menu, uint8_t index){
    return (menu?((index>=COLOUR_COUNT)?-1:menu->colours_[index]):-1);
}

// menu_setColour() : Change the colour used for item's drawings in the
//              active menu bar
//
//  @menu : Pointer to the menu
//  @index : index of the colour to change
//  @colour : new colour value
//
//  @return : previous colour or -1 if error
//
int menu_setColour(POWNMENU menu, uint8_t index, int colour){
    if (!menu || index>=COLOUR_COUNT){
        return -1;
    }

    // Change the colour
    uint8_t actual = menu->colours_[index];
    menu->colours_[index] = colour;
    return actual;
}

// menu_handleKeyboard() : Handle keyboard events
//
//  @menu : Pointer to the menu
//  @action : pointer a MENUACTION struct. It will ba filled with
//              infor!mation about item smected by the user
//
//  @return : FALSE on error
//
BOOL menu_handleKeyboard(POWNMENU menu, PMENUACTION action){
    if (!action || !menu){
        return FALSE;
    }

    uint modifier = MOD_NONE;
    uint key = KEY_NONE;
    uint8_t kID = 0;
    PMENUITEM item = NULL;
    BOOL readKeyboard = TRUE;

    menu_clearAction(action);

    if (readKeyboard){
        key = getKeyEx(&modifier);

        // a menu key pressed ?
        if (key >= KEY_F1 && key <= KEY_F6){
            kID = (key - KEY_F1);

            // Associated item
            if (kID < MENUBAR_MAX_ITEM_COUNT &&
                (item = menu->visible_->items[kID]) &&
                !isBitSet(item->state, ITEM_STATE_INACTIVE)){
                // A sub menu ?
                if (isBitSet(item->status, ITEM_STATUS_SUBMENU)){
                    if (item->subMenu){
                        menu->visible_ = (MENUBAR*)item->subMenu; // "visible" menu
                        menubar_selectByIndex(menu->visible_, -1, FALSE);
                        menu_update(menu);
                    }
                }
                else{
                    if (IDM_RESERVED_BACK == item->id){
                        menu_showParentBar(menu, TRUE);
                    }
                    else{
                        // select the item
                        if (kID != menu->visible_->selIndex){
                            menubar_selectByIndex(menu->visible_, kID, TRUE);
                        }

                        // A checkbox ?
                        if (isBitSet(item->status, ITEM_STATUS_CHECKBOX)){
                            if (isBitSet(item->state, ITEM_STATE_CHECKED)){
                                item->state = removeBit(item->state, ITEM_STATE_CHECKED);
                            }
                            else{
                                item->state = setBit(item->state, ITEM_STATE_CHECKED);
                            }
                        }

                        // a selectable item ...
                        action->value = item->id;
                        action->type = ACTION_MENU;
                        action->state = item->state;
                        readKeyboard = FALSE;
                    }
                }
            }

            menu_update(menu);
        } // if in [KEY_F1, KEY_F6]
        else{
            switch (key){
                // Back to prev. menu (if exists)
                case KEY_EXIT:
                    menu_showParentBar(menu, TRUE);
                    break;

                default :
                    action->value = key;
                    action->type = ACTION_KEYBOARD;
                    action->modifier = modifier;
                    readKeyboard = FALSE;
                    break;
            }
        }
    }

    action->modifier = modifier;

    return TRUE;
}

// menu_clearAction() : Clear the menu action struct.
//
//  @action : pointer a MENUACTION struct.
//
//  @return : TRUE if done
//
BOOL menu_clearAction(PMENUACTION action){
    if (action){
        action->value = 0;
        action->state = ITEM_STATE_DEFAULT;
        action->modifier = MOD_NONE;
        action->type = ACTION_KEYBOARD;
        return TRUE;
    }

    return FALSE;
}

// menu_showParentBar() : Return to parent menubar if exists
//
//  @menu : Pointer to the menu
//  @updateBar : update the menu ?
//
void menu_showParentBar(POWNMENU menu, BOOL updateBar){
    if (menu && menu->visible_ && menu->visible_->parent){
        menu->visible_ = menu->visible_->parent;
        menubar_selectByIndex(menu->visible_, -1, FALSE);
        if (updateBar){
            menu_update(menu);
        }
    }
}

//
// Menu bars
//

//  menubar_create() : Create an empty menubar
//
//  @return : Pointer to the new menubar or NULL
//
PMENUBAR menubar_create(){
    PMENUBAR bar =(PMENUBAR)malloc(sizeof(MENUBAR));

    if (NULL == bar){
        return NULL;
    }

    menubar_clear(bar); // Initialize
    return bar;
}

//  menubar_size() : count of items or sub menus in the bar
//
//  @bar : Pointer to the bar
//
//  @return : The count of items or sub menus
//
uint8_t menubar_size(PMENUBAR bar){
    return (bar?bar->itemCount:0);
}

//  menubar_clear() : Clear content of a menubar
//
//  @source : Pointer to the menubar
//
void menubar_clear(PMENUBAR bar){
    if (bar){
        // Bar is empty
        memset(bar, 0x00, sizeof(MENUBAR));
        bar->selIndex = -1;     // No item is selected
        memset(bar->items, 0x00, sizeof(PMENUITEM) * MENUBAR_MAX_ITEM_COUNT);
    }
}

//  menubar_copy() : Make a copy of a menu bar
//
//  All contained items and sub menus will be copied.
//
//  @source : Pointer to the source
//  @noBackButton : if TRUE, don't add a "back button" at the last position
//                  to return to previous menu
//
//  @return : Pointer to the new copy or NULL on error
//
PMENUBAR menubar_copy(PMENUBAR const source, BOOL noBackButton){
    PMENUBAR bar = NULL;
    if (source && (bar = (PMENUBAR)malloc(sizeof(MENUBAR)))){
        menubar_clear(bar);
        bar->itemCount = source->itemCount;
        bar->parent = source->parent;
        uint8_t max = (noBackButton?MENUBAR_MAX_ITEM_COUNT:(MENUBAR_MAX_ITEM_COUNT-1));
        PMENUITEM sitem, nitem;
        for (uint8_t index = 0; index < max; index++){
            nitem = menubar_copyItem(bar, (sitem = source->items[index]));
            bar->items[index] = nitem;  // simple item copy
        }

        if (!noBackButton){
            // In sub menus last right item is used to return to parent menu
            bar->items[MENUBAR_MAX_ITEM_COUNT-1] =
                    item_create(IDM_RESERVED_BACK, STR_RESERVED_BACK,
                                ITEM_STATE_DEFAULT, ITEM_STATUS_DEFAULT);
        }
    }
    return bar;
}

//  menubar_free() : Free memory used by a bar
//
//  @bar : Pointer to the bar to be released
//  @freeAll : if TRUE free the bar as well
//
void menubar_free(PMENUBAR bar, int freeAll){
    if (bar){
        for (uint8_t index = 0; index < MENUBAR_MAX_ITEM_COUNT; index++){
            menubar_freeMenuItem(bar, bar->items[index]);
        }

        if (freeAll){
            free(bar);  // free the bar
        }
    }
}

//  menubar_freeMenuItem() : Free memory used by a menu item
//
//  @bar : Pointer to the menu bar
//  @item : Pointer to the menu item to be released
//
void menubar_freeMenuItem(PMENUBAR bar, PMENUITEM item){
    if (bar && item){
        // A submenu ?
        if (isBitSet(item->status, ITEM_STATUS_SUBMENU) && item->subMenu){
            menubar_free((PMENUBAR)item->subMenu, TRUE);
        }

        free(item); // free the item
    }
}

//  menubar_addSubMenu() : Add a sub menu
//
//  @container : menubar container of the submenu
//  @index : index of position of the submenu
//  @submenu : submenu to add
//  @id : ID associated to the menu
//  @text : Submenu text
//  @state : initial state of submenu
//  @status : initial status of submenu
//
//  @return : pointer the created item or NULL
//
PMENUITEM menubar_addSubMenu(PMENUBAR const container, uint8_t index, PMENUBAR subMenu, int id, const char* text, int itemState, int status){
    size_t len;
    if (!container || !subMenu ||
        index >= MENUBAR_MAX_ITEM_COUNT ||
        NULL != container->items[index] ||
        menubar_findItem(container, id, SEARCH_BY_ID, NULL, NULL) ||
        container->itemCount == (MENUBAR_MAX_ITEM_COUNT - 1) ||
        ! text || !(len = strlen(text))){
        return NULL;
    }

    // Create a copy of the menu bar
    PMENUBAR sub = menubar_copy(subMenu, isBitSet(itemState, ITEM_STATE_NO_BACK_BUTTON));
    if (NULL == sub){
        return NULL;
    }

    // Create item
    PMENUITEM item = item_create(id, text, ITEM_STATE_DEFAULT, status);
    if (NULL == item){
        menubar_free(sub, TRUE);
        return NULL;
    }

    item->status = setBit(item->status, ITEM_STATUS_SUBMENU);
    item->subMenu = sub;
    sub->parent = container;

    container->items[index] = item;    // item pointing to the sub menu
    container->itemCount++;

    return item;
}

//  menubar_appendSubMenu() : Append a sub menu
//
//  @bar : Pointer to the bar
//  @submenu : menubar corresponding to the new submenu
//  @id : ID associated to the menu
//  @text : Submenu text
//  @state : Menu item initial state
//  @status : initial status of submenu
//
//  @return : pointer the created item or NULL
//
PMENUITEM menubar_appendSubMenu(PMENUBAR bar, const PMENUBAR subMenu, int id, const char* text, int state, int status){
    return menubar_addSubMenu(bar, bar->itemCount, (PMENUBAR)subMenu, id, text, state, status);
}

//  menubar_findItem() : Find an item in the given bar
//
//  @bar : menu bar containing to search item in
//  @searchedID : id or index of the searched item
//  @searchMode : Type of search (SEARCH_BY_ID or SEARCH_BY_INDEX)
//
//  @containerBar : pointer to a PMENUBAR. when not NULL,
//          if item is found, containerBar will point to the bar
//          containing the item
//  @pIndex : when not NULL, will point to the item's index in its menu
//
//  @return : pointer to the item if found or NULL
//
PMENUITEM menubar_findItem(PMENUBAR const bar, int searchedID, int searchMode, PMENUBAR* containerBar, int* pIndex){
    PMENUITEM item = NULL, sItem = NULL, foundItem = NULL;
    if (bar){
        if (SEARCH_BY_ID == searchMode){
            for (uint8_t index = 0;
                !foundItem && index < MENUBAR_MAX_ITEM_COUNT; index++){
                if ((item = bar->items[index])){
                    if (item->id == searchedID){    // items and sub-menus have IDs !!!
                        if (pIndex){
                            (*pIndex) = index;
                        }

                        foundItem = item;
                    }
                    else{
                        if (isBitSet(item->status, ITEM_STATUS_SUBMENU)){
                            // search in the sub-menu ?
                            if ((sItem = menubar_findItem((PMENUBAR)item->subMenu, searchedID, searchMode, containerBar, pIndex))){
                                foundItem = sItem;   // Found in a sub-menu
                            }
                        }
                    }
                } // if (item)
            } // for
        }
        else{
            if (SEARCH_BY_INDEX == searchMode){
                foundItem = bar->items[searchedID];
                if (foundItem && pIndex){
                    (*pIndex) = item?searchedID:0;
                }
            }
        }
    } // if (bar)

    if (foundItem && containerBar){
        (*containerBar) = (bar)?bar:NULL;
    }

    return foundItem;
}

//  menubar_getItem() : Find an item in the menu bar and its submenus
//
//  @bar : menu bar containing to search item in
//  @searchedID : id or index of the searched item
//  @searchMode : Type of search (SEARCH_BY_ID or SEARCH_BY_INDEX)
//  @containerBar : pointer to a PMENUBAR. when not NULL,
//          if item is found, containerBar will point to the bar
//          containing the item
//  @pIndex : when not NULL, will point to the item's index in its menu
//
//  @return : pointer to the item if found or NULL
//
PMENUITEM menubar_getItem(PMENUBAR bar, int searchedID, int searchMode, PMENUBAR* containerBar, int* pIndex){
    return (bar?menubar_findItem(bar, searchedID, searchMode, containerBar, pIndex) : NULL);
}

//  menubar_addItem() : Add an item to a menu bar
//
//  @bar : Pointer to the container bar
//  @index : Index (position) in the menu bar
//  @id : Item ID
//  @text : Item text
//  @state : Item's initial state
//  @status : Item's status
//
//  @return : pointer the created item or NULL
//
PMENUITEM menubar_addItem(PMENUBAR const bar, uint8_t index, int id, const char* text, int state, int status){
    if (!bar ||
        index >= MENUBAR_MAX_ITEM_COUNT ||
        NULL != bar->items[index] ||
        menubar_findItem(bar, id, SEARCH_BY_ID, NULL, NULL) ||    // already handled
        bar->itemCount >= MENUBAR_MAX_ITEM_COUNT /*||
        !text || !(len = strlen(text))*/){
        return NULL;
    }

    PMENUITEM item = item_create(id, text, state, status);
    if (item){
        // Add to the menu
        if (IDM_RESERVED_BACK == id){
            bar->items[MENUBAR_MAX_ITEM_COUNT-1] = item;
        }
        else{
            bar->items[index] = item;
            bar->itemCount++;
        }

        // Successfully added
        return item;
    }

    return NULL;
}

//  menubar_appendItem() : Append an item to the current menu bar
//
//  @bar : Pointer to the container bar
//  @id : Item ID
//  @text : Item text
//  @state : Item's initial state
//  @status : Item's status
//
//  @return : pointer the created item or NULL
//
PMENUITEM menubar_appendItem(PMENUBAR bar, int id, const char* text, int state, int status){
    return (bar?menubar_addItem(bar, bar->itemCount, id, text, state, status):NULL);
}

//  menubar_copyItem() : Make a copy of an item
//
//  @bar : Destination menu bar container
//  @source : Pointer to the source item
//
//  @return : pointer to the copied item or NULL
//
PMENUITEM menubar_copyItem(PMENUBAR const bar, PMENUITEM const source){
    PMENUITEM item = NULL;
    if (source && (item = (PMENUITEM)malloc(sizeof(MENUITEM)))){
        item->id = source->id;
        item->state = source->state;
        item->status = source->status;
        item->ownerData = source->ownerData;
        if (isBitSet(source->status, ITEM_STATUS_TEXT)){
            strcpy(item->text, source->text);
        }
        else{
            item->text[0] = '\0';
        }

        if (isBitSet(source->status, ITEM_STATUS_SUBMENU)){
            item->subMenu = menubar_copy((PMENUBAR)source->subMenu,
                        isBitSet(item->state, ITEM_STATE_NO_BACK_BUTTON));
            ((PMENUBAR)(item->subMenu))->parent = (void*)bar;
        }
        else{
            item->subMenu = NULL;
        }
    }
    return item;
}

//  menubar_removeItem() : Remove an item from the current menu bar
//      Remove the item menu or the submenu
//
//  @bar : menu bar in wich the item is to be searched
//  @searchedID : Item's ID or index
//  @searchMode : if SEARCH_BY_ID, searchedID is the ID of item
//                if SEARCH_BY_INDEX searchedID is the index in the menu bar
//
//  @return : non zero if the item has been successfully removed
//
BOOL menubar_removeItem(PMENUBAR const bar, int searchedID, int searchMode){
    PMENUITEM item = NULL;
    if (bar){
        switch(searchMode){

        case SEARCH_BY_INDEX:
            if (searchedID >= 0 && searchedID < MENUBAR_MAX_ITEM_COUNT){
                item = bar->items[searchedID];
                bar->items[searchedID] = NULL;

                // A sub menu ?
                if (isBitSet(item->status, ITEM_STATUS_SUBMENU)){
                    menubar_free((PMENUBAR)item->subMenu, TRUE);
                }

                // Position is free
                free(item);
                bar->itemCount--;
                return TRUE;
            }
            break;

        case SEARCH_BY_ID:{
            MENUBAR* subBar;
            int index;

            // Item exists ?
            if ((item = menubar_findItem(bar, searchedID, SEARCH_BY_ID, &subBar, &index))){
                // Yes => remove it
                return menubar_removeItem(subBar, index, SEARCH_BY_INDEX);
            }
            break;
        }

        default:
            break;
        }
    }

    // not removed
    return FALSE;
}

//  menubar_selectByIndex() : Select an item by index in the current bar
//
//  @bar : pointer to the bar
//  @index : index of menu item to select or unselect
//  @selected : TRUE if item is to be selected
//
//  @return : TRUE if item is selected
//
BOOL menubar_selectByIndex(PMENUBAR bar, int index, BOOL selected){
    int sel;
    if (!bar||
        index >= MENUBAR_MAX_ITEM_COUNT ||
        index == (sel = menubar_getSelectedIndex(bar))){
        return FALSE;   // Nothing to do
    }

    if (-1 == index){
        index = sel;
        selected = FALSE;
    }

    // Select or unselect an item
    if (index >= 0){
        // Item
        PMENUITEM item = bar->items[index];
        if (NULL == item){
            return FALSE;
        }

        if (selected){
            // unselect prev.
            menubar_unSelectItems(bar);

            item->state = setBit(item->state, ITEM_STATE_SELECTED);
            bar->selIndex = index;
        }
        else{
            item->state = removeBit(item->state, ITEM_STATE_SELECTED);
            bar->selIndex = -1;
        }
    }

    // Done
    return TRUE;
}

//  menu_unSelectItems() : Unselect item(s)
//
//  @bar : Pointer to the bar
//
void menubar_unSelectItems(PMENUBAR bar){
    if (bar){
        PMENUITEM item;
        for (uint8_t index = 0; index < MENUBAR_MAX_ITEM_COUNT; index++){
            if ((item = bar->items[index])){
                item->state = removeBit(item->state, ITEM_STATE_SELECTED);
            }
        }
    }
}

//  menubar_getSelectedIndex() : Index of selected item in the current bar
//
//  @bar : Pointer to the bar
//
//  @return : Index of item selected or -1 if none
//
int menubar_getSelectedIndex(PMENUBAR bar){
    return (bar?bar->selIndex:-1);
}

//  menubar_getItemState() : Get the state of an item
//
//  @bar : Pointer to the bar
//  @searchedID : ID of searched item
//  @searchMode : type of search (SEARCH_BY_ID or SEARCH_BY_INDEX)
//
//  @return : Item's state or -1 if error
//
int menubar_getItemState(PMENUBAR bar, int searchedID, int searchMode){
    if (!bar){
        return -1;
    }

    PMENUITEM item = menubar_findItem(bar, searchedID, searchMode, NULL, NULL);
    return (item?item->state:-1);
}

//  menubar_activateItem() : Activate or deactivate an item
//
//  When an item is deactivated, it can't be called by the user
//
//  @bar : Pointer to the bar
//  @searchedID : Menu item's ID
//  @searchMode : Type of search (SEARCH_BY_ID or SEARCH_BY_INDEX)
//  @activated : true if item must be activated
//
//  @return : TRUE if activation state changed
//
BOOL menubar_activateItem(PMENUBAR bar, int searchedID, int searchMode, BOOL activated){
    if (bar){
        PMENUBAR container = NULL;
        PMENUITEM item = menubar_findItem(bar, searchedID, searchMode, &container, NULL);
        if (item){
            // Found an item with this ID
            BOOL active = !isBitSet(item->state, ITEM_STATE_INACTIVE);
            if (active != activated){
                // change item's state
                if (activated){
                    item->state = removeBit(item->state, ITEM_STATE_INACTIVE);
                }
                else{
                    item->state = setBit(item->state, ITEM_STATE_INACTIVE);

                    // an inactivate item can't be selected !
                    item->state = removeBit(item->state, ITEM_STATE_SELECTED);
                    if (container){
                        container->selIndex = -1;
                    }
                }

                return TRUE;
            }
        }
    }

    return FALSE;
}

BOOL menubar_activate(PMENUBAR bar, int searchedID, int searchMode, BOOL activated){
    return menubar_activateItem(bar, searchedID, searchMode, activated);
}

// menubar_isMenuItemActivated() : Check wether an item is activated or not
//
//  @bar : Pointer to the bar
//  @id : item id
//  @searchMode : type of search (SEARCH_BY_ID or SEARCH_BY_INDEX)
//
//  return : TRUE if the item is activted
//
BOOL menubar_isMenuItemActivated(PMENUBAR bar, int id, int searchMode){
    if (bar){
        PMENUITEM item = menubar_findItem(bar, id, searchMode, NULL, NULL);
        return ((item && !isBitSet(item->state, ITEM_STATE_INACTIVE))?TRUE:FALSE);
    }

    return FALSE;
}


//  menubar_addCheckbox() : Add an item to the current menu bar
//
//  @bar : Pointer to the bar
//  @index : Index (position) in the menu bar
//  @id : Item ID
//  @text : Item text
//  @state : Item's initial state
//  @status : Item's status
//
//  @return : pointer the created item or NULL
//
PMENUITEM menubar_addCheckBox(PMENUBAR bar, int index, int id, const char* text, int state, int status){
    return (bar?menubar_addItem(bar, index, id, text, state, ITEM_STATUS_CHECKBOX | status):NULL);
}

//  menubar_appendCheckbox() : Append a checkbox to the current menu bar
//
//  @bar : Pointer to the bar
//  @id : Item ID
//  @text : Item text
//  @state : Item's initial state
//  @status : Item's status
//
//  @return : pointer the created item or NULL
//
PMENUITEM menubar_appendCheckbox(PMENUBAR bar, int id, const char* text, int state, int status){
    return (bar?menubar_addItem(bar, bar->itemCount, id, text, state,  status | ITEM_STATUS_CHECKBOX) : NULL);
}

// menubar_isMenuItemChecked() : Check wether a checkbox is in the checked state
//
//  @bar : Pointer to the bar
//  @id : checkbox item id
//  @searchMode : type of search (SEARCH_BY_ID or SEARCH_BY_INDEX)
//
//  return : ITEM_CHECKED if the item is checked, ITEM_UNCHECKED
//          if the item is not cheched ITEM_ERROR
//          on error (invalid id, not a check box, ...)
//
int menubar_isMenuItemChecked(PMENUBAR bar, int id, int searchMode){
    if (bar){
        PMENUITEM item = menubar_findItem(bar, id, searchMode, NULL, NULL);
        if (item && isBitSet(item->status, ITEM_STATUS_CHECKBOX)){
            return (isBitSet(item->state,
                    ITEM_STATE_CHECKED)?ITEM_CHECKED:ITEM_UNCHECKED);
        }
    }

    // Not found or not a checkbox
    return ITEM_ERROR;
}

// menubar_checkMenuItem() : Check or uncheck a menu item checkbox
//
//  @bar : Pointer to the bar
//  @id : checkbox item id
//  @searchMode : type of search (SEARCH_BY_ID or SEARCH_BY_INDEX)
//  @checkState : ITEM_CHECKED if item should be checked or ITEM_UNCHECKED
//
//  return : ITEM_CHECKED if item is checked, ITEM_UNCHECKED if not checked
//           and ITEM_ERROR on error
//
int menubar_checkMenuItem(PMENUBAR bar, int id, int searchMode, int checkState){
    if (bar){
        PMENUITEM item = menubar_findItem(bar, id, searchMode, NULL, NULL);
        if (item && isBitSet(item->status, ITEM_STATUS_CHECKBOX)){
            if (ITEM_CHECKED == checkState){
                item->state = setBit(item->state, ITEM_STATE_CHECKED);
            }
            else{
                item->state = removeBit(item->state, ITEM_STATE_CHECKED);
            }

            // return tatus of bit
            return (isBitSet(item->state,
                    ITEM_STATE_CHECKED)?ITEM_CHECKED:ITEM_UNCHECKED);
        }
    }

    // Not found or not a checkbox
    return ITEM_ERROR;
}


//
// items
//

//  item_create() : creae a new menu item
//
//  @id : Item's id
//  @text : Menu item text
//  @state : Item's initial state
//  @status : Item's status
//
//  @return : pointer to the new created if valid or NULL
//
PMENUITEM item_create(int id, const char* text, int state, int status){
    size_t len = (text?strlen(text):0);
    MENUITEM* item = NULL;
    if ((item = (PMENUITEM)malloc(sizeof(MENUITEM)))){
        memset(item, 0x00, sizeof(MENUITEM));
        item->id = id;
        item->state = state;
        item->status = status;
        item->status = setBit(item->status, ITEM_STATUS_TEXT);
        if (len > ITEM_NAME_LEN){
            strncpy(item->text, text, ITEM_NAME_LEN);
            item->text[ITEM_NAME_LEN] = '\0';
        }
        else{
            if (text){
                strcpy(item->text, text);
            }
            else{
                item->text[0]='\0';
                item->status = removeBit(item->status, ITEM_STATUS_TEXT);
            }
        }
    }

    return item;
}

//  item_free() : creae a new menu item
//
//  @item : pointer to item
//
void item_free(PMENUITEM item){
    if (item){
        free(item);
    }
}
// EOF
