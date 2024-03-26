#ifndef SPRITEUTIL_H
#define SPRITEUTIL_H
#include "sprite.h"

static char* directions[] = {
    "static left",
    "static down",
    "static up",
    "static right"
};

static const char* directions_pressed[] = {
    "pressed left",
    "pressed down",
    "pressed up",
    "pressed right"
};

static const char* directions_confirm[] = {
    "confirm left",
    "confirm down",
    "confirm up",
    "confirm right"
};

static const char* directions_color[] = {
    "purpleScroll",
    "blueScroll",
    "greenScroll",
    "redScroll"
};

static const char* directions_sus[] = {
    "purpleHold",
    "blueHold",
    "greenHold",
    "redHold"
};

static const char* directions_sus_end[] = {
    "purpleEnd",
    "blueEnd",
    "greenEnd",
    "redEnd"
};

fnf_sprite* create_character(float x, float y, const char* name);
fnf_sprite* create_icon(float x, float y, const char* name);
fnf_sprite* create_arrows();
void create_stage(const char* name, fnf_camera* camera);

#endif // SPRITEUTIL_H