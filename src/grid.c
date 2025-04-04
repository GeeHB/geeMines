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

//  grid_create() : Create a grid
//
//  @return : pointer to the new created grid
//
PGRID grid_create(){
    size_t size = sizeof(GRID);
    PGRID grid = (PGRID)malloc(size);
    if (grid){
        memset(grid, 0, size);
        //grid->level = LEVEL_BEGINNER;
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
                grid->mines = MEDIUM_MINES;
                grid->size.col = MEDIUM_COLS;
                grid->size.row = MEDIUM_ROWS;
                break;

            case LEVEL_EXPERT:
                grid->mines = EXPERT_MINES;
                grid->size.col = EXPERT_COLS;
                grid->size.row = EXPERT_ROWS;
                break;

            // ???
            default:
            case LEVEL_BEGINNER:
                grid->mines = BEGINNER_MINES;
                grid->size.col = BEGINNER_COLS;
                grid->size.row = BEGINNER_ROWS;
                break;
        }

        // Allocate memory for boxes
        if (grid->size.col){
            grid->boxes = (PBOX)malloc(grid->size.col * grid->size.row * sizeof(BOX));
            if (grid->boxes){
                uint8_t r,c;
                PBOX box;
                for (r = 0; r < grid->size.row; r++){
                    for (c = 0; c < grid->size.col; c++){
                        box = BOX_AT(grid, r, c);
                        box->mine = FALSE;
                        box->state = BS_INITIAL;
                    }
                }

                grid->maxSteps = grid->size.col * grid->size.row - grid->mines;
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

    if (grid && grid->mines){
        srand((unsigned int)clock());
        while (mines < grid->mines){
            r = (uint8_t)(rand() % grid->size.row);
            c = (uint8_t)(rand() % grid->size.col);
            box = BOX_AT(grid, r, c);

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
    COORD nPos;

    printf("\n\t%d x %d\n", grid->size.row, grid->size.col);

    for (r=0; r<grid->size.row; r++){
        for (c=0; c<grid->size.col; c++){
            box = BOX_AT(grid, r, c);
            nPos = (COORD){.col = c, .row = r};
            printf("| %c ", box->mine?'x':'0' + grid_countMines(grid, &nPos));
        }
        printf("|\n");       // EOL
    }

    printf("\n%d mines\n", grid->mines);
}
#endif // #ifndef DEST_CASIO_CALC

//  grid_countMines() : Count the mines surrounding the box
//
//  @grid : Pointer to the grid
//  @pos : Position of the box
//
//  @return : count of mines surrounding
//
uint8_t grid_countMines(PGRID const grid, PCOORD const pos){
    uint8_t sMines = 0;
    int8_t r,c;
    for (r = SET_IN_RANGE(pos->row-1, 0, grid->size.row - 1);
        r <= SET_IN_RANGE(pos->row+1, 0, grid->size.row - 1); r++){
        for (c = SET_IN_RANGE(pos->col-1, 0, grid->size.col - 1);
            c <= SET_IN_RANGE(pos->col+1, 0, grid->size.col - 1); c++){
            if (!(r == pos->row && c == pos->col) &&
                BOX_AT(grid, r, c)->mine){
                sMines++;
            }
        }
    }

    return sMines;
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
