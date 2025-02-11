//----------------------------------------------------------------------
//--
//--    keyboard.cpp
//--
//--    Keybor management functions and consts
//--
//----------------------------------------------------------------------

#include "keys.h"

// getKeyEx() : Get key pressed code
//
//  @mod : if not NULL will receive modifier code
//
//  @return : key code or KEY_NONE
//
uint getKeyEx(uint* mod){
    uint key = KEY_NONE;
    uint rmod = MOD_NONE;

#ifdef DEST_CASIO_CALC
    key_event_t evt;
    evt = pollevent();
    if (evt.type == KEYEV_DOWN){
        key = evt.key;

        // Modifiers
        //
        if (key == KEY_SHIFT){
            if (!_isSet(MOD_SHIFT)){
                rmod = SET_BIT(rmod, MOD_SHIFT);
            }
        }
        else{
            if (key == KEY_ALPHA){
                if (!_isSet(MOD_ALPHA)){
                    rmod = SET_BIT(rmod, MOD_ALPHA);
                }
            }
        }
    }
    else{
        if (evt.type == KEYEV_UP){
            key = evt.key;

            // Modifiers
            //
            if (key == KEY_SHIFT){
                rmod = REMOVE_BIT(rmod, MOD_SHIFT);
            }
            else{
                if (key == KEY_ALPHA){
                    rmod = REMOVE_BIT(rmod, MOD_ALPHA);
                }
            }
        }

        key = KEY_NONE;
        if (mod){
            *mod = rmod;
        }
    }
#else
    key = getchar();
    if (key == 10) key = 0; // CR
    if (mod){
        *mod = MOD_NONE;
    }
#endif // #ifdef DEST_CASIO_CALC

    // return the pressed key code
    return key;
}

uint getKey(){
    return getKeyEx(NULL);
}

// EOF
