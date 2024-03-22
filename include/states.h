#ifndef STATES_H
#define STATES_H
#include "types.h"
#include "sprite.h"

#define FNF_CREATE_STATE(state) void create_##state(); \
void draw_##state (); \
void uninit_##state (); \
static fnf_state state = { \
    .create = create_##state , \
    .draw = draw_##state , \
    .uninit = uninit_##state \
}

typedef struct {
    void(*create)();
    void(*draw)();
    void(*uninit)();
} fnf_state;

FNF_CREATE_STATE(title_state);
FNF_CREATE_STATE(play_state);
//FNF_CREATE_STATE(story_menu_state);
//FNF_CREATE_STATE(freeplay_state);
//FNF_CREATE_STATE(main_menu_state);

bool add_sprite(fnf_sprite* sprite);
bool switch_state(fnf_state* state);
bool free_sprites();
/**
 * Sets everything in sprites[] to null.
 * @note THIS DOES NOT DELETE THE SPRITES. IT ONLY SETS EVERYTHING IN sprites[] TO NULL.
*/
bool kill_sprites();

fnf_state* get_nextState();
fnf_state* get_curState();
fnf_sprite** get_sprites();
uint16* get_sprites_size();

void draw_all_sprites();
bool _switchState();

#endif // STATES_H