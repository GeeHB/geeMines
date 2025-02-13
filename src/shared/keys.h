//----------------------------------------------------------------------
//--
//--    keys.h
//--
//--    Keybor management functions and consts
//--
//----------------------------------------------------------------------

#ifndef __GEE_TOOLS_KEYBOARD_C_h__
#define __GEE_TOOLS_KEYBOARD_C_h__    1

#include "casioCalcs.h"

#ifdef DEST_CASIO_CALC
#include <gint/keyboard.h>
#else
#include <stdint.h>
#include <stdio.h>
typedef unsigned int uint;  // !!!
#endif // #ifdef DEST_CASIO_CALC

#ifndef KEY_NONE
#define KEY_NONE       0
#endif // #ifndef KEY_NONE

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

// Type of key modifiers
//
#define MOD_NONE    0
#define MOD_SHIFT   1       // Shift key pressed
#define MOD_ALPHA   2

// getKeyEx() : Get key pressed code
//
//  @mod : if not NULL will receive modifier code
//
//  @return : key code or KEY_NONE
//
uint getKeyEx(uint* mod);
uint getKey();

// State & status - bitwise manips
//
BOOL isBitSet(int value, int bit);
int setBit(int value, int bit);
int removeBit(int value, int bit);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_TOOLS_KEYBOARD_C_h__

// EOF
