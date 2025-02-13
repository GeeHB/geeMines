#include <stdio.h>
#include <stdlib.h>

#include "../src/shared/menu.h"

int main()
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
