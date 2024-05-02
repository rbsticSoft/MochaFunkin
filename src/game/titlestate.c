#include "states.h"
#include "conductor.h"
#include "audio.h"
#include "controls.h"

#include <stdio.h>
#include <math.h>

fnf_audio audio;
fnf_sprite* spr;

void create_title_state(){
    audio = make_audio();
    load_audio(&audio, "assets/music/freakyMenu.ogg");

    fnf_sprite* logo = make_sprite(-150, -100, true);
    animation_load_atlas(&logo->animation, "assets/images/logoBumpin.xml");
    animation_add_prefix(&logo->animation, "bump", "logo bumpin", true, 24);
    animation_play(&logo->animation, "bump");
    load_sprite(logo, "assets/images/logoBumpin.png");

    spr = make_sprite(SCREEN_WIDTHF*0.4f, SCREEN_HEIGHTF*0.07f, true);
    animation_load_atlas(&spr->animation, "assets/images/gfDanceTitle.xml");

    int32 lind[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    int32 rind[] = {15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29};

    animation_add_prefix_indices(&spr->animation, "gf dance l", "gfDance", false, 24, lind, 15);
    animation_add_prefix_indices(&spr->animation, "gf dance r", "gfDance", false, 24, rind, 15);
    animation_play(&spr->animation, "gf dance l");

    load_sprite(spr, "assets/images/gfDanceTitle.png");

    fnf_sprite* titleEnter = make_sprite(100.f, SCREEN_HEIGHTF*0.8f, true);
    animation_load_atlas(&titleEnter->animation, "assets/images/titleEnter.xml");
    animation_add_prefix(&titleEnter->animation, "idle", "Press Enter to Begin", true, 24);
    animation_play(&titleEnter->animation, "idle");
    load_sprite(titleEnter, "assets/images/titleEnter.png");

    add_sprite(logo);
    add_sprite(spr);
    add_sprite(titleEnter);

    change_bpm(current_conductor, 102);
    
    play_audio(&audio);
}

void draw_title_state(){
    static int32 oldStep = 0;
	static int32 curStep = 0;
    static int32 danceLeft = 0;

    current_conductor->songPosition = roundf(get_audio_position(&audio));

    oldStep = curStep;

    if(key_just_pressed(ENTER))
        switch_state(&main_menu_state);
    if(key_just_pressed(LEFT))
        animation_play(&spr->animation, "gf dance l");
    if(key_just_pressed(RIGHT))
        animation_play(&spr->animation, "gf dance r");

    curStep = (int32)floorf(current_conductor->songPosition / current_conductor->stepCrochet);
    draw_all_sprites();
    if(curStep % 4 == 0 && oldStep != curStep){
        danceLeft = !danceLeft;
        if(danceLeft)
            animation_play(&spr->animation, "gf dance l");
        else
            animation_play(&spr->animation, "gf dance r");
        
    }
}

void uninit_title_state(){
    for(int i=0; i < *get_sprites_size(); i++)
        delete_sprite(get_sprites()[i]);
    kill_sprites();
    delete_audio(&audio);
}
