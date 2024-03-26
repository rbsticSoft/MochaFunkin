#ifndef NOTE_H
#define NOTE_H
#include "sprite.h"

typedef struct {
    float strum_time;
    char note_data;
    bool must_press, will_miss, was_good_hit, can_be_hit, too_late, sustain, dead;
    fnf_sprite* sprite;
} fnf_note;

typedef struct {
    char name[256];
    float bpm, speed;
    char bf[32], dad[32];
} fnf_song;

#endif // NOTE_H