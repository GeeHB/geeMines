//----------------------------------------------------------------------
//--
//--    scores.h
//--
//--            Scores management
//--
//----------------------------------------------------------------------

#ifndef __GEE_MINES_SCORES_h__
#define __GEE_MINES_SCORES_h__    1

#include "shared/casioCalcs.h"
#include "consts.h"

#ifdef DEST_CASIO_CALC
#define SCORE_FILE            "_geeMines.top"
#else
#define SCORE_FILE            "/home/jhb/bidon/geeMines.top"
#endif // #ifdef DEST_CASIO_CALC

#define SCORE_LEVEL_COUNT     5     // # per level
#define SCORE_COUNT           (3 * SCORE_LEVEL_COUNT)

// A score
typedef struct __score{
    uint8_t level :2;
    uint16_t time :14;      // in dsec
}SCORE, * PSCORE;

#define SCORE_SIZE          2//  sizeof(SCORE)

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

//  scores_load() : Load the scores list from disk
//
//  @return a pointer to the scores or NULL
//
PSCORE scores_load();

//  scores_save() : Save the scores list to the disk
//
//  @scores : Pointer to the scores list
//
//  @return TRUE if done
//
BOOL scores_save(PSCORE const scores);

//  scores_free() : Free the list of scores
//
//  @scores : Pointer to the scores list
//
void scores_free(PSCORE const scores);

//  scores_add() : Add a new score to the list
//
//  @scores : Pointer to the scores list
//  @level, @tipme : score's values
//
//  @return TRUE if added (ie. the score is in the high-scores list)
//
BOOL scores_add(PSCORE const scores, uint8_t level, uint16_t time);

//  scores_display() : Display the scores
//
//  @scores : Pointer to the scores list
//
void scores_display(PSCORE const scores);

// scores_time2a() : Convert a game duration val to a string
//
//  @num : Time to convert
//  @str : destination buffer (must be large enough)
//
//  @return : a pointer to the string
//
char* scores_time2a(int num, char *str);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_MINES_SCORES_h__

// EOF
