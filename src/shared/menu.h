//----------------------------------------------------------------------
//--
//--    menu.h
//--
//--        Definition of menuBar object - A bar of menu (or a submenu)
//--
//----------------------------------------------------------------------

#ifndef __GEE_CASIO_MENU_h__
#define __GEE_CASIO_MENU_h__    1

#include "casioCalcs.h"
#include "keyboard.h"

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

#define _GEEHB_MENU_VER_        "0.6.0"

//
// State & status
//
#define IS_BIT_SET(value, bit)  (bit == (value & bit))
#define SET_BIT(value, bit)     {(value |= bit); return value; }
#define REMOVE_BIT(value, bit)  (value & ~bit)


#define MENU_MAX_ITEM_COUNT     6   // ie. "F" buttons count

//
// Dimensions in pixels
//
#define MENUBAR_DEF_HEIGHT      22
#define MENUBAR_DEF_ITEM_WIDTH  (CASIO_WIDTH / MENU_MAX_ITEM_COUNT)

#define ITEM_NAME_LEN           10  // Max length of item name

// Image (in bar) dimensions
#define MENU_IMG_WIDTH          12
#define MENU_IMG_HEIGHT         12

#define ITEM_ROUNDED_DIM        4

//
// Item pos is a menu bar
//
#define MENU_POS_LEFT           0
#define MENU_POS_RIGHT          (MENU_MAX_ITEM_COUNT-1)

//
// Item state - could be any combination of :
//
#define ITEM_STATE_DEFAULT          0
#define ITEM_STATE_SELECTED         1
#define ITEM_STATE_INACTIVE         2
#define ITEM_STATE_CHECKED          4
#define ITEM_STATE_NO_BACK_BUTTON   8

//
// Item status - could be any combination of :
//
#define ITEM_STATUS_DEFAULT     0
#define ITEM_STATUS_TEXT        1   // Item's text is valid
#define ITEM_STATUS_SUBMENU     2   // Open a sub menu on "click"
#define ITEM_STATUS_KEYCODE     4   // item's ID is a key code
#define ITEM_STATUS_CHECKBOX    8   // Item is a checkbox
#define ITEM_STATUS_OWNERDRAWN  16  // Use own callback for item drawing

// Drawing styles for ownerdraw function
//
#define MENU_DRAW_BACKGROUND    1
#define MENU_DRAW_TEXT          2
#define MENU_DRAW_IMAGE         4
#define MENU_DRAW_BORDERS       8

// Checkbox state
//
#define ITEM_ERROR          -1      // Unknown state or not a checkbox
#define ITEM_UNCHECKED      0
#define ITEM_CHECKED        1

// A few helpers ...

// Draw content (but not background)
#define MENU_DRAW_CONTENT (MENU_DRAW_TEXT|MENU_DRAW_IMAGE|MENU_DRAW_BORDERS)

// Draw all, by default
#define MENU_DRAW_ALL    (MENU_DRAW_CONTENT | MENU_DRAW_BACKGROUND)

//
// Reserved menu item ID
//
#define IDM_RESERVED_BACK       0xFFFB  // Return to parent menu
#ifdef DEST_CASIO_CALC
#define STR_RESERVED_BACK       "back"
#endif // #ifdef DEST_CASIO_CALC

//
// Menu colours
//

// ID of colours (for {set/get}Colour methods
//
#define COLOUR_TXT_SELECTED             0
#define COLOUR_TXT_UNSELECTED           1
#define COLOUR_TXT_INACTIVE             2
#define COLOUR_ITEM_BACKGROUND          3
#define COLOUR_ITEM_BACKGROUND_SELECTED 4   // for future use
#define COLOUR_ITEM_BORDER              5
#define COLOUR_COUNT                    6

// Default colours
//
#define ITEM_COLOUR_SELECTED    COLOUR_BLUE
#define ITEM_COLOUR_UNSELECTED  COLOUR_DK_GREY
#define ITEM_COLOUR_INACTIVE    COLOUR_LT_GREY
#define ITEM_COLOUR_BACKGROUND  COLOUR_WHITE
#define ITEM_COLOUR_BORDER      COLOUR_BLACK

//
// items
//

// A single item
//
typedef struct _menuItem{
    int id;
    int state;
    int status;
    char text[ITEM_NAME_LEN + 1];
    void* subMenu;  // if item is a submenu, points to the submenu
    int ownerData;  // Can be anything ...
} MENUITEM,* PMENUITEM;


//  item_create() : creae a new menu item
//
//  @id : Item's id
//  @text : Menu item text
//  @state : Item's initial state
//  @status : Item's status
//
//  @return : pointer to the new created if valid or NULL
//
PMENUITEM item_create(int id, const char* text, int state, int status);

//  item_free() : creae a new menu item
//
//  @item : pointer to item
//
//  @return : NULL
//
PMENUITEM item_free(PMENUITEM item);

//
// menubar
//

// A menu bar struct.
//
typedef struct _menuBar{
    int itemCount;
    int selIndex;
    _menuBar* parent;
    void* pDrawing;    // Pointer to ownerdraw callback
    PMENUITEM items[MENU_MAX_ITEM_COUNT];
    int colours[COLOUR_COUNT];
} MENUBAR, * PMENUBAR;

// Action to perform
//
typedef struct _menuAction{
    int value;
    int state;
    int modifier;
    int type;
} MENUACTION;

// Types of actions
//
#define ACTION_MENU         0   // value is a menu ID
#define ACTION_KEYBOARD     1   // value is a keycode

// Drawing styles for ownerdraw function
//
#define MENU_DRAW_BACKGROUND    1
#define MENU_DRAW_TEXT          2
#define MENU_DRAW_IMAGE         4
#define MENU_DRAW_BORDERS       8

// Draw content (but not background)
#define MENU_DRAW_CONTENT (MENU_DRAW_TEXT|MENU_DRAW_IMAGE|MENU_DRAW_BORDERS)

// Draw all, by default
#define MENU_DRAW_ALL    (MENU_DRAW_CONTENT | MENU_DRAW_BACKGROUND)

// Ownerdraw's function prototype
//
typedef BOOL (*MENUDRAWINGCALLBACK)(
            PMENUBAR const,     // MenuBar containing the item
            PMENUITEM const,    // Item to draw
            RECT* const,        // Drawing rect for item
            int style);         // Elements (in item) to draw

// Types of search modes
//
#define SEARCH_BY_ID        0
#define SEARCH_BY_INDEX     1

//  menubar_free() : Free memory used by a bar
//
//  @bar : Pointer to the bar to be released
//  @freeAll : if TRUE free the bar as well
//
void menubar_free(PMENUBAR bar, int freeAll);

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
PMENUBAR menubar_copy(PMENUBAR const source, int noBackButton);

//  menubar_drawItem() : Draw an item
//
//  @item : Pointer to a MENUITEM strcut containing informations
//          concerning the item to draw
//  @anchor : Position of the item in screen coordinates
//
//  @return : FALSE on error(s)
//
BOOL menubar_drawItem(PMENUBAR bar, PMENUITEM const item, RECT* const anchor);

//  menubar_getHeight() : Get menu bar height
//
//  @bar : Pointer to the menu bar
//
//  @return : Height of menu bar in pixels
//
int menubar_getHeight(PMENUBAR bar);

//  menubar_getRect() : Get bounding rect of current menu bar
//
//  @bar : Pointer to the menu bar
//  @barREct : Pointer to a RECT struct. This struct. will contin the menu's dimensions and position
//
//  @return : TRUE if done
//
BOOL menubar_getRect(PMENUBAR bar, RECT* barRect);

//  menubar_setHeight() : change menu bar height
//
//  @bar : Pointer to the menu bar
//  @barHeight : New height in pixels
//  @updateBar : Update the menubar ?
//
//  @return : TRUE if done
//
BOOL menubar_setHeight(uint16_t barHeight, BOOL updateBar);

//  menubar_update() : Update the given menu bar
//
//  @bar : Pointer to the menu bar
//
void menubar_update(PMENUBAR bar);

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
PMENUITEM menubar_findItem(PMENUBAR const bar, int searchedID, int searchMode, PMENUBAR* containerBar, int* pIndex);

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
PMENUITEM menubar_addItem(PMENUBAR const bar, int index, int id, const char* text, int state, int status);

//  menubar_copyItem() : Make a copy of an item
//
//  @bar : Destination menu bar container
//  @source : Pointer to the source item
//
//  @return : pointer to the copied item or NULL
//
PMENUITEM menubar_copyItem(PMENUBAR const bar, PMENUITEM const source);

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
BOOL menubar_removeItem(PMENUBAR const bar, int searchedID, int searchMode);

//  menubar_freeMenuItem() : Free memory used by a menu item
//
//  @bar : Pointer to the menu bar
//  @item : Pointer to the menu item to be released
//
void menubar_freeMenuItem(PMENUBAR bar, PMENUITEM item);

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
static BOOL menubar_defDrawItem(PMENUBAR const bar, PMENUITEM const item, RECT* const anchor, int style);

// menubar_showParentBar() : Return to parent menubar if exists
//
//  @bar : Pointer to the current bar
//  @updateBar : update the menu ?
//
void menubar_showParentBar(PMENUBAR bar, BOOL updateBar);

//
// menu
//

typedef struct _ownMenu{
    MENUBAR     current_;   // Active bar
    RECT        rect_;      // Bar position and dims
    PMENUBAR    visible_;   // Visible menu bar
} OWNMENU, * POWNMENU;

// menu_create() : Create a new menu
//
// @return : pointer to menu or NULL on error
//
POWNMENU menu_create();

// menu_free() : Free a menu
//
//  @menu : pointer to the menu
//
//  @return : TRUE if the menu has been successfully freed
//
BOOL menu_free(POWNMENU menu);

//  getHeight() : Get menu bar height
//
//  @menu : Pointer to the menu
//
//  @return : Height of menu bar in pixels
//
int menu_getHeight(POWNMENU menu);

//  getRect() : Get bounding rect of current menu bar
//
//  @menu : Pointer to the menu
//  @barREct : Pointer to a RECT struct. This struct. will contin the menu's dimensions and position
//
//  @return : TRUE if done
//
BOOL menu_getRect(POWNMENU menu, RECT* barRect);

//  setHeight() : change menu bar height
//
//  @menu : Pointer to the menu
//  @barHeight : New height in pixels
//  @updateBar : Update the menubar ?
//
//  @return : true if hieght has changed
//
BOOL setHeight(POWNMENU menu, int barHeight, BOOL updateBar);

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
MENUDRAWINGCALLBACK menu_setMenuDrawingCallBack(POWNMENU menu, MENUDRAWINGCALLBACK pF);

//  menu_selectByIndex() : Select an item by index in the current bar
//
//  @menu : pointer to the menu
//  @index : index of menu item to select or unselect
//  @selected : true if item is to be selected
//  @redraw : when true, item and previously (un)selected
//              item are drawn in their new states
//
//  @return : true if item is selected
//
BOOL menu_selectByIndex(POWNMENU menu, int8_t index, BOOL selected, BOOL redraw);

//  menu_drawItem() : Draw an item
//
//  @menu : pointer to the menu
//  @item : Pointer to a MENUITEM strcut containing informations
//          concerning the item to draw
//  @anchor : Position of the item in screen coordinates
//
//  @return : False on error(s)
//
BOOL menu_drawItem(POWNMENU menu, PMENUITEM const item, RECT* const anchor);

// menu_getColour() : Get the colour used for item's drawings in the
//              active menu bar
//
//  @menu : Pointer to the menu
//  @index : index of the colour to retreive
//
//  @return : colour or -1 if error
//
int menu_getColour(POWNMENU menu, int index);

//  menu_update() : Update the menu bar
//
//  All items will be drawn according to their state et status
//
//  @menu : Pointer to the menu
//
void menu_update(POWNMENU menu);


//   menuBar : A bar of menu (or submenu)
//
class menuBar{
public:

    //
    // Items & subBars
    //

    //  size() : count of items or sub menus in the bar
    //
    //  @return : The count of items or sub menus
    //
    uint8_t size(){
        return current_.itemCount;
    }

    //  addSubMenu() : Add a sub menu
    //
    //  @index : Index (position) in the menu bar
    //  @submenu : menubar corresponding to the new submenu
    //  @id : ID associated to the menu
    //  @text : Submenu text
    //  @state : Menu item initial state
    //  @status : initial status of submenu
    //
    //  @return : pointer the created item or NULL
    //
    PMENUITEM addSubMenu(uint8_t index, menuBar* const subMenu,
            int id, const char* text, int state = ITEM_STATE_DEFAULT,
            int status = ITEM_STATUS_DEFAULT){
        return _addSubMenu(&current_, index,
                        (PMENUBAR)subMenu, id, text, state, status);
    }

    //  appendSubMenu() : Append a sub menu
    //
    //  @submenu : menubar corresponding to the new submenu
    //  @id : ID associated to the menu
    //  @text : Submenu text
    //  @state : Menu item initial state
    //  @status : initial status of submenu
    //
    //  @return : pointer the created item or NULL
    //
    PMENUITEM appendSubMenu(const menuBar* subMenu, int id,
                        const char* text, int state = ITEM_STATE_DEFAULT,
                        int status = ITEM_STATUS_DEFAULT){
        return _addSubMenu(&current_, current_.itemCount,
                            (PMENUBAR)subMenu, id, text, state, status);
    }

    //  addItem() : Add an item to the current menu bar
    //
    //  @index : Index (position) in the menu bar
    //  @id : Item ID
    //  @text : Item text
    //  @state : Item's initial state
    //  @status : Item's status
    //
    //  @return : pointer the created item or NULL
    //
    PMENUITEM addItem(uint8_t index, int id, const char* text,
                int state = ITEM_STATE_DEFAULT,
                int status = ITEM_STATUS_DEFAULT){
        return _addItem(&current_, index, id, text, state, status);
    }

    //  addCheckbox() : Add an item to the current menu bar
    //
    //  @index : Index (position) in the menu bar
    //  @id : Item ID
    //  @text : Item text
    //  @state : Item's initial state
    //  @status : Item's status
    //
    //  @return : pointer the created item or NULL
    //
    PMENUITEM addCheckBox(uint8_t index, int id, const char* text,
                int state = ITEM_STATE_DEFAULT,
                int status = ITEM_STATUS_DEFAULT){
        return _addItem(&current_, index, id, text,
                    state, ITEM_STATUS_CHECKBOX | status);
    }

    //  appendItem() : Append an item to the current menu bar
    //
    //  @id : Item ID
    //  @text : Item text
    //  @state : Item's initial state
    //  @status : Item's status
    //
    //  @return : pointer the created item or NULL
    //
    PMENUITEM appendItem(int id, const char* text,
                    int state = ITEM_STATE_DEFAULT,
                    int status = ITEM_STATUS_DEFAULT){
        return _addItem(&current_, current_.itemCount, id, text, state, status);
    }

    //  appendCheckbox() : Append a checkbox to the current menu bar
    //
    //  @id : Item ID
    //  @text : Item text
    //  @state : Item's initial state
    //  @status : Item's status
    //
    //  @return : pointer the created item or NULL
    //
    PMENUITEM appendCheckbox(int id, const char* text,
                    int state = ITEM_STATE_DEFAULT,
                    int status = ITEM_STATUS_DEFAULT){
        return _addItem(&current_, current_.itemCount, id, text,
                        state,  status | ITEM_STATUS_CHECKBOX);
    }

    // isMenuItemChecked() : Check wether a checkbox is in the checked state
    //
    //  @id : checkbox item id
    //  @searchMode : type of search (SEARCH_BY_ID or SEARCH_BY_INDEX)
    //
    //  return : ITEM_CHECKED if the item is checked, ITEM_UNCHECKED
    //          if the item is not cheched and ITEM_ERROR on error
    //          (invalid id, not a check box, ...)
    //
    int isMenuItemChecked(int id, int searchMode = SEARCH_BY_ID);

    // checkMenuItem() : Check or uncheck a menu item checkbox
    //
    //  @id : checkbox item id
    //  @searchMode : type of search (SEARCH_BY_ID or SEARCH_BY_INDEX)
    //  @checkState : ITEM_CHECKED if item should be checked or ITEM_UNCHECKED
    //
    //  return : ITEM_CHECKED if item is checked, ITEM_UNCHECKED if not checked
    //           and ITEM_ERROR on error
    //
    int checkMenuItem(int id, int searchMode = SEARCH_BY_ID,
                    int checkState = ITEM_CHECKED);

    //  removeItem() : Remove an item from the current menu bar
    //      Remove the item menu or the submenu
    //
    //  @searchedID : Item's ID or index
    //  @searchMode : if SEARCH_BY_ID, searchedID is the ID of item
    //                if SEARCH_BY_INDEX serachedID is the index in the menu bar
    //
    //  @return : true if the item has been successfully removed
    //
    bool removeItem(int searchID, int searchMode){
        return _removeItem(&current_, searchID, searchMode);
    }

    //
    // Selection & activation
    //

    //  _unSelectItems() : Unselect item(s)
    //
    void unSelectItems(){
        _unSelectItems(visible_);
    }

    //  selectByIndex() : Select an item by index in the current bar
    //
    //  @index : index of menu item to select or unselect
    //          if equal to -1, unselect the currently selected item
    //  @selected : true if item is to be selected
    //  @redraw : Redraw the item ?
    //
    //  @return : true if item is selected
    //
    bool selectByIndex(int8_t index, bool selected = true,
                    bool redraw = false){
        return _selectByIndex(index, selected, redraw);
    }

    //  getSelectedIndex() : Index of selected item in the current bar
    //
    //  @return : Index of item selected or -1 if none
    //
    int8_t getSelectedIndex(){
        return (visible_?visible_->selIndex:current_.selIndex);
    }

    //  getItemState() : Get the state of an item
    //
    //  @searchedID : ID of searched item
    //  @searchMode : type of search (SEARCH_BY_ID or SEARCH_BY_INDEX)
    //
    //  @return : Item's state or -1 if error
    //
    int8_t getItemState(int searchedID, int searchMode);

    //  activateItem() : Activate or deactivate an item
    //
    //  When an item is deactivated, it can't be called by the user
    //
    //  @searchedID : Menu item's ID
    //  @searchMode : Type of search (SEARCH_BY_ID or SEARCH_BY_INDEX)
    //  @activated : true if item must be activated
    //
    //  @return : true if activation state changed
    //
    bool activateItem(int searchedID, int searchMode, bool activated = true);
    bool activate(int searchedID, int searchMode, bool activated = true){
        return activateItem(searchedID, searchMode, activated);
    }

    // isMenuItemActivated() : Check wether an item is activated or not
    //
    //  @id : item id
    //  @searchMode : type of search (SEARCH_BY_ID or SEARCH_BY_INDEX)
    //
    //  return : true if the item is activted
    //
    bool isMenuItemActivated(int id, int searchMode = SEARCH_BY_ID);

    //
    // Item access and modifications
    //

    //  getItem() : Find an item in the menu bar and its submenus
    //
    //  @searchedID : id or index of the searched item
    //  @searchMode : Type of search (SEARCH_BY_ID or SEARCH_BY_INDEX)
    //  @containerBar : pointer to a PMENUBAR. when not NULL,
    //          if item is found, containerBar will point to the bar
    //          containing the item
    //  @pIndex : when not NULL, will point to the item's index in its menu
    //
    //  @return : pointer to the item if found or NULL
    //
    PMENUITEM getItem(int searchedID, int searchMode,
                PMENUBAR* containerBar = NULL,
                uint8_t* pIndex = NULL){
        return _findItem(&current_, searchedID, searchMode,
                        containerBar, pIndex);
    }

    //  setItem() : Modify an item in the menu bar and its submenus
    //
    //  @pItem : Pointer to the modified menu item
    //  @searchedID : id or index of the searched item
    //  @searchMode : Type of search (SEARCH_BY_ID or SEARCH_BY_INDEX)
    //  @mask : Mask to identify modified fields in the MENUITEM struct
    //
    //  @return : true if successfully applied
    //
    bool setItem(PMENUITEM pItem, int searchID, int searchMode, int Mask);

    //  findItem() : Find an item in the menu bar and its submenus
    //
    //  Use getItem
    //
    PMENUITEM findItem(int searchedID, int searchMode,
                PMENUBAR* containerBar = NULL,
                uint8_t* pIndex = NULL){
        return getItem(searchedID, searchMode, containerBar, pIndex);
    }

    // handleKeyboard() : Handle keyboard events
    //
    // @return : MENUACTION struct containing info
    //          about item selected b user
    //
    MENUACTION handleKeyboard();

    // Internal methods
private:

    //
    // menu bars
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
    PMENUITEM _addSubMenu(PMENUBAR const container, uint8_t index,
                PMENUBAR const subMenu, int id, const char* text,
                int state, int status);

    //  _unSelectItems() : Unselect item(s)
    //
    //  @bar : Pointer to the bar
    //
    void _unSelectItems(PMENUBAR bar);


    //  _findItem() : Find an item in the given bar
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
    PMENUITEM _findItem(PMENUBAR const bar, int searchedID,
                int searchMode, PMENUBAR* containerBar = NULL,
                uint8_t* pIndex = NULL);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_CASIO_MENU_h__

// EOF
