#ifndef AUDIO_H
#define AUDIO_H
#include "types.h"

#include <AL/al.h>
#include <AL/alc.h>

typedef struct {
    uint32 buffer;
    uint32 source;
} fnf_audio;

extern fnf_audio* select_menu;
extern fnf_audio* back_menu;
extern fnf_audio* accept_menu;

void init_audio();
void uninit_audio();

fnf_audio make_audio();
fnf_audio* load_audio(fnf_audio* audio, const char* path);
fnf_audio* play_audio(fnf_audio* audio);
fnf_audio* pause_audio(fnf_audio* audio);
fnf_audio* stop_audio(fnf_audio* audio);
bool* stopped_audio(fnf_audio* audio);
fnf_audio* set_audio_volume(fnf_audio* audio, float vol);
bool delete_audio(fnf_audio* audio);
float get_audio_position(fnf_audio* audio);

#endif