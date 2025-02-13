#include <stdio.h>
#include <stdlib.h>

#include "../src/shared/menu.h"

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

#define IDS_QUIT       "Quitter"
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
    PMENUBAR bar = menu_getMainBar(menu);
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
        BOOL end = FALSE;
        MENUACTION action;
        PMENUBAR bar;

        menubar_appendItem(sub, IDM_GAME_BEGINNER, IDS_GAME_BEGINNER, ITEM_STATUS_DEFAULT, ITEM_STATUS_DEFAULT);
        menubar_appendItem(sub, IDM_GAME_MEDIUM, IDS_GAME_MEDIUM, ITEM_STATUS_DEFAULT, ITEM_STATUS_DEFAULT);
        menubar_appendItem(sub, IDM_GAME_EXPERT, IDS_GAME_EXPERT, ITEM_STATUS_DEFAULT, ITEM_STATUS_DEFAULT);

        bar = menu_getMainBar(menu);

        menubar_appendSubMenu(bar, sub, IDM_NEW_GAME, IDS_NEW_GAME, ITEM_STATE_DEFAULT, ITEM_STATUS_DEFAULT);
        menubar_appendItem(bar, IDM_CHECK, IDS_CHECK, ITEM_STATE_CHECKED, ITEM_STATUS_CHECKBOX);
        menubar_addItem(bar, MENU_POS_RIGHT, IDM_QUIT, IDS_QUIT, ITEM_STATE_DEFAULT, ITEM_STATUS_DEFAULT);

        // Affichage du menu
        menu_update(menu);

        while (!end){
            menu_handleKeyboard(menu, &action);

            switch (action.value){
                case IDM_GAME_BEGINNER :
                    printf("Débutant\n");
                    menu_showParentBar(menu, TRUE);
                    break;

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

    if (menu){
        menu_free(menu);
    }
    return 0;
}
