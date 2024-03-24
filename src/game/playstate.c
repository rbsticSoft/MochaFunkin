#include "states.h"
#include "controls.h"
#include "parson.h"
#include "note.h"
#include "conductor.h"
#include "audio.h"
#include "font.h"

#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

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

fnf_song song;

bool hit_sections[256];

const char* directions[] = {
    "static left",
    "static down",
    "static up",
    "static right"
};

const char* directions_pressed[] = {
    "pressed left",
    "pressed down",
    "pressed up",
    "pressed right"
};

const char* directions_confirm[] = {
    "confirm left",
    "confirm down",
    "confirm up",
    "confirm right"
};

const char* directions_color[] = {
    "purpleScroll",
    "blueScroll",
    "greenScroll",
    "redScroll"
};

const char* directions_sus[] = {
    "purpleHold",
    "blueHold",
    "greenHold",
    "redHold"
};

const char* directions_sus_end[] = {
    "purpleEnd",
    "blueEnd",
    "greenEnd",
    "redEnd"
};

const char* load_song_name = "pico";
const char* difficulty = "hard";
char score_txt[32] = "Score: 0";

int score = 0;
float health = 0.0f;
bool must_hit_now = false;

bool bring_to_front(fnf_sprite** index, fnf_sprite** end);
int count_notes();
void generate_song(const char* path);
fnf_note* generate_note(float strum_time, int note_type, int must_hit, bool sus_time);
void generate_static_notes();
void keyShit();

fnf_camera* play_camera;
fnf_camera* hud_camera;

fnf_vector cam_follow;

fnf_sprite* dad, *gf, *boyfriend;

float lerp(float a, float b, float ratio)
{
    return a + ratio * (b - a);
}

void create_play_state(){
    play_camera = make_camera();
    hud_camera = make_camera();

    play_camera->zoom = 0.9f;

    fnf_sprite* stage_back = make_sprite(-600, -200, false);
    stage_back->camera = play_camera;
    stage_back->scroll.x = 0.9f;
    stage_back->scroll.x = 0.9f;
    load_sprite(stage_back, "assets/shared/images/stageback.png");

    fnf_sprite* stage_front = make_sprite(-650, 600, false);
    stage_front->camera = play_camera;
    stage_front->scroll.x = 0.9f;
    stage_front->scroll.x = 0.9f;
    //scale_sprite(stage_front, 1.1f, 1.0f);
    load_sprite(stage_front, "assets/shared/images/stagefront.png");
    resize_sprite(stage_front, stage_front->w * 1.1, stage_front->h);

    fnf_sprite* stage_curtains = make_sprite(-500, -300, false);
    stage_curtains->camera = play_camera;
    stage_curtains->scroll.x = 1.3f;
    stage_curtains->scroll.y = 1.3f;
    scale_sprite(stage_curtains, 0.9f, 1.0f);
    load_sprite(stage_curtains, "assets/shared/images/stagecurtains.png");
    resize_sprite(stage_curtains, stage_curtains->w * 0.9, stage_curtains->h);

    add_sprite(stage_back);
    add_sprite(stage_front);
    add_sprite(stage_curtains);

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

    boyfriend = make_sprite(770, 450, true);
    boyfriend->camera = play_camera;
    load_sprite(boyfriend, "assets/shared/images/characters/BOYFRIEND.png");
    animation_load_atlas(&boyfriend->animation, "assets/shared/images/characters/BOYFRIEND.xml");
    animation_add_prefix(&boyfriend->animation, "idle", "BF idle dance", false, 24);
    animation_add_prefix(&boyfriend->animation, "singLEFT", "BF NOTE LEFT", false, 24);
    animation_add_prefix(&boyfriend->animation, "singDOWN", "BF NOTE DOWN", false, 24);
    animation_add_prefix(&boyfriend->animation, "singUP", "BF NOTE UP", false, 24);
    animation_add_prefix(&boyfriend->animation, "singRIGHT", "BF NOTE RIGHT", false, 24);
    
    animation_play(&boyfriend->animation, "idle");
    add_sprite(boyfriend);

    dad = make_sprite(100, 100, true);
    dad->camera = play_camera;
    load_sprite(dad, "assets/shared/images/characters/DADDY_DEAREST.png");
    animation_load_atlas(&dad->animation, "assets/shared/images/characters/DADDY_DEAREST.xml");
    animation_add_prefix(&dad->animation, "idle", "Dad idle dance", false, 24);
    animation_play(&dad->animation, "idle");
    add_sprite(dad);

    arrow_base = make_sprite(0, 0, true);
    animation_load_atlas(&arrow_base->animation, "assets/images/NOTE_assets.xml");

    animation_add_prefix(&arrow_base->animation, "greenScroll", "green instance 1", false, 30);
	animation_add_prefix(&arrow_base->animation, "redScroll", "red instance 1", false, 30);
	animation_add_prefix(&arrow_base->animation, "blueScroll", "blue instance 1",false, 30);
	animation_add_prefix(&arrow_base->animation, "purpleScroll", "purple instance 1", false, 30);

    animation_add_prefix(&arrow_base->animation, "greenHold", "green hold piece instance 1", false, 30);
    animation_add_prefix(&arrow_base->animation, "redHold", "red hold piece instance 1", false, 30);
    animation_add_prefix(&arrow_base->animation, "blueHold", "blue hold piece instance 1", false, 30);
    animation_add_prefix(&arrow_base->animation, "purpleHold", "purple hold piece instance 1", false, 30);

    animation_add_prefix(&arrow_base->animation, "greenEnd", "green hold end instance 1", false, 30);
    animation_add_prefix(&arrow_base->animation, "redEnd", "red hold end instance 1", false, 30);
    animation_add_prefix(&arrow_base->animation, "blueEnd", "blue hold end instance 1", false, 30);
    animation_add_prefix(&arrow_base->animation, "purpleEnd", "pruple end hold instance 1", false, 30);

	animation_add_prefix(&arrow_base->animation, "static left", "arrow static instance 1", false, 24);
    animation_add_prefix(&arrow_base->animation, "pressed left", "left press instance 1", false, 24);
	animation_add_prefix(&arrow_base->animation, "confirm left", "left confirm instance 1", false, 24);

	animation_add_prefix(&arrow_base->animation, "static down", "arrow static instance 2", false, 24);
    animation_add_prefix(&arrow_base->animation, "pressed down", "down press instance 1", false, 24);
	animation_add_prefix(&arrow_base->animation, "confirm down", "down confirm instance 1", false, 24);

	animation_add_prefix(&arrow_base->animation, "static up", "arrow static instance 4", false, 24);
    animation_add_prefix(&arrow_base->animation, "pressed up", "up press instance 1", false, 24);
	animation_add_prefix(&arrow_base->animation, "confirm up", "up confirm instance 1", false, 24);

	animation_add_prefix(&arrow_base->animation, "static right", "arrow static instance 3", false, 24);
	animation_add_prefix(&arrow_base->animation, "pressed right", "right press instance 1", false, 24);
	animation_add_prefix(&arrow_base->animation, "confirm right", "right confirm instance 1", false, 24);

    //fnf_sprite* rarrow = clone_sprite(arrow_base);
    animation_play(&arrow_base->animation, "redScroll");
    //rarrow->animated = false;
    load_sprite(arrow_base, "assets/images/NOTE_assets.png");

    generate_static_notes();

    char path[255];
    sprintf(path, "assets/data/%s/%s-%s.json", load_song_name, load_song_name, difficulty);
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
    resize_sprite(health_bar, (health_bar_bg->w - 8) * 0.5f, health_bar_bg->h - 8);
    set_color(health_bar, 0xFF0000FF);
	//healthBar.createFilledBar(, 0xFF66FF33);
	add_sprite(health_bar);

    change_bpm(current_conductor, song.bpm);
    
    inst = make_audio();
    voices = make_audio();

    sprintf(path, "assets/songs/%s/Inst.ogg", load_song_name);
    load_audio(&inst, path);
    sprintf(path, "assets/songs/%s/Voices.ogg", load_song_name);
    load_audio(&voices, path);

    play_audio(&inst);
    play_audio(&voices);
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
        da_note->sprite->enabled = false;
    }
    return true;
}
void beat_hit();
void draw_play_state(){
    static int32 oldStep = 0;
	static int32 curStep = 0;

    current_conductor->songPosition = roundf(get_audio_position(&inst)) + current_conductor->offset;
    oldStep = curStep;
    curStep = (int32)floorf(current_conductor->songPosition / current_conductor->stepCrochet);

    keyShit();

    for(int i=0; i < note_size; i++){
        fnf_note* da_note = &note_sprites[i];
        int strmid = 50 + SWAG_WIDTH / 2;
        float ypos = (50 - (current_conductor->songPosition - note_sprites[i].strum_time) * (0.45 * song.speed));

        //if(da_note->sustain)
            //ypos += da_note->sprite->graphic.h / 2.0f;

        if(da_note->was_good_hit && !da_note->must_press)
            da_note->sprite->enabled = false;

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
        update_note(&note_sprites[i]);
    }

    fnf_sprite* whosturn = must_hit_now ? boyfriend : dad;

    if(hit_sections[curStep/16]){
        cam_follow.x = ((boyfriend->graphic.w) * 0.5f) - 100;
        cam_follow.y = ((boyfriend->graphic.h) * 0.5f) - 100;
    }
    else{
        cam_follow.x = 400 + (dad->graphic.w / 2);
        cam_follow.y = (dad->graphic.w / 2);
    }

    play_camera->x = lerp(play_camera->x, cam_follow.x, 0.04);
    play_camera->y = lerp(play_camera->y, cam_follow.y, 0.04);


    if(curStep % 4 == 0 && oldStep != curStep)
        beat_hit();

    draw_all_sprites();

    draw_text(score_txt, health_bar_bg->x + health_bar_bg->w - 190.f, health_bar_bg->y + 30.f, 1.f);
}

void beat_hit(){
    static bool danceLeft = false;

    animation_play(&boyfriend->animation, "idle");
    danceLeft = !danceLeft;
        if(danceLeft)
            animation_play(&gf->animation, "danceLeft");
        else
            animation_play(&gf->animation, "danceRight");
}

void uninit_play_state(){
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
    chart = json_parse_file(path);

    JSON_Object* json_song = json_value_get_object(chart);
    JSON_Array* notes = json_object_dotget_array(json_song, "song.notes");
    const char* song_name = json_object_dotget_string(json_song, "song.song");
    song.bpm = json_object_dotget_number(json_song, "song.bpm");
    song.speed = json_object_dotget_number(json_song, "song.speed");
    memcpy(song.name, song_name, 254);
    song.name[255] = 0;

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
            hit_sections[i] = must_hit;

            if(sus_time){
                sus_time = (int)floor(sus_time / current_conductor->stepCrochet);

                for(int sus=0; sus<sus_time; sus++){
                    fnf_note* da_note = generate_note(strum_time + (current_conductor->stepCrochet * sus) + current_conductor->stepCrochet, note_type, must_hit, 1);
                    const char** sus_type = sus == sus_time-1 ? directions_sus_end : directions_sus;

                    if(sus != sus_time-1)
                        scale_sprite(da_note->sprite, .7f, da_note->sprite->scale.y * (current_conductor->stepCrochet / 100 * 1.5 * song.speed));
                    animation_play(&da_note->sprite->animation, sus_type[note_type % 4]);
                    move_sprite(da_note->sprite, da_note->sprite->x + 40, da_note->sprite->y);
                    add_sprite(da_note->sprite);
                }
            }

            fnf_note* da_note = generate_note(strum_time, note_type, must_hit, 0);
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
    for(int i=0; i<4; i++)
        if(keys[i])
            return true;
    return false;
}

void good_note_hit(fnf_note* da_note){
    if(da_note->was_good_hit) return;
    for(int i=0; i < 4; i++){
        if(da_note->note_data == i){
            animation_play(&playerStrums[i]->animation, directions_confirm[i]);
            playerStrums[i]->offset.x = -20.f;
            playerStrums[i]->offset.y = -20.f;
        }
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
    
    sprintf(score_txt, "Score: %i", score);
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

    for(int i=0; i < 4; i++){
        fnf_sprite* da_note = playerStrums[i];
        bool confirm_note = strcmp(da_note->animation.currentAnimation->name, directions_confirm[i]) == 0;
        if(press_array[i] && !confirm_note)
            animation_play(&da_note->animation, directions_pressed[i]);
        if (!hold_array[i]){
            playerStrums[i]->offset.x = 0.f;
            playerStrums[i]->offset.y = 0.f;
            animation_play(&da_note->animation, directions[i]);}
    }

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
}