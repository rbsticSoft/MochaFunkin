#include <stdio.h>
#include <assert.h>
#include "states.h"
#include "fnf_data.h"

FILE* fnf_save;
typedef struct __attribute__((packed)) {
    char id;
    uint32_t score;
} fnf_score;

void init_saves(){
    fnf_save = fopen("save.fnf", "r+");
    if(fnf_save)
        return;

    fnf_save = fopen("save.fnf", "wb+");

    int songs_size = rarray_size(week_songs) * SONGS_PER_WEEK;

    fnf_score* temp = (fnf_score*)malloc((songs_size) * sizeof(fnf_score));
    for(int i=0; i<songs_size; i++){
        temp[i].id = i;
        temp[i].score = 0;
    }
    fwrite(temp, songs_size, 5, fnf_save);
    //free(temp);
}

void uninit_saves(){
    fclose(fnf_save);
}

char find_song(char* song){
    for(int i=0; i<rarray_size(week_songs); i++){
        for(int s=0; s<3; s++) {
            if(!week_songs[i][s]) continue;

            if (strcmp(week_songs[i][s], song) == 0) {
                return i;
            }
        }
    }
    return -1;
}

void set_highscore(char* song, uint32_t score){
    char position = find_song(song);
    if(position == -1)
        return;

    assert(position < rarray_size(week_songs) * SONGS_PER_WEEK);
    fseek(fnf_save, position * 5, SEEK_SET);
    fnf_score temp = {
            .id = position,
            .score = score
    };

    fwrite(&temp, 1, sizeof(fnf_score), fnf_save);
}

uint32_t get_highscore(char* song){
    char position = find_song(song);
    if(position == -1)
        return 0;

    fseek(fnf_save, position * 5, SEEK_SET);

    fnf_score score;
    fread(&score, 1, sizeof(fnf_score), fnf_save);
    return score.score;
}