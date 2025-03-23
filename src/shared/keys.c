//----------------------------------------------------------------------
//--
//--    keyboard.cpp
//--
//--    Keyboard management functions and consts
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
#ifdef DEST_CASIO_CALC
    uint modifier = MOD_NONE;
    key_event_t evt;
    evt = pollevent();
    if (evt.type == KEYEV_DOWN){
        key = evt.key;

        // Modifiers
        //
        if (key == KEY_SHIFT){
            if (!isBitSet(modifier, MOD_SHIFT)){
                modifier = setBit(modifier, MOD_SHIFT);
            }
        }
        else{
            if (key == KEY_ALPHA){
                if (!isBitSet(modifier, MOD_ALPHA)){
                    modifier = setBit(modifier, MOD_ALPHA);
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
                modifier = removeBit(modifier, MOD_SHIFT);
            }
            else{
                if (key == KEY_ALPHA){
                    modifier = removeBit(modifier, MOD_ALPHA);
                }
            }
        }

        key = KEY_NONE;
        if (mod){
            *mod = modifier;
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

// State & status - bitwise manips
//
BOOL isBitSet(int value, int bit){
    return (bit == (value & bit));
 }
int setBit(int value, int bit){
    return (value |= bit);
}
int removeBit(int value, int bit){
    return (value & ~bit);
}

// EOF
