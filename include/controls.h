#ifndef CONTROLS_H
#define CONTROLS_H

typedef enum {
    UP = 2,
    DOWN = 1,
    LEFT = 0,
    RIGHT = 3,
    BACK = 5,
    ENTER = 6
} fnf_key_t;

bool key_pressed(fnf_key_t key);
bool key_just_pressed(fnf_key_t key);

#endif // CONTROLS_H