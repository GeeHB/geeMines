//----------------------------------------------------------------------
//--
//--    geeMines.c
//--
//--            App. entry-point
//--
//----------------------------------------------------------------------

#include "shared/casioCalcs.h"
#include "consts.h"

#include "shared/menu.h"

#ifdef DEST_CASIO_CALC
    #include "shared/keys.h"
    #include <math.h>
    #include <string.h>

    // Background image
    extern bopti_image_t g_about;
#endif // #ifdef DEST_CASIO_CALC

// _clearScreen() : Clear the whole screen
//
void _clearScreen(){
#ifdef DEST_CASIO_CALC
    //dclear(COLOUR_WHITE);
    drect(0, 0, CASIO_WIDTH - 1, CASIO_HEIGHT- 1, C_WHITE);
#endif // #ifdef DEST_CASIO_CALC
}

// _onAbout() : Show "about" informations
//
void _onAbout(){

    _clearScreen();

    // Draw the image and copyright
#ifdef DEST_CASIO_CALC
    dimage(0, 0, &g_about);

    char copyright[255];    // Should be enough !
    strcpy(copyright, APP_NAME);
#ifdef FXCG50
    strcat(copyright, " par ");
    strcat(copyright, APP_AUTHOR);
#endif // #ifdef FX9860G
    strcat(copyright, " v");
    strcat(copyright, APP_VERSION);

    int w, h;
    dsize(copyright, NULL, &w, &h);
    dtext(CASIO_WIDTH - w - 5,
            CASIO_HEIGHT - MENUBAR_DEF_HEIGHT - h - 10,
            COLOUR_BLACK, copyright);

    dupdate();
#endif // #ifdef DEST_CASIO_CALC
}

int main(void){
#ifdef DEST_CASIO_CALC
    int i;
	dclear(C_WHITE);
	dtext(1, 1, C_BLACK, "Sample fxSDK add-in.");
	dupdate();

	srand(1000);
	i = rand() % 20;
	i++;

	float a = 18.3;
	float b = floor(a);
	b+=1.0;

	getkey();
#endif // #ifdef DEST_CASIO_CALC
	return 1;
}
