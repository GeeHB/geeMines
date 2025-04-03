//----------------------------------------------------------------------
//--
//--    consts.h
//--
//--            Shared types and constants for geeMine app.
//--
//----------------------------------------------------------------------

#ifndef __GEE_MINES_CONSTS_h__
#define __GEE_MINES_CONSTS_h__    1

// Application infos
//
#define APP_NAME            "geeMines"
#define APP_VERSION         "0.2.2"
#define APP_AUTHOR          "GeeHB"

// "About" / Splash screen
//
#define APP_LOGO_WIDTH      80
#define APP_LOGO_HEIGHT     APP_LOGO_WIDTH
#define APP_LOGO_OFFSET     30

#define BKGROUND_COLOUR     C_RGB(26,26,26)
//#define BKGROUND_COLOUR     COLOUR_LT_GREY
#define WINDOW_COLOUR       COLOUR_WHITE //C_RGB(19,24,27)

// Main menu
//
#define IDM_NEW             1
#define IDS_NEW             "New"

#define IDM_START           2
#define IDS_START           "Start"

#ifdef _DEBUG_
#define IDM_DEBUG           3
#define IDS_DEBUG           "Debug"
#endif // #ifdef _DEBUG_

#define IDM_SCORES          5
#define IDS_SCORES          "Scores"

#define IDM_QUIT            6
#define IDS_QUIT            "Quit"

#define IDM_NEW_BEGINNER    11
#define IDS_NEW_BEGINNER    "Beginner"      // sub-menu

#define IDM_NEW_MEDIUM      12
#define IDS_NEW_MEDIUM      "Medium"

#define IDM_NEW_EXPERT      13
#define IDS_NEW_EXPERT      "Expert"

// Game menu
//
#define IDM_STEP            101
#define IDS_STEP            "Step"

#define IDM_FLAG            102
#define IDS_FLAG            "Flag"

#define IDM_QUESTION        103
//#define IDS_QUESTION        "?"
#define IDS_QUESTION        "Question"

#define IDM_CANCEL          106
#define IDS_CANCEL          "Cancel"

// "pause" image
//
#ifdef DEST_CASIO_CALC
    #define IMG_PAUSE_W         CASIO_WIDTH
    #define IMG_PAUSE_H         70
    #define IMG_PAUSE_COPY_Y    46
    #define IMG_PAUSE_LINES     (CASIO_HEIGHT - IMG_PAUSE_H)
#endif // #ifdef DEST_CASIO_CALC

// Key codes
//
#ifdef DEST_CASIO_CALC
#include "shared/keys.h"
enum GAME_KEY{
    KEY_CODE_F1 = KEY_F1,     // !!!
    KEY_CODE_F6 = KEY_F6,
    KEY_CODE_UP = KEY_UP,
    KEY_CODE_DOWN = KEY_DOWN,
    KEY_CODE_LEFT = KEY_LEFT,
    KEY_CODE_RIGHT = KEY_RIGHT,
    KEY_CODE_CAPTURE = KEY_7,
    KEY_CODE_ROTATE_DISPLAY = KEY_XOT,
    KEY_CODE_PAUSE = KEY_OPTN,
    KEY_CODE_EXIT = KEY_EXIT,
    KEY_CODE_STEP = KEY_EXE,
    KEY_CODE_FLAG = KEY_ADD,
    KEY_CODE_QUESTION = KEY_SUB
};
#else
enum GAME_KEY{
    KEY_CODE_F1 = 'a',
    KEY_CODE_F6 = 'f',
    KEY_CODE_UP = '8',
    KEY_CODE_DOWN = '2',
    KEY_CODE_LEFT = '4',
    KEY_CODE_RIGHT = '6',
    KEY_CODE_CAPTURE = '7',
    KEY_CODE_ROTATE_DISPLAY = 'r',
    KEY_CODE_PAUSE = 'p',
    KEY_CODE_EXIT = 'q',
    KEY_CODE_STEP = 13,
    KEY_CODE_FLAG = '+',
    KEY_CODE_QUESTION = '-'
};
#endif // #ifdef DEST_CASIO_CALC

#endif // #ifdef __GEE_MINES_CONSTS_h__

// EOF
