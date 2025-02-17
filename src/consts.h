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
#define APP_NAME     "geeMines"
#define APP_VERSION  "0.0.1"
#define APP_AUTHOR   "GeeHB"

// Menu
//
#include "shared/menu.h"

#define IDS_NEW             "New"
#define IDM_NEW             1

#define IDS_PRESS           "Press"
#define IDM_PRESS           2

#define IDS_FLAG            "Flag"
#define IDM_FLAG            3

#define IDS_QUESTION        "Question"
#define IDM_QUESTION        4

#define IDS_QUIT            "Quit"
#define IDM_QUIT            6

#define IDS_NEW_BEGINNNER   "Beginner"      // sub-menu
#define IDM_NEW_BEGINNER    11

#define IDS_NEW_MEDIUM      "Medium"
#define IDM_NEW_MEDIUM      12

#define IDS_NEW_EXPERT      "Expert"
#define IDM_NEW_EXPERT      13



// mine defines
#define LED_WIDTH           13
#define LED_HEIGHT          23
#define FACE_WIDTH          24
#define FACE_HEIGHT         24

// Game state
//
typedef enum {
    WAITING, PLAYING, GAMEWON, GAMELOST
    } GAME_STATE;

// Smiley state
//
typedef enum {
    FACE_DOWN, FACE_WIN, FACE_LOSE, FACE_CAUTION, FACE_HAPPY
    } FACE_STATE;


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
