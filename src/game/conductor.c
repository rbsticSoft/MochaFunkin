#include "conductor.h"

fnf_conductor_t* current_conductor = 0;

void init_conductor(fnf_conductor_t* conductor) {
    conductor->bpm = 100.0f;
    conductor->crochet = (60.0f / conductor->bpm) * 1000.0f;
    conductor->stepCrochet = conductor->crochet / 4.0f;
    conductor->songPosition = 0.0f;
    conductor->lastSongPos = 0.0f;
    conductor->offset = 0.0f;
    conductor->safeFrames = 10;
    conductor->safeZoneOffset = (conductor->safeFrames / 60.0f) * 1000.0f;
}

void change_bpm(fnf_conductor_t* conductor, float newBpm){
    conductor->bpm = newBpm;

    conductor->crochet = ((60.f / conductor->bpm) * 1000.f);
    conductor->stepCrochet = conductor->crochet / 4.f;
}