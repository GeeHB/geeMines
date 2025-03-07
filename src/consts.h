//----------------------------------------------------------------------
//--
//--    consts.h
//--
//--            Shared types and constants pour geeMine app.
//--
//----------------------------------------------------------------------

// other sample : https://github.com/reactos/reactos/tree/master/base/applications/games/winmine
// https://minesweeper.online/fr/game/4289190080

#ifndef __GEE_MINES_CONSTS_h__
#define __GEE_MINES_CONSTS_h__    1

// Application infos
//
#define APP_NAME            "geeMines"
#define APP_VERSION         "0.0.1"
#define APP_AUTHOR          "GeeHB"

// Main menu
//
#define IDS_NEW             "New"
#define IDM_NEW             1

#define IDS_START           "Start"
#define IDM_START           2

#define IDS_QUIT            "Quit"
#define IDM_QUIT            6

#define IDS_NEW_BEGINNER    "Beginner"      // sub-menu
#define IDM_NEW_BEGINNER    11

#define IDS_NEW_MEDIUM      "Medium"
#define IDM_NEW_MEDIUM      12

#define IDS_NEW_EXPERT      "Expert"
#define IDM_NEW_EXPERT      13

// Game menu
//
#define IDS_PRESS           "Press"
#define IDM_PRESS           101

#define IDS_FLAG            "Flag"
#define IDM_FLAG            102

#define IDS_QUESTION        "Question"
#define IDM_QUESTION        103

#define IDS_CANCEL          "Cancel"
#define IDM_CANCEL          106


// "pause" image
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
    KEY_CODE_EXE = KEY_EXE
};
#else
enum GAME_KEY{
    KEY_CODE_F1 = 'a',
    KEY_CODE_F6 = 'f',
    KEY_CODE_UP = '8',
    KEY_CODE_DOWN = '2',
    KEY_CODE_LEFT = '4',
    KEY_CODE_RIGHT = '6',
    KEY_CODE_CAPTURE = '*',
    KEY_CODE_ROTATE_DISPLAY = 'r',
    KEY_CODE_PAUSE = 'p',
    KEY_CODE_EXIT = 'q',
    KEY_CODE_EXE = 13
};
#endif // #ifdef DEST_CASIO_CALC

#define COL_BKGROUND    C_RGB(192,192,192)

/*
typedef struct
{
    HINSTANCE hInst;
    HWND hWnd;
    int cxNcLeft;
    int cyNcTop;
    int cxNcRight;
    int cyNcBottom;
    HDC hMemDCLed;
    HDC hMemDCFace;
    HDC hMemDCBox;

    POINT Pos;
    DIFFICULTY Difficulty;
    UINT uWidth;
    UINT uHeight;
    UINT uRows;
    UINT uCols;
    UINT uMines;
    UINT uMaxSteps;

    RECT StatRect;
    RECT CounterRect;
    RECT FaceRect;
    RECT TimerRect;
    RECT GridRect;

    GAME_STATE GameState;
    FACE_STATE FaceState;
    UINT uMinesLeft;
    UINT uTime;
    UINT uSteps;
    BOOL bMark;
    int colPressed;
    int rowPressed;

    BOX_STRUCT Box[MAX_ROWS][MAX_COLS];

} BOARD;


void InitBoard();
void LoadBoard();
void SaveBoard();
void DestroyBoard();
void CheckLevel();
void NewBoard();
void NewGame();
void TranslateMouseMsg(UINT* puMsg, WPARAM wParam);
void ProcessMouseMsg(UINT uMsg, LPARAM lParam);
void OnCommand(WPARAM wParam, LPARAM lParam);
void SetDifficulty(DIFFICULTY Difficulty);
void DrawBackground(HDC hDC);
void DrawMinesLeft(HDC hdc);
void DrawFace(HDC hdc);
void DrawTime(HDC hdc);
void DrawGrid(HDC hdc);
void DisplayFace(FACE_STATE faceState);
void SetAndDispBoxState(int r, int c, BOX_STATE state);
void DecMinesLeft();
void IncMinesLeft();
void ZeroMinesLeft();
void IncTime();
void Pt2RowCol(POINT pt, int *prow, int *pcol);
void GameWon();
void GameLost();
void PressBox(int row, int col);
void UnPressBox();
*/

// ???
//
/*
#define MAX_COLS            30
#define MAX_ROWS            24
*/

#endif // #ifdef __GEE_MINES_CONSTS_h__

// EOF
