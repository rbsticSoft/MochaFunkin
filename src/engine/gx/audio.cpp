extern "C" {
#include "audio.h"
}

#include <tremor/ivorbiscodec.h>
#include <tremor/ivorbisfile.h>
#include <malloc.h>

#include <gccore.h>
#include <aesndlib.h>

#include "audiogc.hpp"

fnf_audio* select_menu = NULL;
fnf_audio* back_menu = NULL;
fnf_audio* accept_menu = NULL;

void init_audio(){
    AESND_Init();
/*
    select_menu = FNF_NEW(fnf_audio);
    back_menu = FNF_NEW(fnf_audio);
    accept_menu = FNF_NEW(fnf_audio);

    *select_menu = make_audio();
    *back_menu = make_audio();
    *accept_menu = make_audio();

    load_audio(back_menu, "assets/sounds/cancelMenu.ogg");
    load_audio(accept_menu, "assets/sounds/confirmMenu.ogg");
    load_audio(select_menu, "assets/sounds/scrollMenu.ogg");
    */
}

void uninit_audio(){
    //AESND_End();
}

fnf_audio make_audio(){
    fnf_audio tmp;
    //tmp.source = ASND_GetFirstUnusedVoice();

    return tmp;
}

fnf_audio* load_audio(fnf_audio* audio, const char* path){
    audiogc::player* player = new audiogc::player(audiogc::type::vorbis, path, audiogc::mode::stream);
    audio->player = player;
    return audio;
}

fnf_audio* play_audio(fnf_audio* audio){
    if(!audio) return NULL;
    audiogc::player* player = static_cast<audiogc::player*>(audio->player);
    player->play();
    //alSourcePlay(audio->source);
    return audio;
}

fnf_audio* pause_audio(fnf_audio* audio){
    if(!audio) return NULL;
    audiogc::player* player = static_cast<audiogc::player*>(audio->player);

    //alSourcePause(audio->source);
    return audio;
}

fnf_audio* stop_audio(fnf_audio* audio){
    if(!audio) return NULL;
    audiogc::player* player = static_cast<audiogc::player*>(audio->player);

    //alSourceStop(audio->source);
    return audio;
}

bool stopped_audio(fnf_audio* audio){
    int state = false;
    //alGetSourcei(audio->source, AL_SOURCE_STATE, &state);
    return state;
}

fnf_audio* set_audio_volume(fnf_audio* audio, float vol){
    //alSourcef(audio->source, AL_GAIN, vol);
    return audio;
}

bool delete_audio(fnf_audio* audio){
    audiogc::player* player = static_cast<audiogc::player*>(audio->player);
    delete player;
    //alDeleteBuffers(1, &audio->buffer);
    //alDeleteSources(1, &audio->source);
    return true;
}

float get_audio_position(fnf_audio* audio){
    if(!audio) return 0.0f;
    float pos = 0;
    //alGetSourcef(audio->source, AL_SEC_OFFSET, &pos);
    return pos * 1000;
}