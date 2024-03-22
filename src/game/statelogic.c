#include "states.h"
#include "types.h"

fnf_state* nextState = NULL;
fnf_state* curState = NULL;

fnf_sprite* sprites[MAX_SPRITES];
uint16 sprites_size = 0;

bool add_sprite(fnf_sprite* sprite){
    if(sprites_size == MAX_SPRITES)
        return false;
    sprites[sprites_size++] = sprite;
    return true;
}

bool switch_state(fnf_state* state){
    nextState = state;
    return true;
}

bool _switchState(){
    if(curState != NULL)
        curState->uninit();

    FNF_ZERO(sprites, sizeof(sprites));

    curState = nextState;
    curState->create();
    nextState = NULL;
    return true;
}

void draw_all_sprites(){
    for(int i=0; i<sprites_size; i++){
        fnf_sprite* sprite = sprites[i];
        if(!sprite)
            continue;
        draw_sprite(sprite);
    }
}

bool free_sprites(){
    for(int i=0; i<sprites_size; i++){
        if(!sprites[i]) continue;
        FNF_DELETE(sprites[i]);
    }
    return true;
}

bool kill_sprites(){
    memset(sprites, 0, sprites_size * sizeof(fnf_sprite));
    sprites_size = 0;
    return true;
}

fnf_state* get_nextState(){
    return nextState;
}
fnf_state* get_curState(){
    return curState;
}
fnf_sprite** get_sprites(){
    return sprites;
}
uint16* get_sprites_size(){
    return &sprites_size;
}