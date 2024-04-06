#include "states.h"
#include "font.h"
#include "math_fnf.h"
#include "controls.h"
#include "audio.h"

#define rarray_size(x) sizeof(x) / sizeof(x[0])

char* songs[] = {
    "tutorial",
    "bopeebo",
    "fresh",
    "dadbattle",
    "spookeez",
    "south",
    "monster",
    "pico",
    "philly",
    "blammed",
    "satin-panties",
    "high",
    "milf",
    "cocoa",
    "eggnog",
    "winter-horrorland",
    "senpai",
    "roses",
    "thorns",
    "ugh",
    "guns",
    "stress"
};

int songs_size = rarray_size(songs);

float xs[rarray_size(songs)];
float ys[rarray_size(songs)];

int current_selection = 0;

void select_song(int what);

void create_freeplay_state(){

    fnf_sprite* bg = make_sprite(0, 0, false);
    load_sprite(bg, "assets/images/menuDesat.png");
    add_sprite(bg);

    for(int i=0; i<songs_size; i++){
        xs[i] = 0;
        ys[i] = (70 * i) + 30;
    }
}

void draw_freeplay_state(){
    if(key_just_pressed(UP))
        select_song(-1);
    if(key_just_pressed(DOWN))
        select_song(1);
    if(key_just_pressed(ENTER)){
        load_song_name = songs[current_selection];
        switch_state(&play_state);
    }


    draw_all_sprites();
    for(int i=0; i<songs_size; i++){
        int target = i - current_selection;
        float scaledY = remap(target - 2, 0, 1, 0, 1.3f);
        ys[i] = lerp(ys[i], (scaledY * 120) + (SCREEN_WIDTHF * 0.48), 0.16);
		xs[i] = lerp(xs[i], (target * 20) + 90, 0.16);
        draw_alphabet(songs[i], xs[i], ys[i], current_selection == i ? 1.0f : 0.6f);
    }
}

void select_song(int what){
    current_selection += what;
    if(current_selection >= songs_size)
        current_selection = 0;
    if(current_selection < 0)
        current_selection = songs_size - 1;
    play_audio(select_menu);
}

void uninit_freeplay_state(){
}
