#include "states.h"
#include "math_fnf.h"
#include "controls.h"
#include "audio.h"
#include "font.h"
#include "fnf_data.h"
#include <stdio.h>

fnf_sprite* weeks[8];
fnf_sprite* base_week;

int menu_selection = 0;

const char* characters[] = {
        NULL,
        "Dad idle",
        "spooky",
        "Pico Idle",
        "Mom",
        "Parent",
        "SENPAI",
        "Tankman"
};

void select_week(int what);
void create_story_menu_state(){
    fnf_sprite* yellow = make_sprite(0, 56, false);
    create_shape(yellow);
    resize_sprite(yellow, SCREEN_WIDTHF, 400);
    set_color(yellow, 0x51CFF9);

    base_week = make_sprite((SCREEN_WIDTHF * 0.25f) - 150, 70, true);

    load_sprite(base_week, "assets/images/campaign_menu_UI_characters.png");
    animation_load_atlas(&base_week->animation, "assets/images/campaign_menu_UI_characters.xml");
    animation_add_prefix(&base_week->animation, "BF idle", "BF idle", true, 24);
    animation_add_prefix(&base_week->animation, "GF idle", "GF Dancing", true, 24);
    //animation_add_prefix(&base_week->animation, characters[i], characters[i], true, 24);

    for(int i=0; i<8; i++){
        char* weekpath[32];
        sprintf(weekpath, "assets/images/storymenu/week%i.png", i);

        fnf_sprite* week = make_sprite(0, 456, true);

        load_sprite(week, weekpath);
        move_sprite(week, week->x, (week->h + 20) * i);
        add_sprite(week);
        center_sprite(week, X);

        weeks[i] = week;
        if(!characters[i]) continue;
        animation_add_prefix(&base_week->animation, characters[i], characters[i], true, 24);
    }
    add_sprite(yellow);

    scale_sprite(base_week, 0.5f, 0.5f);
    add_sprite(base_week);

    fnf_sprite* bf = clone_sprite(base_week);
    move_sprite(bf, ((SCREEN_WIDTHF * 0.25f) * (2) - 150) - 80, 70);
    scale_sprite(bf, 0.9f, 0.9f);
    animation_play(&bf->animation, "BF idle");
    add_sprite(bf);

    fnf_sprite* gf = clone_sprite(base_week);
    move_sprite(gf, ((SCREEN_WIDTHF * 0.25f) * (3) - 150), 70);
    scale_sprite(gf, 0.5f, 0.5f);
    animation_play(&gf->animation, "GF idle");
    add_sprite(gf);
}

void draw_story_menu_state(){
    if(key_just_pressed(BACK))
        switch_state(&main_menu_state);
    if(key_just_pressed(ENTER)){
        load_song(NULL, week_songs[menu_selection]);
        switch_state(&play_state);
    }
    if(key_just_pressed(UP))
        select_week(-1);
    if(key_just_pressed(DOWN))
        select_week(1);
    for(int i=0; i<7; i++){
        fnf_sprite* week = weeks[i];
        move_sprite(week, week->x, lerp(week->y, ((i - menu_selection) * 120) + 480, 0.17f));
    }
    draw_all_sprites();

    static const char* trackbase = "Tracks\n%s\n%s\n%s";
    static char trackbuffer[255];
    sprintf(trackbuffer, trackbase, week_songs[menu_selection][0], week_songs[menu_selection][1], week_songs[menu_selection][2]);

    draw_text(trackbuffer, (SCREEN_WIDTHF * 0.5f) - SCREEN_WIDTHF * 0.35f, 500, 1.0f);
}

void select_week(int what){
    menu_selection += what;
    play_audio(select_menu);
    if(menu_selection > 7)
        menu_selection = 0;
    if(menu_selection < 0)
        menu_selection = 7;
    if(!characters[menu_selection]) {
        base_week->enabled = false;
        return;
    }
    base_week->enabled = true;
    switch (menu_selection)
    {
        case 1:
            scale_sprite(base_week, 0.5f, 0.5f);
            base_week->offset = (fnf_vector){0, 0};
            set_flip(base_week, NONE);
        case 3:
            scale_sprite(base_week, 1.0f, 1.0f);
            base_week->offset = (fnf_vector){-40, 170};
            set_flip(base_week, X);
        case 5:
            scale_sprite(base_week, 0.9f, 0.9f);
            base_week->offset = (fnf_vector){-600, 0};
            set_flip(base_week, NONE);
        default:
            scale_sprite(base_week, 0.5f, 0.5f);
            base_week->offset = (fnf_vector){0, 0};
            set_flip(base_week, NONE);
    }
    animation_play(&base_week->animation, characters[menu_selection]);
}

void uninit_story_menu_state(){
    free_sprites();
    kill_sprites();
}
