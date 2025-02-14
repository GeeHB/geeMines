//----------------------------------------------------------------------
//--
//--    consts.h
//--
//--            Shared types and constants pour geeMine app.
//--
//----------------------------------------------------------------------

#ifndef __GEE_MINES_CONSTS_h__
#define __GEE_MINES_CONSTS_h__    1

// Application infos
//
#define APP_NAME     "geeMines"
#define APP_VERSION  "0.0.1"
#define APP_AUTHOR   "GeeHB"


// mine defines
#define BOX_WIDTH           16
#define BOX_HEIGHT          16
#define LED_WIDTH           13
#define LED_HEIGHT          23
#define FACE_WIDTH          24
#define FACE_HEIGHT         24

// client area width = LEFT_MARGIN + LEFT_SPACE + GRID_BORDER
//  + GRID_WIDTH(uCols*BOX_WIDTH) + GRID_BORDER + RIGHT_SPACE
#define LEFT_MARGIN         3
#define LEFT_SPACE          6
#define GRID_BORDER         3
#define RIGHT_SPACE         5

// client area height = TOP_MARGIN + TOP_SPACE
//  + STAT_BORDER + STAT_HEIGHT + STAT_BORDER
//  + VCENTER_SPACE
//  + GRID_BORDER + GRID_HEIGHT(uRows*BOX_HEIGHT) + GRID_BORDER
//  + BOTTOM_SPACE
#define TOP_MARGIN          3
#define TOP_SPACE           6
#define STAT_BORDER         2
#define STAT_HEIGHT         33
#define VCENTER_SPACE       6
#define BOTTOM_SPACE        5
// STAT_WIDTH not defined since it is determined by other parameters
// GRID_BORDER*2 + GRID_WIDTH = STAT_BORDER*2 + STAT_WIDTH

// geometric parameters within the sunken area
#define STAT_TOP_SPACE      4
#define STAT_LEFT_SPACE     5
#define STAT_RIGHT_SPACE    7
// no STAT_BOTTOM_SPACE since it is determined by other parameters

#define STAT_RECT_LEFT      (LEFT_MARGIN + LEFT_SPACE + STAT_BORDER)
#define STAT_RECT_TOP       (TOP_MARGIN + TOP_SPACE + STAT_BORDER)
#define STAT_RECT_BOTTOM    (STAT_RECT_TOP + STAT_HEIGHT)

#define COUNTER_RECT_LEFT   (STAT_RECT_LEFT + STAT_LEFT_SPACE + 1)
#define COUNTER_RECT_TOP    (STAT_RECT_TOP + STAT_TOP_SPACE + 1)

#define GRID_RECT_LEFT      (LEFT_MARGIN + LEFT_SPACE + GRID_BORDER)
#define GRID_RECT_TOP       (STAT_RECT_BOTTOM + STAT_BORDER \
                            + VCENTER_SPACE + GRID_BORDER)

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
void LayMines(int row, int col);
void Pt2RowCol(POINT pt, int *prow, int *pcol);
void GameWon();
void GameLost();
void PressBox(int row, int col);
void UnPressBox();
UINT CountMines(int row, int col);
BOOL StepBox(int row, int col);
*/

// ???
//
/*
#define MAX_COLS            30
#define MAX_ROWS            24
*/

#endif // #ifdef __GEE_MINES_CONSTS_h__
