#include "states.h"
#include "sprite.h"
#include "animation.h"
#include "controls.h"
#include "audio.h"

const char* options[] = {
    "story mode %s",
    "freeplay %s "
};

int cur_selection = 0;

fnf_sprite* menu_options[2];

void selection(int what);

void create_main_menu_state(){    
    fnf_sprite* bg = make_sprite(0, 0, false);
    bg->scroll.y = 0.18f;
    load_sprite(bg, "assets/images/menuBG.png");
    add_sprite(bg);


	fnf_sprite* base_item = make_sprite(0, 0, true);
    animation_load_atlas(&base_item->animation, "assets/images/main_menu.xml");
    load_sprite(base_item, "assets/images/main_menu.png");

    for (int i=0; i<2; i++){
        fnf_sprite* item = clone_sprite(base_item);
        move_sprite(item, 0, 60 + (i * 160));
        char buf[32];
        sprintf(buf, options[i], "idle");
        animation_add_prefix(&item->animation, buf, buf, true, 24);
        sprintf(buf, options[i], "selected");
        animation_add_prefix(&item->animation, buf, buf, true, 24);

        animation_play(&item->animation, "idle");
        menu_options[i] = item;

        add_sprite(item);
    }

    selection(1);
    selection(1);
}

void selection(int what){
    play_audio(select_menu);
    char buf[32];
    sprintf(buf, options[cur_selection], "idle");
    animation_play(&menu_options[cur_selection]->animation, buf);
    cur_selection += what;
    if(cur_selection > 1)
        cur_selection = 0;
    if(cur_selection < 0)
        cur_selection = 1;
    sprintf(buf, options[cur_selection], "selected");
    animation_play(&menu_options[cur_selection]->animation, buf);
}

void draw_main_menu_state(){
    if(key_just_pressed(UP))
        selection(-1);
    if(key_just_pressed(DOWN))
        selection(1);
    if(key_just_pressed(ENTER)) {
        if(cur_selection == 0)
            switch_state(&story_menu_state);
        else
            switch_state(&freeplay_state);
    }

    for (int i=0; i<2; i++)
        center_sprite(menu_options[i], X);
    draw_all_sprites();
}

void uninit_main_menu_state(){
    free_sprites();
    kill_sprites();
}
