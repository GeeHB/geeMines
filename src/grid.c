//----------------------------------------------------------------------
//--
//--    grid.c
//--
//--            A game grid
//--
//----------------------------------------------------------------------

#include "grid.h"

#ifdef DEST_CASIO_CALC
#include <gint/timer.h>
#include <gint/clock.h>
#else
#include <stdio.h>
#endif // #ifdef DEST_CASIO_CALC

#include <time.h>

// Internal consts
//

#define BEGINNER_MINES      10
#define BEGINNER_COLS       9
#define BEGINNER_ROWS       9

#define MEDIUM_MINES        40
#define MEDIUM_COLS         16
#define MEDIUM_ROWS         16

#define EXPERT_MINES        99
#define EXPERT_COLS         30
#define EXPERT_ROWS         16

//  grid_create() : Intialize a new grid
//
//  @grid : Pointer to the grid
//  @level : Difficulty of the new grid
//
//  @return : TRUE if done
//
BOOL grid_create(PGRID const grid, GAME_DIFFICULTY level){
    if (grid){
        grid->level_ = level;

        switch (level) {

            case MEDIUM:
                grid->minesCount_ = MEDIUM_MINES;
                grid->cols_ = MEDIUM_COLS;
                grid->rows_ = MEDIUM_ROWS;
                break;

            case EXPERT:
                grid->minesCount_ = EXPERT_MINES;
                grid->cols_ = EXPERT_COLS;
                grid->rows_ = EXPERT_ROWS;
                break;

            // ???
            default:
            case BEGINNER:
                grid->minesCount_ = BEGINNER_MINES;
                grid->cols_ = BEGINNER_COLS;
                grid->rows_ = BEGINNER_ROWS;
                break;
        }

        // Allocate memory for boxes
        if (grid->cols_){
            grid->boxes_ = (PBOX)malloc(grid->cols_ * grid->rows_ * sizeof(BOX));
            if (grid->boxes_){
                uint8_t r,c;
                PBOX box;
                for (r = 0; r < grid->rows_; r++){
                    for (c = 0; c < grid->cols_; c++){
                        box = GRID_AT(grid, r, c);
                        box->mine_ = FALSE;
                        box->state_ = BS_INITIAL;
                    }
                }

                return TRUE;    // Done
            }
        }

        grid->boxes_ = NULL;
    }

    // Error(s)
    return FALSE;
}

//  grid_layMines() : Put mines in the grid
//
//  @grid : Pointer to the grid
//  @level : Difficulty of the new grid
//
//  @return : count of mines in the current grid (0 if error)
//
uint8_t grid_layMines(PGRID const grid){

    uint8_t mines = 0;
    uint8_t r,c;
    PBOX box;

    if (grid && grid->minesCount_){
        srand((unsigned int)clock());
        while (mines < grid->minesCount_){
            r = (uint8_t)(rand() % grid->rows_);
            c = (uint8_t)(rand() % grid->cols_);
            box = GRID_AT(grid, r, c);

            if (!box->mine_){
                box->mine_ = TRUE;     // A new mine in a new pos.
                mines++;
            }
        }
    }

    return mines;
}

#ifndef DEST_CASIO_CALC
//  grid_display() : Display the grid (for tests on Linux)
//
//  @grid : Pointer to the grid
//
void grid_display(PGRID const grid){
    if (!grid){
        printf("Pointeur invalide\n");
        return;
    }
    uint8_t r,c;
    PBOX box;

    printf("\n\t%d x %d\n", grid->rows_, grid->cols_);

    for (r=0; r<grid->rows_; r++){
        for (c=0; c<grid->cols_; c++){
            box = GRID_AT(grid, r, c);
            printf("| %c ", box->mine_?'x':'0' + grid_countMines(grid, r, c));
        }
        printf("|\n");       // EOL
    }

    printf("\n%d mines\n", grid->minesCount_);
}
#endif // #ifndef DEST_CASIO_CALC

//  grid_countMines() : Count the mines surrounding the box
//
//  @grid : Pointer to the grid
//  @row, @col : Position of the box
//
//  @return : count of mines surrounding
//
uint8_t grid_countMines(PGRID const grid, uint8_t row, uint8_t col){
    uint8_t sMines = 0;
    int8_t r, c;

    for (r = (int8_t)row-1; r <= row+1; r++){
        for (c = (uint8_t)col-1; c <= col+1; c++){
            if (GRID_IS_VALID_POS(grid, r, c) && (r != row || c != col) &&
                GRID_AT(grid, r, c)->mine_){
                sMines++;
            }
        }
    }

    return sMines;
}

//  grid_stepBox : steps on a box
//
//  @grid : Pointer to the grid
//  @row, @col : Position of the box
//
//  @return : TRUE if pos is safe and FALSE if stepped on a bomb
//
BOOL grid_stepBox(PGRID const grid, uint8_t row, uint8_t col){
    if (grid){

        uint8_t surroundingMines = 0;
        PBOX box = GRID_AT(grid, col, row);

        surroundingMines = grid_countMines(grid, row, col);

        if (box->mine_){
            // stepped on a mine!
            box->state_ =  BS_BLAST;
            return FALSE;
        }

        box->state_ =  BS_DOWN - surroundingMines;

        // Auto step surrounding boxes
        if (!surroundingMines){
            int r, c;
            for (r = row-1; r <= row+1; r++){
                for (c = col-1; c <= col+1; c++){
                    if (GRID_IS_VALID_POS(grid, r, c) && (r != row || c != col)){
                        grid_stepBox(grid, r, c);
                    }
                }
            }
        }
    }

    // Valid step
    return TRUE;
}

//  grid_free() : Free memory allocated for a grid
//
//  @grid : Pointer to the grid
//
void grid_free(PGRID const grid){
    if (grid && grid->boxes_){
        free(grid->boxes_);
        grid->boxes_ = NULL;
    }
}

// EOF
