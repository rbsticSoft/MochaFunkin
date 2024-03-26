#include "audio.h"
#include <vorbis/vorbisfile.h>
#include <malloc.h>

ALCdevice* device;
ALCcontext* context;

void init_audio(){
    device = alcOpenDevice(NULL);
    context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
}

void uninit_audio(){
    alcDestroyContext(context);
    alcCloseDevice(device);
}

fnf_audio make_audio(){
    fnf_audio tmp;

    alGenSources(1, &tmp.source);
    alGenBuffers(1, &tmp.buffer);

    return tmp;
}

fnf_audio* load_audio(fnf_audio* audio, const char* path){
    OggVorbis_File vf;
    int decodedSize=0;
    int current_section;

    if(ov_open(fopen(path, "rb"), &vf, NULL, 0) < 0) {
        fprintf(stderr,"Input does not appear to be an Ogg bitstream.\n");
        return NULL;
    }

    vorbis_info *vi=ov_info(&vf,-1);
    double duration = ov_time_total(&vf, -1);
    long oggsize = duration * vi->rate * vi->channels * 3;

    char* audioBuffer = malloc(oggsize);

    char buffer[4096];

    while(1){
        long read = ov_read(&vf, buffer, 4096, 0, 2, 1, &current_section);
        if(read <= 0)
            break;

        memcpy(audioBuffer + (decodedSize), buffer, read);
        decodedSize += read;
    }
    char* alBuffer = malloc(decodedSize);
    memcpy(alBuffer, audioBuffer, decodedSize);
    FNF_DELETE(audioBuffer);

    alBufferData(audio->buffer, AL_FORMAT_STEREO16, alBuffer, decodedSize, vi->rate);
    alSourcei(audio->source, AL_BUFFER, audio->buffer);
    
    FNF_DELETE(alBuffer);
    return audio;
}

fnf_audio* play_audio(fnf_audio* audio){
    if(!audio) return NULL;

    alSourcePlay(audio->source);
    return audio;
}

fnf_audio* set_audio_volume(fnf_audio* audio, float vol){
    alSourcef(audio->source, AL_GAIN, vol);
    return audio;
}

bool delete_audio(fnf_audio* audio){
    alDeleteBuffers(1, &audio->buffer);
    alDeleteSources(1, &audio->source);
    return true;
}

float get_audio_position(fnf_audio* audio){
    if(!audio) return 0.0f;
    float pos = 0;
    alGetSourcef(audio->source, AL_SEC_OFFSET, &pos);
    return pos * 1000;
}