#include "../src/shared/casioCalcs.h"
#include "../src/shared/menu.h"
#include "../src/consts.h"

#include "../src/board.h"
#include "../src/scores.h"

// Menu de test
//

#define IDS_NEW_GAME    "Nouveau"
#define IDM_NEW_GAME    1

#define IDS_GAME_BEGINNER   "Débutant"
#define IDM_GAME_BEGINNER   11

#define IDS_GAME_MEDIUM     "Medium"
#define IDM_GAME_MEDIUM     12

#define IDS_GAME_EXPERT     "Expert"
#define IDM_GAME_EXPERT     13

#define IDS_CHECK      "Yes"
#define IDM_CHECK      2

//#define IDS_QUIT       "Quitter"
#define IDM_QUIT       6

// Premier test
int main_basic()
{
    // Création d'un menu
    //
    POWNMENU menu = menu_create();
    if (!menu){
        printf("Impossible de créer le menu\n");
        return 1;
    }

    // Une première barre ...
    PMENUBAR bar = menu_getMenuBar(menu);
    if (bar){
        BOOL end = FALSE;
        MENUACTION action;

        menubar_appendItem(bar, 1, "Fichier", ITEM_STATE_DEFAULT, ITEM_STATUS_DEFAULT);
        menubar_appendItem(bar, 2, "Edition", ITEM_STATE_DEFAULT, ITEM_STATUS_DEFAULT);
        menubar_addItem(bar, MENU_POS_RIGHT, 3, "Quitter", ITEM_STATE_DEFAULT, ITEM_STATUS_DEFAULT);

        // Affichage du menu
        menu_update(menu);

        while (!end){
            menu_handleKeyboard(menu, &action);

            switch (action.value){
                case 1 :
                    printf("1\n");
                    menu_update(menu);
                    break;

                case 2 :
                    printf("2\n");
                    menu_update(menu);
                    break;

                case 3 :
                    end = TRUE;
                    break;

                default :
                    break;
            }
        }

    }

    if (menu){
        menu_free(menu);
    }
    return 0;
}

//  _onStep : User steps on a box
//
//  @board : Pointer to the board
//  @pos : Position of the box
//  @redraw : pointer to the redraw indicator
//
//  @return : FALSE if stepped on a mine
//
//  _onStep : User steps on a box
//
//  @board : Pointer to the board
//  @pos : Position of the box
//  @redraw : pointer to the redraw indicator
//
//  @return : FALSE if stepped on a mine
//
BOOL _onStep(PBOARD const board, PCOORD const pos, uint16_t* redraw){
    uint8_t minesAround = 0;
    PBOX box = BOX_AT_POS(board->grid, pos);

    // Already stepped ???
    if (box->state > BS_QUESTION){
        (*redraw) = 255;
        return TRUE;
    }

    (*redraw) = 255;

    if (box->mine){
        // stepped on a mine!
        box->state =  BS_BLAST;
        board_drawBoxAtPos(board, pos);
        return FALSE;
    }

    minesAround = grid_countMines(board->grid, pos);
    board->steps++;
    box->state =  BS_DOWN - minesAround;
    board_drawBoxAtPos(board, pos);

    // Auto step surrounding boxes
    if (!minesAround){
        COORD nPos;
        int8_t r, rmin, rmax;
        int8_t c, cmin, cmax;

        rmin = pos->row-1;
        if (rmin<0){
            rmin =0;
        }
        rmax = pos->row+1;
        if (rmax>(int8_t)board->grid->size.row-1){
            rmax = (int8_t)board->grid->size.row-1;
        }

        cmin = pos->col-1;
        if (cmin<0){
            cmin =0;
        }
        cmax = pos->col+1;
        if (cmax>(int8_t)board->grid->size.col-1){
            cmax = (int8_t)board->grid->size.col-1;
        }

        for (r = rmin; r <= rmax; r++){
            for (c = cmin; c <= cmax; c++){
                if (r != pos->row || c != pos->col){
                    nPos = (COORD){.col = c, .row = r};
                    _onStep(board, &nPos, redraw);
                }
            }
        }
    }

    return TRUE;    // No mine at this pos
}

int main()
{
    // Création d'un menu
    //
    POWNMENU menu = menu_create();
    if (!menu){
        printf("Impossible de créer le menu\n");
        return 1;
    }

    // Premier sous menu
    PMENUBAR sub = menubar_create();
    if (sub){
        BOOL validKey, end = FALSE;
        MENUACTION action;
        PMENUBAR bar;

        menubar_appendItem(sub, IDM_GAME_BEGINNER, IDS_GAME_BEGINNER, ITEM_STATE_DEFAULT, ITEM_STATUS_TEXT);
        menubar_appendItem(sub, IDM_GAME_MEDIUM, IDS_GAME_MEDIUM, ITEM_STATE_DEFAULT, ITEM_STATUS_TEXT);
        menubar_appendItem(sub, IDM_GAME_EXPERT, IDS_GAME_EXPERT, ITEM_STATE_DEFAULT, ITEM_STATUS_TEXT);

        bar = menu_getMenuBar(menu);

        menubar_appendSubMenu(bar, sub, IDM_NEW_GAME, IDS_NEW_GAME, ITEM_STATE_DEFAULT, ITEM_STATUS_DEFAULT);
        menubar_appendItem(bar, IDM_CHECK, IDS_CHECK, ITEM_STATE_CHECKED, ITEM_STATUS_CHECKBOX);
        menubar_addItem(bar, MENU_POS_RIGHT, IDM_QUIT, IDS_QUIT, ITEM_STATE_DEFAULT, ITEM_STATUS_TEXT);

        // Affichage du menu
        menu_update(menu);

        while (!end){
            validKey = menu_handleKeyboard(menu, &action);

            if (validKey){
                switch (action.value){
                    case IDM_GAME_BEGINNER :{
                        PBOARD board = board_create();
                        COORD pos;
                        //uint16_t redraw = 0;
                        board_init(board, LEVEL_BEGINNER);

                        grid_display(board->grid);
                        printf("\n\n");

                        pos.col = 6;
                        pos.row = 4;
                        pos.col++;
                        //_onStep(board, &pos, &redraw);
                        board_drawGridEx(board, FALSE);

                        board_free(board, TRUE);

                        menu_showParentBar(menu, TRUE);
                        break;
                    }

                    case IDM_GAME_MEDIUM :
                        printf("Moyen\n");
                        menu_showParentBar(menu, TRUE);
                        break;

                    case IDM_GAME_EXPERT:
                        printf("Expert\n");
                        menu_showParentBar(menu, TRUE);
                        break;

                    case IDM_CHECK :
                        printf("check\n");
                        break;

                    case IDM_QUIT :
                        end = TRUE;
                        break;

                    default :
                        break;
                }
            }
        }

    }

    if (menu){
        menu_free(menu);
    }
    return 0;
}
