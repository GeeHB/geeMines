//----------------------------------------------------------------------
//--
//--    scores.c
//--
//--            Scores management
//--
//----------------------------------------------------------------------

#include "scores.h"
#include "grid.h"

#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#ifdef DEST_CASIO_CALC
#else
#include <stdio.h>
#endif // #ifdef DEST_CASIO_CALC

//  scores_load() : Load the scores list from disk
//
//  @return a pointer to the scores or NULL
//
PSCORE scores_load(){
    PSCORE scores = NULL;
    int fd = open(SCORE_FILE, O_RDONLY | O_CREAT);
    if (fd != -1){
        size_t len = SCORE_SIZE * SCORE_COUNT;
        scores = (PSCORE)malloc(len);
        if (scores){
            memset(scores, 0, len);
            if (read(fd, (void*)scores, len) != len){
                // set default values
                for (uint8_t id=0; id < SCORE_COUNT; id++){
                    scores[id].level = id / SCORE_LEVEL_COUNT;
                    scores[id].time = 50 + 150 * scores[id].level;
                }
            }
        }
        close(fd);
    }

    return scores;
}

//  scores_save() : Save the scores list to the disk
//
//  @scores : Pointer to the scores list
//
//  @return TRUE if done
//
BOOL scores_save(PSCORE const scores){
    if (scores){
        int fd = open(SCORE_FILE, O_WRONLY | O_CREAT);
        if (fd != -1){
            size_t len = SCORE_SIZE * SCORE_COUNT;
            size_t ct = write(fd, scores, len);
            close(fd);

            return (ct == len);
        }
    }

    return FALSE;
}

//  scores_free() : Free the list of scores
//
//  @scores : Pointer to the scores list
//
void scores_free(PSCORE const scores){
    if (scores){
        free(scores);
    }
}

//  scores_add() : Add a new score to the list
//
//  @scores : Pointer to the scores list
//  @level, @tipme : score's values
//
//  @return TRUE if added (ie. the score is in the high-scores list)
//
BOOL scores_add(PSCORE const scores, uint8_t level, uint16_t time){
    BOOL found = FALSE, add = FALSE;
    uint8_t id, pos=0, count;

    if (!scores || level > LEVEL_EXPERT){
        return FALSE;
    }

    for (id=(level * SCORE_LEVEL_COUNT); !found && id < (level+1) * SCORE_LEVEL_COUNT; id++){
        if (scores[id].time == time){
            found = TRUE;
            add = FALSE;
        }
        else if (scores[id].time>time){
                pos = id;
                found = TRUE;
                add = TRUE;
            }
    }

    if (!found || !add){
        return FALSE;
    }

    count = SCORE_LEVEL_COUNT - (pos % SCORE_LEVEL_COUNT) - 1;
    if (count){
        for (id = pos + count; id > pos; id--){
            scores[id].time = scores[id-1].time;
        }
    }

    scores[pos].time = time;
    return TRUE;
}

//  scores_display() : Display the scores
//
//  @scores : Pointer to the scores list
//
void scores_display(PSCORE scores){
    if (scores){
#ifdef DEST_CASIO_CALC
#else
        for (uint8_t id=0; id<SCORE_COUNT; id++){
            printf("\t- %d : %d\n", scores[id].level, scores[id].time);
        }
#endif // #ifdef DEST_CASIO_CALC
    }
}

// scores_time2a() : Convert a game duration val to a string
//
//  @num : Time to convert
//  @str : destination buffer (must be large enough)
//
//  @return : a pointer to the string
//
char* scores_time2a(int num, char *str){
    if (str){
        size_t len;
        __itoa((num<10?10:num), str);   // not less than 1 sec.

        if ((len = strlen(str))>1){
            str[len] = str[len-1];
            str[len-1] = '.';
            str[len+1] = 0;
        }

    }
    return str;
}


// EOF
