#include "states.h"
#include "controls.h"
#include "parson.h"
#include "note.h"
#include "conductor.h"
#include "audio.h"
#include "font.h"
#include "spriteutil.h"
#include "engine.h"
#include "math_fnf.h"
#include "stage.h"
#include "saves.h"
#include "timer.h"

#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define SWAG_WIDTH (160.f*0.7f)

JSON_Value* chart;

fnf_sprite* enemyStrums[4];
fnf_sprite* playerStrums[4];
fnf_sprite* arrow_base;
fnf_sprite* health_bar_bg, *health_bar;

fnf_note* note_sprites = NULL;
uint32_t note_size = 0;

fnf_audio inst;
fnf_audio voices;
fnf_audio dead;

fnf_song song;
fnf_stage_t stage;

bool hit_sections[256];
bool bf_dead = false;
bool paused = false;
bool music_started = false;

char load_song_name[64];
const char* difficulty = "hard";
char score_txt[32] = "Score: 0";

int score = 0;
float health = 1.0f;

bool bring_to_front(fnf_sprite** index, fnf_sprite** end);

int count_notes();
void song_info();
void generate_song(const char* path);
void generate_static_notes();
void add_health(float hp);
void keyShit();
void end_song();

fnf_note* generate_note(float strum_time, int note_type, int must_hit, bool sus_time);

fnf_camera* play_camera;
fnf_camera* hud_camera;

fnf_vector cam_follow;

fnf_sprite* dad, *gf, *boyfriend;
fnf_sprite* dad_icon, *bf_icon;
fnf_sprite* limo_overlay;
fnf_sprite* countdown_sprites[4];
fnf_audio countdown_audio[4];
fnf_timer countdown_timer, rsg_timer;

int countdown;
float bf_timer, dad_timer;

char** current_week;
int current_week_song;

void tolowersptr(char* ptr) {
    char *lptr = ptr;
    while (*lptr) {
        intptr_t loc = lptr++ - ptr;
        ptr[loc] = tolower(ptr[loc]);
    }
}

void load_song(char* name, char** week){
    current_week = week;
    if(!name){
        current_week_song = 0;
        strcpy(load_song_name, week[current_week_song]);
        tolowersptr(load_song_name);
        return;
    }

    strcpy(load_song_name, name);
    tolowersptr(load_song_name);
};

void create_play_state(){
    health = 1.0f;
    score = 0.0f;
    note_size = 0;
    countdown = 0;
    bf_dead = false;
    paused = false;
    music_started = false;

    char path[255];
    sprintf(path, "assets/data/%s/%s-%s.json", load_song_name, load_song_name, difficulty);
    chart = json_parse_file(path);
    song_info();

    play_camera = make_camera();
    hud_camera = make_camera();

    play_camera->zoom = 0.9f;

    create_stage(stage, play_camera);

    switch(stage){
        case LIMO: {
            limo_overlay = make_sprite(-120, 550, true);
            load_sprite(limo_overlay, "assets/week4/images/limo/limoDrive.png");
            animation_load_atlas(&limo_overlay->animation, "assets/week4/images/limo/limoDrive.xml");

            animation_add_prefix(&limo_overlay->animation, "drive", "Limo stage", true, 24);
            animation_play(&limo_overlay->animation, "drive");
            limo_overlay->camera = play_camera;
            break;
        }
        default: break;
    }

    gf = make_sprite(400, 130, true);
    gf->scroll = (fnf_vector){0.95f, 0.95f};
    gf->camera = play_camera;

    load_sprite(gf, "assets/shared/images/characters/GF_assets.png");
    animation_load_atlas(&gf->animation, "assets/shared/images/characters/GF_assets.xml");

    int32 lind[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    int32 rind[] = {15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29};

    animation_add_prefix_indices(&gf->animation, "danceLeft", "GF Dancing Beat", false, 24, lind, 15);
    animation_add_prefix_indices(&gf->animation, "danceRight", "GF Dancing Beat", false, 24, rind, 15);

    animation_play(&gf->animation, "danceLeft");
    add_sprite(gf);

    if(stage == LIMO)
        add_sprite(limo_overlay);

    boyfriend = create_character(770, 450, song.bf);
    boyfriend->camera = play_camera;
    add_sprite(boyfriend);

    dad = create_character(100, 100, song.dad);
    dad->camera = play_camera;
    add_sprite(dad);

    switch(stage){
        case LIMO: {
            move_sprite(boyfriend, boyfriend->x + 260, boyfriend->y - 220);
            break;
        }
        default: break;
    }

    if(!arrow_base)
        arrow_base = create_arrows();

    generate_static_notes();
    
    generate_song(path);


    //fnf_sprite** sprarray = &get_sprites()[7];
    //uint16 sprarray_size = *get_sprites_size();
    //bring_to_front(sprarray, &sprarray[sprarray_size]);
    //printf("Counted notes = %i\n", sections_count);
	health_bar_bg = make_sprite(0, SCREEN_HEIGHTF * 0.9f, false);
    load_sprite(health_bar_bg, "assets/shared/images/healthBar.png");
	center_sprite(health_bar_bg, X);
	add_sprite(health_bar_bg);

	fnf_sprite* health_barbg = make_sprite(health_bar_bg->x + 4, health_bar_bg->y + 4, false);
    create_shape(health_barbg);
    resize_sprite(health_barbg, health_bar_bg->w - 8, health_bar_bg->h - 8);
    set_color(health_barbg, 0xFF33FF66);
	//healthBar.createFilledBar(, 0xFF66FF33);
	add_sprite(health_barbg);

	health_bar = make_sprite(health_bar_bg->x + 4, health_bar_bg->y + 4, false);
    create_shape(health_bar);
    resize_sprite(health_bar, (health_bar_bg->w - 8), health_bar_bg->h - 8);
    set_color(health_bar, 0xFF0000FF);
	//healthBar.createFilledBar(, 0xFF66FF33);
	add_sprite(health_bar);

    dad_icon = create_icon(20, health_bar->y - 75, song.dad);
    add_sprite(dad_icon);

    bf_icon = create_icon(20, health_bar->y - 75, song.bf);
    set_flip(bf_icon, X);
    add_sprite(bf_icon);

    add_health(0.0f);

    change_bpm(current_conductor, song.bpm);
    
    inst = make_audio();
    voices = make_audio();
    dead = make_audio();

    sprintf(path, "assets/songs/%s/Inst.ogg", load_song_name);
    load_audio(&inst, path);
    sprintf(path, "assets/songs/%s/Voices.ogg", load_song_name);
    load_audio(&voices, path);

    load_audio(&dead, "assets/shared/sounds/fnf_loss_sfx.ogg");

    //play_audio(&inst);
    //play_audio(&voices);
    current_conductor->songPosition = -current_conductor->crochet * 5;

    static const char* rsg_paths[] = {
            "", "assets/shared/images/ready.png", "assets/shared/images/set.png", "assets/shared/images/go.png"
    };
    static const char* rsg_audio_paths[] = {
            "assets/shared/sounds/intro3.ogg", "assets/shared/sounds/intro2.ogg", "assets/shared/sounds/intro1.ogg", "assets/shared/sounds/introGo.ogg"
    };

    countdown_sprites[0] = make_sprite(0, 0, false);
    for(int i=1;i<4; i++){
        countdown_sprites[i] = make_sprite(0, 0, false);
        load_sprite(countdown_sprites[i], rsg_paths[i]);
        center_sprite(countdown_sprites[i], XY);
        countdown_sprites[i]->enabled = false;
        add_sprite(countdown_sprites[i]);
    }

    for(int i=0;i<4; i++){
        countdown_audio[i] = make_audio();
        load_audio(&countdown_audio[i], rsg_audio_paths[i]);
    }

    countdown_timer = timer_start(current_conductor->crochet);
    //add_sprite(arrow_base);
}

bool update_note(fnf_note* da_note){
    if (da_note->must_press)
	{
			// miss on the NEXT frame so lag doesnt make u miss notes
		if (da_note->will_miss && !da_note->was_good_hit)
		{
			da_note->too_late = true;
			da_note->can_be_hit = false;
		}
		else
		{
			if (da_note->strum_time > current_conductor->songPosition - current_conductor->safeZoneOffset)
			{ // The * 0.5 is so that it's easier to hit them too late, instead of too early
				if (da_note->strum_time < current_conductor->songPosition + (current_conductor->safeZoneOffset * 0.5))
					da_note->can_be_hit = true;
			}
			else
			{
				da_note->can_be_hit = true;
				da_note->will_miss = true;
			}
		}
	}
	else
	{
		da_note->can_be_hit = false;
		if (da_note->strum_time <= current_conductor->songPosition)
			da_note->was_good_hit = true;
	}
    if(da_note->too_late)
    {
        //da_note->sprite->enabled = false;
    }
    return true;
}

void beat_hit(int32 beat);
void step_hit(int32 step);

void update_bf(){
    float elapsed = get_delta() * 0.001;

    if (boyfriend->animation.currentAnimation->name[0] == 's')
		bf_timer += elapsed;
	else
		bf_timer = 0;

	if (dad->animation.currentAnimation->name[0] == 's')
		dad_timer += elapsed;
	if (dad_timer >= current_conductor->stepCrochet * 6.1f * 0.001){
        animation_play(&dad->animation, "idle");
		dad_timer = 0;
	}
        
	//if (animation.curAnim.name.endsWith('miss') && animation.curAnim.finished)
	//	playAnim('idle', true, false, 10);

	//if (animation.curAnim.name == 'firstDeath' && animation.curAnim.finished)
	//	playAnim('deathLoop');
    //printf("%f\n", elapsed);

}

const char* paused_items[] = {
        "Resume",
        "Restart Song",
        "Toggle Practice Mode",
        "Exit to menu"
};

int paused_items_size = rarray_size(paused_items);
int paused_selection = 0;

float px[rarray_size(paused_items)];
float py[rarray_size(paused_items)];
fnf_sprite* black = 0;

void init_pause(){
    if(!black) {
        black = make_sprite(0, 0, false);
        add_sprite(black);
    }
    create_shape(black);
    resize_sprite(black, SCREEN_WIDTHF, SCREEN_HEIGHTF);
    set_alpha(black, 0.5f);

    for(int i=0; i<paused_items_size; i++){
        px[i] = 0;
        py[i] = (70 * i) + 30;
    }
}

void unpause(){
    paused = false;

    play_audio(&inst);
    play_audio(&voices);
    set_alpha(black, 0.0f);
}

void select_pause(int what){
    paused_selection += what;
    if(paused_selection >= paused_items_size)
        paused_selection = 0;
    if(paused_selection < 0)
        paused_selection = paused_items_size - 1;
    play_audio(select_menu);
}

void pause_action(){
    switch(paused_selection){
        case 0:
            unpause();
            break;
        case 1:
            switch_state(&play_state);
            break;
        case 3:
            paused = false;
            switch_state(&freeplay_state);
            break;
    }
}

void draw_play_state(){
    if(countdown_timer.completed && countdown != 5){
        if(countdown == 4) {
            countdown++;
        }
        if(countdown!= 5) {
            if (countdown)
                countdown_sprites[countdown - 1]->enabled = false;
            countdown_timer = timer_start(current_conductor->crochet);
            countdown_sprites[countdown]->enabled = true;
            rsg_timer = timer_start(current_conductor->crochet);
            play_audio(&countdown_audio[countdown]);
            countdown++;
        }
    }
    timer_step(&countdown_timer);

    if(countdown == 5 && !music_started){
        play_audio(&inst);
        play_audio(&voices);
        countdown_sprites[3]->enabled = false;
        music_started = true;
    }

    bool enter_pressed = key_just_pressed(ENTER);
    if(enter_pressed && !paused && countdown == 5) {
        paused = true;
        pause_audio(&inst);
        pause_audio(&voices);
        init_pause();
        enter_pressed = false;
    }
    if(enter_pressed && paused) {
        pause_action();
    }

    if(stopped_audio(&inst))
        end_song();

    static int32 oldStep = 0;
	static int32 curStep = 0;

    current_conductor->songPosition = countdown == 5 ? roundf(get_audio_position(&inst)) + current_conductor->offset : current_conductor->songPosition + 16;//get_delta();//
    oldStep = curStep;
    curStep = (int32)floorf(current_conductor->songPosition / current_conductor->stepCrochet);

    if(!bf_dead && !paused) {
        update_bf();
        keyShit();
    }
    else{
        cam_follow.x = (boyfriend->x + boyfriend->graphic.w * 0.5f) - 100;
        cam_follow.y = (boyfriend->y + boyfriend->graphic.h * 0.5f) - 100;
    }

    for(int i=0; i < note_size; i++){
        if(paused)break;
        fnf_note* da_note = &note_sprites[i];
        update_note(&note_sprites[i]);
        //if(!da_note->sprite->enabled) continue; //this line causes lag????
        int strmid = 50 + SWAG_WIDTH / 2;
        float ypos = (50 - (current_conductor->songPosition - note_sprites[i].strum_time) * (0.45 * song.speed));

        //if(da_note->sustain)
            //ypos += da_note->sprite->graphic.h / 2.0f;

        if(da_note->was_good_hit && !da_note->must_press && da_note->sprite->enabled){
            da_note->sprite->enabled = false;
            
            set_audio_volume(&voices, 1.0f);
            switch(da_note->note_data % 4) {
                case 0:
                    animation_play(&dad->animation, "singLEFT");
                    break;
                case 1:
                    animation_play(&dad->animation, "singDOWN");
                    break;
                case 2:
                    animation_play(&dad->animation, "singUP");
                    break;
                case 3:
                    animation_play(&dad->animation, "singRIGHT");
                    break;
            }
            if(da_note->alt)
                    switch(da_note->note_data % 4){
                        case 0:
                            animation_play(&dad->animation, "singLEFT-alt"); break;
                        case 1:
                            animation_play(&dad->animation, "singDOWN-alt"); break;
                        case 2:
                            animation_play(&dad->animation, "singUP-alt"); break;
                        case 3:
                            animation_play(&dad->animation, "singRIGHT-alt"); break;
                    }

            dad_timer = 0;
        }

        /*
        if (da_note->sustain
			&& (!da_note->must_press || (da_note->was_good_hit || (!da_note->can_be_hit)))
			&& da_note->sprite->y + da_note->sprite->offset.y * da_note->sprite->scale.y <= strmid){
			fnf_rect swag_rect = {0, 0, da_note->sprite->w / da_note->sprite->scale.x, da_note->sprite->graphic.h / da_note->sprite->scale.y};

			swag_rect.y = (strmid - da_note->sprite->y) / da_note->sprite->scale.y;
			swag_rect.h -= swag_rect.y;
			da_note->sprite->clip = swag_rect;
		}
        */
        move_sprite(note_sprites[i].sprite, note_sprites[i].sprite->x, ypos);

        if (da_note->too_late || da_note->was_good_hit)
		{
			if (da_note->too_late && da_note->sprite->enabled)
			{
				add_health(-0.0475);
                set_audio_volume(&voices, 0.0f);
			}

            da_note->sprite->enabled = false;
		}
    }


    //cam_follow.x = 0;
    //cam_follow.y = 0;
    //cam_follow.x -= (SCREEN_WIDTHF * 0.5f);
    //cam_follow.y -= (SCREEN_HEIGHTF * 0.5f);
    play_camera->x = lerp(play_camera->x, cam_follow.x, 0.04);
    play_camera->y = lerp(play_camera->y, cam_follow.y, 0.04);

    if(curStep >= 0 && oldStep != curStep)
        step_hit(curStep);

    //timer_step(&rsg_timer );
    //float eased_time = cubeInOut(rsg_timer.elapsed / rsg_timer.target);
    //set_alpha(countdown_sprites[countdown % 4], (1.0f - eased_time));

    draw_all_sprites();
    if(paused){
        if(key_just_pressed(UP))
            select_pause(-1);
        if(key_just_pressed(DOWN))
            select_pause(1);
        for(int i=0; i<paused_items_size; i++){
            int target = i - paused_selection;
            float scaledY = remap(target - 2, 0, 1, 0, 1.3f);
            py[i] = lerp(py[i], (scaledY * 120) + (SCREEN_WIDTHF * 0.48), 0.16);
            px[i] = lerp(px[i], (target * 20) + 90, 0.16);
            draw_alphabet(paused_items[i], px[i], py[i], paused_selection == i ? 1.0f : 0.6f);
        }
    }
    if(!bf_dead)
        draw_text(score_txt, health_bar_bg->x + health_bar_bg->w - 190.f, health_bar_bg->y + 30.f, 1.f);
    if(key_just_pressed(ENTER) && bf_dead)
        switch_state(&play_state);
    if(key_just_pressed(BACK) && bf_dead)
        switch_state(&freeplay_state);
}

void step_hit(int32 step){
    static int32 beat = 0;
    beat = step % 4;
    if(beat == 0)
        beat_hit(floor(step/4));

    int offsetx = 0;
    int offsety = 0;

    if(hit_sections[step/16]){
        switch(stage){
            case LIMO: {
                offsetx = -300;
                break;
            }
            case MALL: {
                offsety = -200;
                break;
            }
            default: break;
        }
        cam_follow.x = (boyfriend->x + boyfriend->graphic.w * 0.5f) - 100;
        cam_follow.y = (boyfriend->y + boyfriend->graphic.h * 0.5f) - 100;
    }
    else{
        switch(stage) {
            case LIMO: {
                offsety = -100;
                break;
            }
            default: break;
        }
        cam_follow.x = (dad->x + dad->graphic.w * 0.5f) + 150;
        cam_follow.y = (dad->y + dad->graphic.h * 0.5f) - 100;
    }

    cam_follow.x += offsetx;
    cam_follow.y += offsety;

}

void beat_hit(int32 beat){
    if(bf_dead)
        return;
    //quick comparison
    if(beat % 2 == 0){    
        if(boyfriend->animation.currentAnimation->name[0] != 's')
            animation_play(&boyfriend->animation, "idle");
        if(dad->animation.currentAnimation->name[0] != 's')
            animation_play(&dad->animation, "idle");
    }

    if(beat % 2)
        animation_play(&gf->animation, "danceLeft");
    else
        animation_play(&gf->animation, "danceRight");
}

void uninit_play_state(){
    stop_audio(&inst);
    stop_audio(&voices);

    delete_audio(&inst);
    delete_audio(&voices);

    FNF_DELETE(note_sprites);
    black = 0;

    free_sprites();
    kill_sprites();
}

bool bring_to_front(fnf_sprite** index, fnf_sprite** end) {
    if (index == NULL || end == NULL)
        return false;

    fnf_sprite** start = index;
    fnf_sprite** ptr = end;

    while (start < ptr) {    
        fnf_sprite* temp = *start;
        *start = *ptr;
        *ptr = temp;
        start++;
        ptr--;
    }
    return true;
}

void song_info(){
    JSON_Object* json_song = json_value_get_object(chart);
    JSON_Array* notes = json_object_dotget_array(json_song, "song.notes");
    const char* song_name = json_object_dotget_string(json_song, "song.song");
    song.bpm = json_object_dotget_number(json_song, "song.bpm");
    song.speed = json_object_dotget_number(json_song, "song.speed");
    strcpy(song.name, song_name);
    strcpy(song.bf, json_object_dotget_string(json_song, "song.player1"));
    strcpy(song.dad, json_object_dotget_string(json_song, "song.player2"));

    if(!strcmp(load_song_name, "bopeebo") || !strcmp(load_song_name, "fresh") || !strcmp(load_song_name, "dadbattle"))
        stage = STAGE;
    if(!strcmp(load_song_name, "spookeez") || !strcmp(load_song_name, "south") || !strcmp(load_song_name, "monster"))
        stage = SPOOKY;
    if(!strcmp(load_song_name, "pico") || !strcmp(load_song_name, "philly") || !strcmp(load_song_name, "blammed"))
        stage = PHILLY;
    if(!strcmp(load_song_name, "satin-panties") || !strcmp(load_song_name, "high") || !strcmp(load_song_name, "milf"))
        stage = LIMO;
    if(!strcmp(load_song_name, "cocoa") || !strcmp(load_song_name, "eggnog") || !strcmp(load_song_name, "winter-horrorland"))
        stage = MALL;
    if(!strcmp(load_song_name, "senpai") || !strcmp(load_song_name, "roses") || !strcmp(load_song_name, "thorns"))
        stage = SCHOOL;
    if(!strcmp(load_song_name, "ugh") || !strcmp(load_song_name, "guns") || !strcmp(load_song_name, "stress"))
        stage = TANK;
}

int count_notes(){
    int size = 0;
    JSON_Object *root_object = json_value_get_object(chart);
    JSON_Object *song_object = json_object_get_object(root_object, "song");
    JSON_Array *notes_array = json_object_get_array(song_object, "notes");
    size_t notes_count = json_array_get_count(notes_array);
    for (size_t i = 0; i < notes_count; i++) {
        JSON_Object *note_object = json_array_get_object(notes_array, i);
        JSON_Array *section_notes = json_object_get_array(note_object, "sectionNotes");

        size_t sectionNotes_count = json_array_get_count(section_notes);
        for (size_t j = 0; j < sectionNotes_count; j++) {
            size++;
            JSON_Array* section_note = json_array_get_array(section_notes, j);
            int sus_time = json_array_get_number(section_note, 2);
            if(!sus_time) continue;
            sus_time = sus_time / current_conductor->stepCrochet;
            size += (int)floor(sus_time);
        }
    }
    return size;
}

void generate_song(const char* path){
    JSON_Object* json_song = json_value_get_object(chart);
    JSON_Array* notes = json_object_dotget_array(json_song, "song.notes");
    const char* song_name = json_object_dotget_string(json_song, "song.song");
    song.bpm = json_object_dotget_number(json_song, "song.bpm");
    song.speed = json_object_dotget_number(json_song, "song.speed");
    strcpy(song.name, song_name);
    strcpy(song.bf, json_object_dotget_string(json_song, "song.player1"));
    strcpy(song.dad, json_object_dotget_string(json_song, "song.player2"));
    //song.name[255] = 0;

    assert(notes != NULL);

    size_t notes_malloc = sizeof(fnf_note) * count_notes();
    note_sprites = (fnf_note*)malloc(notes_malloc);
    FNF_ZERO(note_sprites, notes_malloc);

    for(int i=0; i < json_array_get_count(notes); i++){
        JSON_Object* section = json_array_get_object(notes, i);
        JSON_Array* section_notes = json_object_get_array(section, "sectionNotes");

        for(int notes_index=0; notes_index < json_array_get_count(section_notes); notes_index++){
            JSON_Array* note_data = json_array_get_array(section_notes, notes_index);
            int strum_time = json_array_get_number(note_data, 0);
            int note_type = json_array_get_number(note_data, 1);
            int sus_time = json_array_get_number(note_data, 2);
            int must_hit = json_object_get_boolean(section, "mustHitSection");
            int alt = json_object_get_boolean(section, "altAnim") != -1;


            hit_sections[i] = must_hit;

            if(sus_time){
                sus_time = (int)floor(sus_time / current_conductor->stepCrochet);

                for(int sus=0; sus<sus_time; sus++){
                    fnf_note* da_note = generate_note(strum_time + (current_conductor->stepCrochet * sus) + current_conductor->stepCrochet, note_type, must_hit, 1);
                    da_note->alt = alt;

                    const char** sus_type = sus == sus_time-1 ? directions_sus_end : directions_sus;

                    if(sus != sus_time-1)
                        scale_sprite(da_note->sprite, .7f, da_note->sprite->scale.y * (current_conductor->stepCrochet / 100 * 1.5 * song.speed));
                    animation_play(&da_note->sprite->animation, sus_type[note_type % 4]);
                    move_sprite(da_note->sprite, da_note->sprite->x + 40, da_note->sprite->y);
                    add_sprite(da_note->sprite);
                }
            }

            fnf_note* da_note = generate_note(strum_time, note_type, must_hit, 0);
            da_note->alt = alt;

            add_sprite(da_note->sprite);

        }
    }
}

fnf_note* generate_note(float strum_time, int note_type, int must_hit, bool sus_time){
    fnf_note* da_note = &note_sprites[note_size++];
    da_note->strum_time = strum_time;
    da_note->sprite = clone_sprite(arrow_base);
    da_note->note_data = note_type;

    da_note->sustain = sus_time;
    uint8_t player = note_type > 3;
    if(must_hit)
        player = !player;

    da_note->must_press = player;
    scale_sprite(da_note->sprite, 0.7f, 0.7f);
    animation_play(&da_note->sprite->animation, directions_color[note_type % 4]);
    note_type %= 4;
    move_sprite(da_note->sprite, (SWAG_WIDTH * note_type) + 92 + ((SCREEN_WIDTHF/2) * player), 50);
    return da_note;
}

void generate_static_notes(){
    for(int i=0; i<8; i++){
        fnf_sprite** strums = i < 4 ? enemyStrums : playerStrums;
        fnf_sprite* strum = clone_sprite(arrow_base);
        uint32_t side = i>3;

        strums[i % 4] = strum;

        scale_sprite(strum, 0.7f, 0.7f);
        animation_play(&strum->animation, directions[i % 4]);
        uint32_t strumx = (SWAG_WIDTH * (i % 4)) + 92 + ((SCREEN_WIDTHF/2) * side);
        move_sprite(strum, strumx, 50);
        add_sprite(strum);
    }

    return;
}

bool pressed_key(bool* keys){
    return (*(uint32_t*)keys) != 0x00000000;
}

void good_note_hit(fnf_note* da_note){
    if(da_note->was_good_hit) return;
    for(int i=0; i < 4; i++){
        if(da_note->note_data % 4 == i)
            animation_play(&playerStrums[i]->animation, directions_confirm[i]);
    }

    da_note->sprite->enabled = false;
    da_note->was_good_hit = true;
    switch(da_note->note_data % 4){
        case 0:
        animation_play(&boyfriend->animation, "singLEFT"); break;
        case 1:
        animation_play(&boyfriend->animation, "singDOWN"); break;
        case 2:
        animation_play(&boyfriend->animation, "singUP"); break;
        case 3:
        animation_play(&boyfriend->animation, "singRIGHT"); break;
    }

    if(da_note->sustain) return;

    int note_diff = abs(da_note->strum_time - current_conductor->songPosition);

    score += 350;

    if (note_diff > current_conductor->safeZoneOffset * 0.9)
		score -= 300;
	else if (note_diff > current_conductor->safeZoneOffset * 0.75)
		score -= 250;
	else if (note_diff > current_conductor->safeZoneOffset * 0.2)
		score -= 150;
    
    add_health(0.023f);
    set_audio_volume(&voices, 1.0f);

    sprintf(score_txt, "Score: %i", score);
}

void bf_death(){
    stop_audio(&inst);
    stop_audio(&voices);

    fnf_sprite** sprites = get_sprites();
    uint16 sprites_size = *get_sprites_size();

    for(int i=0; i<sprites_size; i++) {
        if(!sprites[i]) continue;
        sprites[i]->enabled = false;
    }
    boyfriend->enabled = true;
    animation_play(&boyfriend->animation, "firstDeath");
    bf_dead = true;
    play_audio(&dead);
    //switch_state(&play_state);
}

void add_health(float hp){
    health += hp;
    float norm_health = health / 2.0f;

    if(health > 2.0f)
        health = 2.0f;
    if(health < 0.0f) {
        health = 0.0f;
        bf_death();
    }

    bf_icon->animation.frameNum = 0;
    dad_icon->animation.frameNum = 0;

    if(norm_health < 0.2f)
        bf_icon->animation.frameNum = 1;
    if(norm_health > 0.8f)
        dad_icon->animation.frameNum = 1;

    float ratio = health / 2.0f;
    ratio *= 100.0f;
    ratio = remap(ratio, 0.0f, 100.0f, 100.0f, 0.0f);

    move_sprite(bf_icon, ((health_bar->w * 0.5f) + health_bar->x + (health_bar->w * ratio * 0.01f) - 26) - (health_bar->w * 0.5f), dad_icon->y);
    move_sprite(dad_icon, (health_bar->w * 0.5f) + health_bar->x + (health_bar->w * ratio * 0.01f) - (150 - 26) - (health_bar->w * 0.5f), dad_icon->y);
    scale_sprite(health_bar, FNF_MAX(1.0f - norm_health, 0), health_bar->scale.y);
}

void keyShit(){
    bool hold_array[] = {
        key_pressed(LEFT),
        key_pressed(DOWN),
        key_pressed(UP),
        key_pressed(RIGHT)
    };

    bool press_array[] = {
        key_just_pressed(LEFT),
        key_just_pressed(DOWN),
        key_just_pressed(UP),
        key_just_pressed(RIGHT)
    };

    for(int i=0; i < note_size; i++){
        if(!pressed_key(press_array) && !pressed_key(hold_array)) break;
        
        fnf_note* da_note = &note_sprites[i];
        if(!da_note->sprite->enabled) continue;

        if (da_note->sustain && da_note->can_be_hit && da_note->must_press && hold_array[da_note->note_data % 4]){
                good_note_hit(da_note);
                printf("sus might have been pressed or something %i\n", da_note->note_data);
        }
        if (da_note->can_be_hit && da_note->must_press && !da_note->too_late && !da_note->was_good_hit && !da_note->sustain){
            if(press_array[da_note->note_data % 4]){
                good_note_hit(da_note);
                printf("note might have been pressed or something %i\n", da_note->note_data);
            }
        }
    }    
    if(pressed_key(press_array))
        bf_timer = 0;
    for(int i=0; i < 4; i++){
        fnf_sprite* da_note = playerStrums[i];
        bool confirm_note = strcmp(da_note->animation.currentAnimation->name, directions_confirm[i]) == 0;
        if(press_array[i] && !confirm_note)
            animation_play(&da_note->animation, directions_pressed[i]);
        if (!hold_array[i]){
            animation_play(&da_note->animation, directions[i]);}
    }

    if (bf_timer > current_conductor->stepCrochet * 4.0f * 0.001f && (*(uint32_t*)hold_array) == 0x00000000)
			if (boyfriend->animation.currentAnimation->name[0] == 's')
				animation_play(&boyfriend->animation, "idle");
}

void end_song(){
    if(bf_dead) return;
    set_highscore(load_song_name, score);
    if(current_week){
        strcpy(load_song_name, current_week[++current_week_song]);
        tolowersptr(load_song_name);
        if(current_week_song > 2 || !current_week[current_week_song]) {
            switch_state(&story_menu_state);
            return;
        }
        switch_state(&play_state);
        return;
    }
    switch_state(&freeplay_state);
}