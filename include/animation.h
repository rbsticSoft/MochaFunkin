#ifndef ANIMATION_H
#define ANIMATION_H
#include "types.h"

typedef struct {
        int32 x,y, fx, fy;
        uint16 w,h, fw, fh;
} fnf_frame;

typedef struct {
    char name[64];
    fnf_frame frames[255];
    uint16 framesSize;
} fnf_animation;

typedef struct {
        char name[64];
        uint16 fps;
        bool looped;
        fnf_animation* animation;
        int16 playlistSize;
        int32 playlist[255];
} fnf_animation_prefix;

typedef struct {
    fnf_animation animations[512];
    fnf_animation_prefix prefixes[64];
    uint16 animationsSize, prefixesSize;
} fnf_animation_collection;

typedef struct {
    fnf_animation_collection* collection;
    fnf_animation_prefix* currentAnimation;
    uint16 frameNum;
    uint32 lastFrame;
    uint32 accum;
    bool finished;
} fnf_animation_controller;


bool animation_load_atlas(fnf_animation_controller* controller, const char* path);
bool animation_add_prefix(fnf_animation_controller* controller, const char* prefix, const char* anim, bool looped, int32 fps);
bool animation_add_prefix_indices(fnf_animation_controller* controller, const char* prefix, const char* anim, bool looped, int32 fps, int32* indices, uint8 indices_size);
bool animation_play(fnf_animation_controller* controller, const char* prefix);
#endif // ANIMATION_H