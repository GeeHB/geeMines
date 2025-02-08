//----------------------------------------------------------------------
//--
//--    menu.c
//--
//--        Implementation of menuBar - A bar of menu (or a submenu)
//--
//----------------------------------------------------------------------

#include "menu.h"

#ifndef DEST_CASIO_CALC
#include "locals.h"
#else
// Background image
extern bopti_image_t g_menu;

// Images index in the image
enum MENU_IMG_INDEX{
    MENU_IMG_BACK_ID = 0,
    MENU_IMG_CHECKED_ID = 1,
    MENU_IMG_UNCHECKED_ID = 2
};
#endif // #ifndef DEST_CASIO_CALC

#include <stdlib.h>

//
// Internal functions definition
//

// menu
//
BOOL menu_drawItem(POWNMENU const menu, PMENUITEM const item, RECT* const anchor);

// Menubar handling
//
void menubar_clear(PMENUBAR bar);
PMENUBAR menubar_copy(PMENUBAR const source, int noBackButton);
void menubar_free(PMENUBAR bar, int freeAll);
BOOL menubar_defDrawItem(PMENUBAR const bar, PMENUITEM const item, RECT* const anchor, int style);
PMENUITEM menubar_findItem(PMENUBAR const bar, int searchedID, int searchMode, PMENUBAR* containerBar, int* pIndex);
BOOL menubar_removeItem(PMENUBAR const bar, int searchedID, int searchMode);

// Items
//
PMENUITEM item_create(int id, const char* text, int state, int status);
PMENUITEM item_add(PMENUBAR const bar, int index, int id, const char* text, int state, int status);
PMENUITEM item_copy(PMENUBAR const bar, PMENUITEM const source);
BOOL item_selectByIndex(int index, BOOL selected, BOOL redraw);

//
// Functions
//

// menu_create() : Create a new menu
//
// @return : pointer to menu or NULL on error
//
POWNMENU menu_create(){
    int size = sizeof(OWNMENU);
    POWNMENU menu =(POWNMENU)malloc(size);

    if (NULL == menu){
        return NULL;
    }

    menubar_clear(&menu->current_);
    menu->visible_ = &menu->current_; // show current bar
    menu_setHeight(MENUBAR_DEF_HEIGHT, false);
}

// menu_free() : Free a menu
//
// @menu : pointer to the menu
//
// @return : 1 if the menu has been successfully freed
//
int menu_free(POWNMENU menu){
    if (menu){

    }
}


// Construction
//
menuBar::menuBar(){
    // Intializes members
    visible_ = &current_; // show current bar
    setHeight(MENUBAR_DEF_HEIGHT, false);
    _clearMenuBar(&current_);
}

//
// Ownerdraw function
//

// setMenuDrawingCallBack() : Set function for ownerdraw drawings
//
// When an item has the ITEM_STATUS_OWNERDRAWN status bit set, the
// ownerdraw callback function will be called each time the menubar
// needs to redraw the item
//
// @pF : Pointer to the callback function or NULL if no ownerdraw
//
// @return : pointer to the default drawing function
//
MENUDRAWINGCALLBACK menuBar::setMenuDrawingCallBack(MENUDRAWINGCALLBACK pF){
    visible_->pDrawing = (void*)pF;
    return (MENUDRAWINGCALLBACK)defDrawItem;   // Def. function if needed by calling function
}

// setColour() : Change the colour used for item's drawings in the
//              active menu bar
//
//  @index : index of the colour to change
//  @colour : new colour value
//
//  @return : previous colour or -1 if error
//
int menuBar::setColour(uint8_t index, int colour){
    if (index>=COL_COUNT){
        return -1;
    }

    // Change the colour
    int actual = visible_->colours[index];
    visible_->colours[index] = colour;
    return actual;
}

//
// Dimensions
//

//  getRect() : Get bounding rect of current menu bar
//
void menuBar::getRect(RECT& barRect){
    barRect = {rect_.x, rect_.y, rect_.w, rect_.h};
}

//  setHeight() : change menu bar height
//
//  @barHeight : New height in pixels
//  @updateBar : Update the menubar ?
//
//  @return : true if hieght has changed
//
bool menuBar::setHeight(uint16_t barHeight, bool updateBar){
    rect_ = {0, CASIO_HEIGHT - barHeight, CASIO_WIDTH, barHeight};
    if (updateBar){
        update();
    }
    return true;
}

// Drawings
//
void menuBar::update(){
    if (NULL == visible_){
        return;
    }

    // First item's rect
    RECT anchor = {rect_.x, rect_.y, MENUBAR_DEF_ITEM_WIDTH, rect_.h};

    //  Draw all items (even empty ones)
    for (uint8_t index(0); index < MENU_MAX_ITEM_COUNT; index++){
        _drawItem(visible_->items[index], &anchor);
        anchor.x += anchor.w;   // Next item's position
    } // for

#ifdef DEST_CASIO_CALC
    dupdate();
#else
    cout << endl;
#endif // #ifdef DEST_CASIO_CALC
}

//  getItemState() : Get the state of an item
//
//  @searchedID : ID of searched item
//  @searchMode : type of search (SEARCH_BY_ID or SEARCH_BY_INDEX)
//
//  @return : Item's state or -1 if error
//
int8_t menuBar::getItemState(int searchedID, int searchMode){
    PMENUITEM item(_findItem(&current_, searchedID, searchMode));
    return (item?item->state:-1);
}

//  activate() : Activate or deactivate an item
//
//  When an item is deactivated, it can't be called by the user
//
//  @searchedID : Menu item's ID
//  @searchMode : Type of search (SEARCH_BY_ID or SEARCH_BY_INDEX)
//  @activated : true if item must be activated
//
//  @return : true if activation state changed
//
bool menuBar::activateItem(int searchedID, int searchMode, bool activated){
    PMENUBAR container(NULL);
    PMENUITEM item(_findItem(&current_, searchedID, searchMode, &container));
    if (item){
        // Found an item with this ID
        bool active = !isBitSet(item->state, ITEM_STATE_INACTIVE);
        if (active != activated){
            // change item's state
            if (activated){
                removeBit(item->state, ITEM_STATE_INACTIVE);
            }
            else{
                setBit(item->state, ITEM_STATE_INACTIVE);

                // an inactivate item can't be selected !
                removeBit(item->state, ITEM_STATE_SELECTED);
                if (container){
                    container->selIndex = -1;
                }
            }

            return true;
        }
    }

    return false;
}

// isMenuItemActivated() : Check wether an item is activated or not
//
//  @id : item id
//  @searchMode : type of search (SEARCH_BY_ID or SEARCH_BY_INDEX)
//
//  return : true if the item is activted
//
bool menuBar::isMenuItemActivated(int id, int searchMode){
    PMENUITEM item(_findItem(&current_, id, searchMode));
    return (item && !isBitSet(item->state, ITEM_STATE_INACTIVE));
}

//  freeMenuItem() : Free memory used by a menu item
//
//  @item : Pointer to the menu item to be released
//
void menuBar::freeMenuItem(PMENUITEM item){
    if (item){
        // A submenu ?
        if (isBitSet(item->status, ITEM_STATUS_SUBMENU) && item->subMenu){
            _freeMenuBar((PMENUBAR)item->subMenu, true);
        }

        free(item); // free the item
    }
}

// handleKeyboard() : Handle keyboard events
//
// @return : MENUACTION struct containing info
//          about item selected by user
//
MENUACTION menuBar::handleKeyboard(){
    uint key(0);
    uint8_t kID(0);
    keyboard kb;
    PMENUITEM item(NULL);
    MENUACTION ret = {0, ITEM_STATE_DEFAULT, MOD_NONE, ACTION_KEYBOARD};
    bool readKeyboard(true);

    if (readKeyboard){
        key = kb.getKey();

        // a menu key pressed ?
        if (key >= KEY_F1 && key <= KEY_F6){
            kID = (key - KEY_F1);

            // Associated item
            if (kID < MENU_MAX_ITEM_COUNT &&
                (item = visible_->items[kID]) &&
                !isBitSet(item->state, ITEM_STATE_INACTIVE)){
                // A sub menu ?
                if (isBitSet(item->status, ITEM_STATUS_SUBMENU)){
                    if (item->subMenu){
                        visible_ = (MENUBAR*)item->subMenu; // "visible" menu
                        _selectByIndex(-1, false, false);
                        update();
                    }
                }
                else{
                    if (IDM_RESERVED_BACK == item->id){
                        showParentBar();
                    }
                    else{
                        // select the item
                        if (kID != visible_->selIndex){
                            _selectByIndex(kID, true, true);
                        }

                        // A checkbox ?
                        if (isBitSet(item->status, ITEM_STATUS_CHECKBOX)){
                            if (isBitSet(item->state, ITEM_STATE_CHECKED)){
                                removeBit(item->state, ITEM_STATE_CHECKED);
                            }
                            else{
                                setBit(item->state, ITEM_STATE_CHECKED);
                            }
                        }

                        // a selectable item ...
                        ret.value = item->id;
                        ret.type = ACTION_MENU;
                        ret.state = item->state;
                        readKeyboard = false;
                    }
                }
            }

            update();
        } // if in [KEY_F1, KEY_F6]
        else{
            switch (key){
                // Back to prev. menu (if exists)
                case KEY_EXIT:
                    if (visible_ && visible_->parent){
                        visible_ = visible_->parent;
                        _selectByIndex(-1, false, false);
                        update();
                    }
                    break;

                default :
                    ret.value = key;
                    ret.type = ACTION_KEYBOARD;
                    ret.modifier = kb.modifier();
                    readKeyboard = false;
                    break;
            }
        }
    }

    // Return keyboard event
    ret.modifier = kb.modifier();
    return ret;
}

// showParentBar() : Return to parent menubar if exists
//
//  @updateBar : update the menu ?
//
void menuBar::showParentBar(bool updateBar){
    if (visible_ && visible_->parent){
        visible_ = visible_->parent;
        _selectByIndex(-1, false, false);

        if (updateBar){
            update();
        }
    }
}

// isMenuItemChecked() : Check wether a checkbox is in the checked state
//
//  @id : checkbox item id
//  @searchMode : type of search (SEARCH_BY_ID or SEARCH_BY_INDEX)
//
//  return : ITEM_CHECKED if the item is checked, ITEM_UNCHECKED
//          if the item is not cheched ITEM_ERROR
//          on error (invalid id, not a check box, ...)
//
int menuBar::isMenuItemChecked(int id, int searchMode){
    PMENUITEM item(_findItem(&current_, id, searchMode));
    if (item && isBitSet(item->status, ITEM_STATUS_CHECKBOX)){
        return (isBitSet(item->state,
                ITEM_STATE_CHECKED)?ITEM_CHECKED:ITEM_UNCHECKED);
    }

    // Not found or not a checkbox
    return ITEM_ERROR;
}

// checkMenuItem() : Check or uncheck a menu item checkbox
//
//  @id : checkbox item id
//  @searchMode : type of search (SEARCH_BY_ID or SEARCH_BY_INDEX)
//  @checkState : ITEM_CHECKED if item should be checked or ITEM_UNCHECKED
//
//  return : ITEM_CHECKED if item is checked, ITEM_UNCHECKED if not checked
//           and ITEM_ERROR on error
//
int menuBar::checkMenuItem(int id, int searchMode, int checkState){
    PMENUITEM item(_findItem(&current_, id, searchMode));
    if (item && isBitSet(item->status, ITEM_STATUS_CHECKBOX)){
        if (ITEM_CHECKED == checkState){
            setBit(item->state, ITEM_STATE_CHECKED);
        }
        else{
            removeBit(item->state, ITEM_STATE_CHECKED);
        }

        // return tatus of bit
        return (isBitSet(item->state,
                ITEM_STATE_CHECKED)?ITEM_CHECKED:ITEM_UNCHECKED);
    }

    // Not found or not a checkbox
    return ITEM_ERROR;
}

//
// Menu bars
//

//  _addSubMenu() : Add a sub menu
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
PMENUITEM menuBar::_addSubMenu(PMENUBAR const container, uint8_t index,
                            PMENUBAR subMenu, int id, const char* text,
                            int itemState, int status){
    size_t len(0);
    if (!container || !subMenu ||
        index >= MENU_MAX_ITEM_COUNT ||
        NULL != container->items[index] ||
        _findItem(container, id, SEARCH_BY_ID) ||
        container->itemCount == (MENU_MAX_ITEM_COUNT - 1) ||
        ! text || !(len = strlen(text))){
        return NULL;
    }

    // Create a copy of the menu bar
    PMENUBAR sub(_copyMenuBar(subMenu,
                    isBitSet(itemState, ITEM_STATE_NO_BACK_BUTTON)));
    if (NULL == sub){
        return NULL;
    }

    // Create item
    PMENUITEM item = _createItem(id, text,
                            ITEM_STATE_DEFAULT, status);
    if (NULL == item){
        _freeMenuBar(sub, true);
        return NULL;
    }

    setBit(item->status, ITEM_STATUS_SUBMENU);
    item->subMenu = sub;
    sub->parent = container;

    container->items[index] = item;    // item pointing to the sub menu
    container->itemCount++;

    return item;
}

//  _unSelectItems() : Unselect item(s)
//
//  @bar : Pointer to the bar
//
void menuBar::_unSelectItems(PMENUBAR bar){
    if (bar){
        PMENUITEM item;
        for (uint8_t index(0); index < MENU_MAX_ITEM_COUNT; index++){
            if ((item = bar->items[index])){
                removeBit(item->state, ITEM_STATE_SELECTED);
            }
        }
    }
}

//
// Menu items
//

//  _drawItem() : Draw an item
//
//  @item : Pointer to a MENUITEM strcut containing informations
//          concerning the item to draw
//  @anchor : Position of the item in screen coordinates
//
//  @return : False on error(s)
//
bool menuBar::_drawItem(PMENUITEM const item, RECT* const anchor){
    if (NULL == anchor){
        return false;
    }

    MENUDRAWINGCALLBACK ownerFunction;
    if (item && isBitSet(item->status, ITEM_STATUS_OWNERDRAWN) &&
        NULL != (ownerFunction = ((MENUDRAWINGCALLBACK)visible_->pDrawing))){
        return ownerFunction(visible_, item, anchor, MENU_DRAW_ALL); // Call ownerdraw func.
    }

    // Call default method
    return defDrawItem(visible_, item, anchor, MENU_DRAW_ALL);
}

//
// Internal functions
//

//
// menu functions
//

//  menu_drawItem() : Draw an item
//
//  @menu : Pointer to the MENU
//  @item : Pointer to a MENUITEM strcut containing informations
//          concerning the item to draw
//  @anchor : Position of the item in screen coordinates
//
//  @return : FALSE on error(s)
//
BOOL menu_drawItem((POWNMENU const menu, PMENUITEM const item, RECT* const anchor){
    if (NULL == anchor){
        return FALSE;
    }

    MENUDRAWINGCALLBACK ownerFunction;
    if (item && isBitSet(item->status, ITEM_STATUS_OWNERDRAWN) &&
        NULL != (ownerFunction = ((MENUDRAWINGCALLBACK)visible_->pDrawing))){
        return ownerFunction(menu->visible_, item, anchor, MENU_DRAW_ALL); // Call ownerdraw func.
    }

    // Call default method
    return item_defDrawFunction(menu->visible_, item, anchor, MENU_DRAW_ALL);
}


//
// menuBar management
//

//  menubar_clear() : Clear content of a menubar
//
//  @source : Pointer to the menubar
//
void menubar_clear(PMENUBAR bar){
    if (bar){
        // Bar is empty
        memset(bar, 0x00, sizeof(MENUBAR));
        bar->selIndex = -1;     // No item is selected
        memset(bar->items, 0x00, sizeof(PMENUITEM) * MENU_MAX_ITEM_COUNT);

        // Default colours
        bar->colours[TXT_SELECTED] = ITEM_COLOUR_SELECTED;
        bar->colours[TXT_UNSELECTED] = ITEM_COLOUR_UNSELECTED;
        bar->colours[TXT_INACTIVE] = ITEM_COLOUR_INACTIVE;
        bar->colours[ITEM_BACKGROUND] = ITEM_COLOUR_BACKGROUND;
        bar->colours[ITEM_BACKGROUND_SELECTED] = ITEM_COLOUR_BACKGROUND;
        bar->colours[ITEM_BORDER] = ITEM_COLOUR_BORDER;
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
PMENUBAR menubar_copy(PMENUBAR const source, int noBackButton){
    PMENUBAR bar = NULL;
    if (source && (bar = (PMENUBAR)malloc(sizeof(MENUBAR)))){
        menubar_clear(bar);
        bar->itemCount = source->itemCount;
        bar->parent = source->parent;
        bar->pDrawing = source->pDrawing;
        uint8_t max(noBackButton?MENU_MAX_ITEM_COUNT:(MENU_MAX_ITEM_COUNT-1));
        PMENUITEM sitem, nitem;
        for (uint8_t index(0); index < max; index++){
            nitem = item_copy(bar, (sitem = source->items[index]));
            bar->items[index] = nitem;  // simple item copy
        }

        if (!noBackButton){
            // In sub menus last right item is used to return to parent menu
            bar->items[MENU_MAX_ITEM_COUNT-1] =
                    item_create(IDM_RESERVED_BACK, STR_RESERVED_BACK,
                                ITEM_STATE_DEFAULT, ITEM_STATUS_DEFAULT);
        }

        // Copy choosen colours
        memcpy(bar->colours, source->colours, COL_COUNT * sizeof(int));
    }
    return bar;
}

//  _menubar_free() : Free memory used by a bar
//
//  @bar : Pointer to the bar to be released
//  @freeAll : if TRUE free the bar as well
//
void menubar_free(PMENUBAR bar, int freeAll){
    if (bar){
        for (uint8_t index(0); index < MENU_MAX_ITEM_COUNT; index++){
            freeMenuItem(bar->items[index]);
        }

        if (freeAll){
            free(bar);  // free the bar
        }
    }
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
                !foundItem && index < MENU_MAX_ITEM_COUNT; index++){
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
                            if ((sItem = item_find((PMENUBAR)item->subMenu, searchedID, searchMode, containerBar, pIndex))){
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
            if (searchedID >= 0 && searchedID < MENU_MAX_ITEM_COUNT){
                item = bar->items[searchedID];
                bar->items[searchedID] = NULL;

                // A sub menu ?
                if (isBitSet(item->status, ITEM_STATUS_SUBMENU)){
                    menubar_free((PMENUBAR)item->subMenu, true);
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

//  menubar_defDrawItem() : Draw an item
//
// @bar : Pointer to the bar containing the item to be drawn
//  @item : Pointer to a MENUITEM strcut containing informations
//          concerning the item to draw
//  @anchor : Position of the item in screen coordinates
//  @style : Drawing style ie. element(s) to draw
//
//  @return : FALSE on error(s)
//
BOOL menubar_defDrawItem(PMENUBAR const bar, PMENUITEM const item, RECT* const anchor, int style){
    if (NULL == bar || NULL == anchor){
        return FALSE;
    }

#ifdef DEST_CASIO_CALC
    bool selected(false);

    // Draw background
    if (isBitSet(style, MENU_DRAW_BACKGROUND)){
        drect(anchor->x, anchor->y, anchor->x + anchor->w - 1,
                anchor->y + anchor->h - 1,
                bar->colours[ITEM_BACKGROUND]);
    }

    if (item){
        int colour;

        selected = isBitSet(item->state, ITEM_STATE_SELECTED);
        int imgID(-1);  // No image

        // Text
        int x, w, h(0);
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
                        &g_menuImgs, imgID * MENU_IMG_WIDTH,
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
                        TXT_SELECTED:
                        (isBitSet(item->state, ITEM_STATE_INACTIVE)?
                        TXT_INACTIVE:TXT_UNSELECTED));

            // draw the text
            dtext_opt(x, anchor->y + (anchor->h - h) / 2,
                    bar->colours[colour],
                    bar->colours[ITEM_BACKGROUND],
                    DTEXT_LEFT, DTEXT_TOP,
                     item->text, -1);
        }

        // Borders
        if (isBitSet(style, MENU_DRAW_BORDERS) && selected){
            colour = bar->colours[ITEM_BORDER];
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
                bar->colours[ITEM_BORDER]);
    }
#else
    if (item){
        cout << "|" <<
            (isBitSet(item->state, ITEM_STATE_SELECTED)?">" : " ");
        if (isBitSet(item->status, ITEM_STATUS_CHECKBOX)){
            cout <<
                (isBitSet(item->state, ITEM_STATE_CHECKED)?"[x] ":"[ ] ");
        }

        if (isBitSet(item->status, ITEM_STATUS_TEXT)){
            if (isBitSet(item->state,ITEM_STATE_INACTIVE)){
                cout << "_" << (item->text+1);
            }
            else {
                cout << item->text;
            }
        }
        else{
            cout << "[no text]";
        }

        cout << (isBitSet(item->state,ITEM_STATE_SELECTED)?"<" : " ");
        cout << "|";
    }
    else{
        cout << "| [empty] |";
    }
#endif // #ifdef DEST_CASIO_CALC
    return TRUE;    // Done
}

//
// menu items management
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
        setBit(item->status, ITEM_STATUS_TEXT);
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
                removeBit(item->status, ITEM_STATUS_TEXT);
            }
        }
    }

    return item;
}

//  item_add() : Add an item to a menu bar
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
PMENUITEM item_add(PMENUBAR const bar, uint8_t index, int id, const char* text, int state, int status){
    if (!bar ||
        index >= MENU_MAX_ITEM_COUNT ||
        NULL != bar->items[index] ||
        item_find(bar, id, SEARCH_BY_ID) ||    // already handled
        current_.itemCount >= MENU_MAX_ITEM_COUNT /*||
        !text || !(len = strlen(text))*/){
        return NULL;
    }

    PMENUITEM item = item_create(id, text, state, status);
    if (item){
        // Add to the menu
        if (IDM_RESERVED_BACK == id){
            bar->items[MENU_MAX_ITEM_COUNT-1] = item;
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

//  _copyItem() : Make a copy of an item
//
//  @bar : Destination menu bar container
//  @source : Pointer to the source item
//
//  @return : pointer to the copied item or NULL
//
PMENUITEM item_copy(PMENUBAR const bar, PMENUITEM const source){
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
            ((PMENUBAR)(item->subMenu))->parent = bar;
        }
        else{
            item->subMenu = NULL;
        }
    }
    return item;
}

//  item_selectByIndex() : Select an item by index in the current bar
//
//  @index : index of menu item to select or unselect
//  @selected : TRUE if item is to be selected
//  @redraw : when TRUE, item and previously (un)selected
//              item are drawn in their new states
//
//  @return : TRUE if item is selected
//
BOOL item_selectByIndex(int index, BOOL selected, BOOL redraw){
    int sel;
    if (index >= MENU_MAX_ITEM_COUNT ||
        index == (sel = getSelectedIndex())){
        return FALSE;   // Nothing to do
    }

    if (-1 == index){
        index = sel;
        selected = FALSE;
    }

    // Select or unselect an item
    if (index >= 0){
        // Item
        PMENUITEM item = visible_->items[index];
        if (NULL == item){
            return FALSE;
        }

        if (selected){
            // unselect prev.
            _unSelectItems(visible_);

             setBit(item->state, ITEM_STATE_SELECTED);
            visible_->selIndex = index;
        }
        else{
            removeBit(item->state, ITEM_STATE_SELECTED);
            visible_->selIndex = -1;
        }
    }

    // Update item ?
    if (redraw){
        RECT anchor = {rect_.x, rect_.y, MENUBAR_DEF_ITEM_WIDTH, rect_.h};
        if (-1 != sel){
            anchor.x+=(sel * anchor.w);
            item_draw(visible_->items[sel], &anchor);
        }

        if (-1 != index){
            anchor.x=(index * anchor.w + rect_.x);
            item_draw(visible_->items[index], &anchor);
        }
    }

    // Done
    return TRUE;
}

// EOF
