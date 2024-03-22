#ifndef CONDUCTOR_H
#define CONDUCTOR_H

typedef struct {
    float bpm;
    float crochet;
    float stepCrochet;
    float songPosition;
    float lastSongPos;
    float offset;
    int safeFrames;
    float safeZoneOffset;
} fnf_conductor_t;

extern fnf_conductor_t* current_conductor;

void init_conductor(fnf_conductor_t* conductor);
void change_bpm(fnf_conductor_t* conductor, float newBpm);

#endif // CONDUCTOR_H