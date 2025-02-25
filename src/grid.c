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

#include <string.h>
#include <time.h>

// Internal consts
//


//  grid_create() :Create a grid
//
//  @return : pointer to the new created grid
//
PGRID grid_create(){
    size_t size = sizeof(GRID);
    PGRID grid = (PGRID)malloc(size);
    if (grid){
        /*
        grid->minesCount_ = 0;     // count of mines at startup
        grid->cols_ = 0;           // Grid dimensions
        grid->rows_ = 0;
        grid->boxes_ = NULL;
        */
        memset(grid, 0, size);
        grid->level = LEVEL_BEGINNER;
    }
    return grid;
}

//  grid_init() : Intialize an existing grid
//
//  @grid : Pointer to the grid
//  @level : Difficulty of the new grid
//
//  @return : TRUE if done
//
BOOL grid_init(PGRID const grid, GAME_LEVEL level){
    if (grid){
        grid_free(grid, FALSE); // Clear previous if any

        grid->level = level;
        switch (level) {

            case LEVEL_MEDIUM:
                grid->minesCount = MEDIUM_MINES;
                grid->cols = MEDIUM_COLS;
                grid->rows = MEDIUM_ROWS;
                break;

            case LEVEL_EXPERT:
                grid->minesCount = EXPERT_MINES;
                grid->cols = EXPERT_COLS;
                grid->rows = EXPERT_ROWS;
                break;

            // ???
            default:
            case LEVEL_BEGINNER:
                grid->minesCount = BEGINNER_MINES;
                grid->cols = BEGINNER_COLS;
                grid->rows = BEGINNER_ROWS;
                break;
        }

        // Allocate memory for boxes
        if (grid->cols){
            grid->boxes = (PBOX)malloc(grid->cols * grid->rows * sizeof(BOX));
            if (grid->boxes){
                uint8_t r,c;
                PBOX box;
                for (r = 0; r < grid->rows; r++){
                    for (c = 0; c < grid->cols; c++){
                        box = GRID_AT(grid, r, c);
                        box->mine = FALSE;
                        box->state = BS_INITIAL;
                    }
                }

                return TRUE;    // Done
            }
        }

        grid->boxes = NULL;
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

    if (grid && grid->minesCount){
        srand((unsigned int)clock());
        while (mines < grid->minesCount){
            r = (uint8_t)(rand() % grid->rows);
            c = (uint8_t)(rand() % grid->cols);
            box = GRID_AT(grid, r, c);

            if (!box->mine){
                box->mine = TRUE;     // A new mine in a new pos.
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

    printf("\n\t%d x %d\n", grid->rows, grid->cols);

    for (r=0; r<grid->rows; r++){
        for (c=0; c<grid->cols; c++){
            box = GRID_AT(grid, r, c);
            printf("| %c ", box->mine?'x':'0' + grid_countMines(grid, r, c));
        }
        printf("|\n");       // EOL
    }

    printf("\n%d mines\n", grid->minesCount);
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
                GRID_AT(grid, r, c)->mine){
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

        if (box->mine){
            // stepped on a mine!
            box->state =  BS_BLAST;
            return FALSE;
        }

        box->state =  BS_DOWN - surroundingMines;

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
//  @freeAll : if FALSE only boxes are freed. If TRUE boxes and grid memory will
//              be freed
//
//  @return : pointer to grid or NULL if freed
//
PGRID grid_free(PGRID const grid, BOOL freeAll){
    if (grid){
        if (grid->boxes){
            free(grid->boxes);
            grid->boxes = NULL;
        }

        if (freeAll){
            free(grid);
            return NULL;
        }
    }

    return grid;
}

// EOF
