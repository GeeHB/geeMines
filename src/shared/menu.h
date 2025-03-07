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
#include "keys.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

#define _GEEHB_MENU_VER_        "0.6.1"

#define MENUBAR_MAX_ITEM_COUNT  6   // ie. "F" buttons count

//
// Dimensions in pixels
//
#define MENUBAR_DEF_HEIGHT      22
#define MENUBAR_DEF_ITEM_WIDTH  (CASIO_WIDTH / MENUBAR_MAX_ITEM_COUNT)

#define ITEM_NAME_LEN           10  // Max length of item name

// Image (in bar) dimensions
#define MENU_IMG_WIDTH          12
#define MENU_IMG_HEIGHT         12

#define ITEM_ROUNDED_DIM        4

//
// Item state - could be any combination of :
//
#define ITEM_STATE_DEFAULT          0
#define ITEM_STATE_SELECTED         1
#define ITEM_STATE_INACTIVE         2
#define ITEM_STATE_CHECKED          4
#define ITEM_STATE_UNCHECKED        ITEM_STATE_DEFAULT
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
#define STR_RESERVED_BACK       "back"

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
void item_free(PMENUITEM item);

//
// menubar
//

// A menu bar struct.
//
typedef struct _menuBar{
    int itemCount;
    int selIndex;
    void* parent;   // Pointer to the parent MENUBAR
    PMENUITEM items[MENUBAR_MAX_ITEM_COUNT];
} MENUBAR, * PMENUBAR;

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

// Types of search modes
//
#define SEARCH_BY_ID        0
#define SEARCH_BY_INDEX     1

//
// Item pos is a menu bar
//
#define MENU_POS_LEFT       0
#define MENU_POS_RIGHT      (MENUBAR_MAX_ITEM_COUNT-1)

//  menubar_create() : Create an empty menubar
//
//  @return : Pointer to the new menubar or NULL
//
PMENUBAR menubar_create();

//  menubar_size() : count of items or sub menus in the bar
//
//  @bar : Pointer to the bar
//
//  @return : The count of items or sub menus
//
uint8_t menubar_size(PMENUBAR bar);

//  menubar_clear() : Clear content of a menubar
//
//  @bar : Pointer to the menubar
//
void menubar_clear(PMENUBAR bar);

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
PMENUBAR menubar_copy(PMENUBAR const source, BOOL noBackButton);

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
PMENUITEM menubar_getItem(PMENUBAR bar, int searchedID, int searchMode, PMENUBAR* containerBar, int* pIndex);

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
PMENUITEM menubar_addItem(PMENUBAR const bar, uint8_t index, int id, const char* text, int state, int status);

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
PMENUITEM menubar_appendItem(PMENUBAR bar, int id, const char* text, int state, int status);

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
PMENUITEM menubar_addSubMenu(PMENUBAR const container, uint8_t index, PMENUBAR subMenu, int id, const char* text, int itemState, int status);

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
PMENUITEM menubar_appendSubMenu(PMENUBAR bar, const PMENUBAR subMenu, int id, const char* text, int state, int status);

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

//  menubar_selectByIndex() : Select an item by index in the current bar
//
//  @bar : pointer to the bar
//  @index : index of menu item to select or unselect
//  @selected : true if item is to be selected
//  @redraw : when true, item and previously (un)selected
//              item are drawn in their new states
//
//  @return : true if item is selected
//
BOOL menubar_selectByIndex(PMENUBAR bar, int index, BOOL selected);

//  menu_unSelectItems() : Unselect item(s)
//
//  @bar : Pointer to the bar
//
void menubar_unSelectItems(PMENUBAR bar);

//  menubar_getSelectedIndex() : Index of selected item in the current bar
//
//  @bar : Pointer to the bar
//
//  @return : Index of item selected or -1 if none
//
int menubar_getSelectedIndex(PMENUBAR bar);

// menubar_isMenuItemActivated() : Check wether an item is activated or not
//
//  @bar : Pointer to the bar
//  @id : item id
//  @searchMode : type of search (SEARCH_BY_ID or SEARCH_BY_INDEX)
//
//  return : TRUE if the item is activted
//
BOOL menubar_isMenuItemActivated(PMENUBAR bar, int id, int searchMode);

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
PMENUITEM menubar_addCheckBox(PMENUBAR bar, int index, int id, const char* text, int state, int status);

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
PMENUITEM menubar_appendCheckbox(PMENUBAR bar, int id, const char* text, int state, int status);

//  menubar_activateItem() : Activate or deactivate an item
//
//  When an item is deactivated, it can't be called by the user
//
//  @bar : Pointer to the bar
//  @searchedID : Menu item's ID
//  @searchMode : Type of search (SEARCH_BY_ID or SEARCH_BY_INDEX)
//  @activated : TRUE if item must be activated
//
//  @return : TRUE if activation state changed
//
BOOL menubar_activateItem(PMENUBAR bar, int searchedID, int searchMode, BOOL activated);
BOOL menubar_activate(PMENUBAR bar, int searchedID, int searchMode, BOOL activated);

//  menubar_getItemState() : Get the state of an item
//
//  @bar : Pointer to the bar
//  @searchedID : ID of searched item
//  @searchMode : type of search (SEARCH_BY_ID or SEARCH_BY_INDEX)
//
//  @return : Item's state or -1 if error
//
int menubar_getItemState(PMENUBAR bar, int searchedID, int searchMode);

//  menubar_setItemState() : Set the state of an item
//
//  @bar : Pointer to the bar
//  @searchedID : ID of searched item
//  @searchMode : type of search (SEARCH_BY_ID or SEARCH_BY_INDEX)
//  @state : new item state
//
//  @return : previous item state or -1 on error
//
int menubar_setItemState(PMENUBAR bar, int searchedID, int searchMode, int state);

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
int menubar_checkMenuItem(PMENUBAR, int id, int searchMode, int checkState);

//
// menu
//

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

typedef struct _ownMenu{
    MENUBAR     current_;       // Active bar
    RECT        rect_;          // Bar position and dims
    PMENUBAR    visible_;       // Visible menu bar
    int         colours_[COLOUR_COUNT]; // Colours used for drawings
    void*       drawingFunc;    // Pointer to ownerdraw callback
} OWNMENU, * POWNMENU;

// Ownerdraw's function prototype
//
typedef BOOL (*MENUDRAWINGCALLBACK)(
            POWNMENU const,     // Menu containing the item
            PMENUITEM const,    // Item to draw
            RECT* const,        // Drawing rect for item
            int style);         // Elements (in item) to draw

// menu_create() : Create a new menu
//
// @return : pointer to menu or NULL on error
//
POWNMENU menu_create();

// menu_free() : Free a menu
//
//  @menu : pointer to the menu
//
void menu_free(POWNMENU menu);

// menu_getMenuBar() : Get a pointer to the default menubar
//
//  @menu : pointer to the menu
//
//  @return : A pointer to the bar or NULL on error
//
PMENUBAR menu_getMenuBar(POWNMENU menu);

//  getHeight() : Get menu bar height
//
//  @menu : Pointer to the menu
//
//  @return : Height of menu bar in pixels
//
uint16_t menu_getHeight(POWNMENU menu);

//  getRect() : Get bounding rect of current menu bar
//
//  @menu : Pointer to the menu
//  @barREct : Pointer to a RECT struct. This struct. will contin the menu's dimensions and position
//
//  @return : TRUE if done
//
BOOL menu_getRect(POWNMENU menu, RECT* barRect);

//  menu_setHeight() : change menu bar height
//
//  @menu : Pointer to the menu
//  @barHeight : New height in pixels
//  @updateBar : Update the menubar ?
//
//  @return : true if hieght has changed
//
BOOL menu_setHeight(POWNMENU menu, uint16_t barHeight, BOOL updateBar);

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
BOOL menu_defDrawItem(POWNMENU const menu, PMENUITEM const item, RECT* const anchor, int style);

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


//  menu_update() : Update the menu bar
//
//  All items will be drawn according to their state et status
//
//  @menu : Pointer to the menu
//
void menu_update(POWNMENU menu);

// menu_getColour() : Get the colour used for item's drawings in the
//              active menu bar
//
//  @menu : Pointer to the menu
//  @index : index of the colour to retreive
//
//  @return : colour or -1 if error
//
int menu_getColour(POWNMENU menu, uint8_t index);

// menu_setColour() : Change the colour used for item's drawings in the
//              active menu bar
//
//  @menu : Pointer to the menu
//  @index : index of the colour to change
//  @colour : new colour value
//
//  @return : previous colour or -1 if error
//
int menu_setColour(POWNMENU menu, uint8_t index, int colour);

// menu_showParentBar() : Return to parent menubar if exists
//
//  @menu : Pointer to the menu
//  @updateBar : update the menu ?
//
void menu_showParentBar(POWNMENU menu, BOOL updateBar);

//
// Keyboard management
//

// Action to perform
//
typedef struct _menuAction{
    int value;
    int state;
    int modifier;
    int type;
} MENUACTION, * PMENUACTION;

// Types of actions
//
#define ACTION_MENU         0   // value is a menu ID
#define ACTION_KEYBOARD     1   // value is a keycode

// Keys used by menu
//
#ifndef DEST_CASIO_CALC
    #define KEY_F1         'A'
    #define KEY_F6         'F'
    #define KEY_EXIT       'q'
#endif // #ifdef DEST_CASIO_CALC

// menu_handleKeyboard() : Handle keyboard events
//
//  @menu : Pointer to the menu
//  @action : pointer a MENUACTION struct. It will ba filled with
//              infor!mation about item smected by the user
//
//  @return : FALSE on error
//
BOOL menu_handleKeyboard(POWNMENU menu, PMENUACTION action);

// menu_clearAction() : Clear the menu action struct.
//
//  @action : pointer a MENUACTION struct.
//
//  @return : TRUE if done
//
BOOL menu_clearAction(PMENUACTION action);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_CASIO_MENU_h__

// EOF
