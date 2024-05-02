#include "spriteutil.h"
#include "states.h"
#include <stdlib.h>
#include <stdio.h>

fnf_sprite* create_character(float x, float y, const char* name){
    fnf_sprite* character = make_sprite(x, y, true);

    if(strcmp(name, "bf") == 0){
        load_sprite(character, "assets/shared/images/characters/BOYFRIEND.png");
        animation_load_atlas(&character->animation, "assets/shared/images/characters/BOYFRIEND.xml");
        animation_add_prefix(&character->animation, "idle", "BF idle dance", false, 24);
        animation_add_prefix(&character->animation, "singLEFT", "BF NOTE LEFT", false, 24);
        animation_add_prefix(&character->animation, "singDOWN", "BF NOTE DOWN", false, 24);
        animation_add_prefix(&character->animation, "singUP", "BF NOTE UP", false, 24);
        animation_add_prefix(&character->animation, "singRIGHT", "BF NOTE RIGHT", false, 24);

        animation_add_prefix(&character->animation, "singLEFTmiss", "BF NOTE LEFT MISS", false, 24);
        animation_add_prefix(&character->animation, "singDOWNmiss", "BF NOTE DOWN MISS", false, 24);
        animation_add_prefix(&character->animation, "singUPmiss", "BF NOTE UP MISS", false, 24);
        animation_add_prefix(&character->animation, "singRIGHTmiss", "BF NOTE RIGHT MISS", false, 24);

        animation_add_prefix(&character->animation, "firstDeath", "BF dies", false, 24);
        animation_add_prefix(&character->animation, "deathLoop", "BF Dead Loop", false, 24);
        animation_add_prefix(&character->animation, "deathConfirm", "BF Dead confirm", false, 24);
        
        animation_play(&character->animation, "idle");
    }

    if(strcmp(name, "bf-pixel") == 0){
        load_sprite(character, "assets/shared/images/characters/bfPixel.png");
        animation_load_atlas(&character->animation, "assets/shared/images/characters/bfPixel.xml");
        animation_add_prefix(&character->animation, "idle", "BF IDLE instance 1", false, 24);
        animation_add_prefix(&character->animation, "singLEFT", "BF LEFT NOTE instance 1", false, 24);
        animation_add_prefix(&character->animation, "singDOWN", "BF DOWN NOTE instance 1", false, 24);
        animation_add_prefix(&character->animation, "singUP", "BF UP NOTE instance 1", false, 24);
        animation_add_prefix(&character->animation, "singRIGHT", "BF RIGHT NOTE instance 1", false, 24);

        animation_add_prefix(&character->animation, "singLEFTmiss", "BF LEFT MISS instance 1", false, 24);
        animation_add_prefix(&character->animation, "singDOWNmiss", "BF DOWN MISS instance 1", false, 24);
        animation_add_prefix(&character->animation, "singUPmiss", "BF UP MISS instance 1", false, 24);
        animation_add_prefix(&character->animation, "singRIGHTmiss", "BF RIGHT MISS instance 1", false, 24);

        scale_sprite(character, 6.0f, 6.0f);

        animation_play(&character->animation, "idle");
    }

    if(strcmp(name, "bf-car") == 0){
        load_sprite(character, "assets/shared/images/characters/bfCar.png");
        animation_load_atlas(&character->animation, "assets/shared/images/characters/bfCar.xml");
        animation_add_prefix(&character->animation, "idle", "BF idle dance", false, 24);
        animation_add_prefix(&character->animation, "singLEFT", "BF NOTE LEFT", false, 24);
        animation_add_prefix(&character->animation, "singDOWN", "BF NOTE DOWN", false, 24);
        animation_add_prefix(&character->animation, "singUP", "BF NOTE UP", false, 24);
        animation_add_prefix(&character->animation, "singRIGHT", "BF NOTE RIGHT", false, 24);

        animation_play(&character->animation, "idle");
    }

    if(strcmp(name, "bf-christmas") == 0){
        move_sprite(character, character->x + 200, character->y);
        load_sprite(character, "assets/shared/images/characters/bfChristmas.png");
        animation_load_atlas(&character->animation, "assets/shared/images/characters/bfChristmas.xml");
        animation_add_prefix(&character->animation, "idle", "BF idle dance", false, 24);
        animation_add_prefix(&character->animation, "singLEFT", "BF NOTE LEFT", false, 24);
        animation_add_prefix(&character->animation, "singDOWN", "BF NOTE DOWN", false, 24);
        animation_add_prefix(&character->animation, "singUP", "BF NOTE UP", false, 24);
        animation_add_prefix(&character->animation, "singRIGHT", "BF NOTE RIGHT", false, 24);

        animation_play(&character->animation, "idle");
    }

    if(strcmp(name, "bf-holding-gf") == 0){
        move_sprite(character, character->x + 200, character->y);
        load_sprite(character, "assets/shared/images/characters/bfAndGF.png");
        animation_load_atlas(&character->animation, "assets/shared/images/characters/bfAndGF.xml");
        animation_add_prefix(&character->animation, "idle", "BF idle dance w gf", false, 24);
        animation_add_prefix(&character->animation, "singLEFT", "BF NOTE LEFT", false, 24);
        animation_add_prefix(&character->animation, "singDOWN", "BF NOTE DOWN", false, 24);
        animation_add_prefix(&character->animation, "singUP", "BF NOTE UP", false, 24);
        animation_add_prefix(&character->animation, "singRIGHT", "BF NOTE RIGHT", false, 24);

        animation_play(&character->animation, "idle");
    }

    if(strcmp(name, "dad") == 0){
        load_sprite(character, "assets/shared/images/characters/DADDY_DEAREST.png");
        animation_load_atlas(&character->animation, "assets/shared/images/characters/DADDY_DEAREST.xml");
        animation_add_prefix(&character->animation, "idle", "Dad idle dance", false, 24);
        animation_add_prefix(&character->animation, "singLEFT", "Dad Sing Note LEFT", false, 24);
        animation_add_prefix(&character->animation, "singDOWN", "Dad Sing Note DOWN", false, 24);
        animation_add_prefix(&character->animation, "singUP", "Dad Sing Note UP", false, 24);
        animation_add_prefix(&character->animation, "singRIGHT", "Dad Sing Note RIGHT", false, 24);
        animation_play(&character->animation, "idle");
    }

    if(strcmp(name, "spooky") == 0){
        move_sprite(character, character->x, character->y + 200);
        load_sprite(character, "assets/shared/images/characters/spooky_kids_assets.png");
        animation_load_atlas(&character->animation, "assets/shared/images/characters/spooky_kids_assets.xml");
        animation_add_prefix(&character->animation, "singUP", "spooky UP NOTE", false, 24);
        animation_add_prefix(&character->animation, "singDOWN", "spooky DOWN note", false, 24);
        animation_add_prefix(&character->animation, "singLEFT", "note sing left", false, 24);
        animation_add_prefix(&character->animation, "singRIGHT", "spooky sing right", false, 24);
        animation_play(&character->animation, "singUP");
    }

    if(strcmp(name, "monster") == 0){
        move_sprite(character, character->x, character->y + 100);
        load_sprite(character, "assets/shared/images/characters/Monster_Assets.png");
        animation_load_atlas(&character->animation, "assets/shared/images/characters/Monster_Assets.xml");
        animation_add_prefix(&character->animation, "idle", "monster idle", false, 24);
        animation_add_prefix(&character->animation, "singUP", "monster up note", false, 24);
        animation_add_prefix(&character->animation, "singDOWN", "monster down", false, 24);
        animation_add_prefix(&character->animation, "singLEFT", "Monster Right note", false, 24);
        animation_add_prefix(&character->animation, "singRIGHT", "Monster left note", false, 24);
        animation_play(&character->animation, "idle");
    }

    if(strcmp(name, "pico") == 0){
        load_sprite(character, "assets/shared/images/characters/Pico_FNF_assetss.png");
        animation_load_atlas(&character->animation, "assets/shared/images/characters/Pico_FNF_assetss.xml");
        animation_add_prefix(&character->animation, "idle", "Pico Idle Dance", false, 24);
        animation_add_prefix(&character->animation, "singLEFT", "Pico NOTE LEFT", false, 24);
        animation_add_prefix(&character->animation, "singDOWN", "Pico Down Note", false, 24);
        animation_add_prefix(&character->animation, "singUP", "pico Up note", false, 24);
        animation_add_prefix(&character->animation, "singRIGHT", "Pico Note Right", false, 24);
        animation_play(&character->animation, "idle");

        set_flip(character, X);

        move_sprite(character, character->x, character->y + 300);
    }

    if(strcmp(name, "mom-car") == 0){
        load_sprite(character, "assets/shared/images/characters/momCar.png");
        animation_load_atlas(&character->animation, "assets/shared/images/characters/momCar.xml");
        animation_add_prefix(&character->animation, "idle", "Mom Idle", false, 24);
        animation_add_prefix(&character->animation, "singLEFT", "Mom Left Pose", false, 24);
        animation_add_prefix(&character->animation, "singDOWN", "MOM DOWN POSE", false, 24);
        animation_add_prefix(&character->animation, "singUP", "Mom Up Pose", false, 24);
        animation_add_prefix(&character->animation, "singRIGHT", "Mom Pose Left", false, 24);
        animation_play(&character->animation, "idle");
    }

    if(strcmp(name, "parents-christmas") == 0){
        move_sprite(character, character->x - 500, character->y);
        load_sprite(character, "assets/shared/images/characters/mom_dad_christmas_assets.png");
        animation_load_atlas(&character->animation, "assets/shared/images/characters/mom_dad_christmas_assets.xml");
        animation_add_prefix(&character->animation, "idle", "Parent Christmas Idle", false, 24);
        animation_add_prefix(&character->animation, "singLEFT", "Parent Left Note Dad", false, 24);
        animation_add_prefix(&character->animation, "singDOWN", "Parent Down Note Dad", false, 24);
        animation_add_prefix(&character->animation, "singUP", "Parent Up Note Dad", false, 24);
        animation_add_prefix(&character->animation, "singRIGHT", "Parent Right Note Dad", false, 24);

        animation_add_prefix(&character->animation, "singLEFT-alt", "Parent Left Note Mom", false, 24);
        animation_add_prefix(&character->animation, "singDOWN-alt", "Parent Down Note Mom", false, 24);
        animation_add_prefix(&character->animation, "singUP-alt", "Parent Up Note Mom", false, 24);
        animation_add_prefix(&character->animation, "singRIGHT-alt", "Parent Right Note Mom", false, 24);
        animation_play(&character->animation, "idle");
    }

    if(strcmp(name, "monster-christmas") == 0){
        move_sprite(character, character->x, character->y + 130);
        load_sprite(character, "assets/shared/images/characters/monsterChristmas.png");
        animation_load_atlas(&character->animation, "assets/shared/images/characters/monsterChristmas.xml");
        animation_add_prefix(&character->animation, "idle", "monster idle", false, 24);
        animation_add_prefix(&character->animation, "singUP", "monster up note", false, 24);
        animation_add_prefix(&character->animation, "singDOWN", "monster down", false, 24);
        animation_add_prefix(&character->animation, "singLEFT", "Monster Right note", false, 24);
        animation_add_prefix(&character->animation, "singRIGHT", "Monster left note", false, 24);
        animation_play(&character->animation, "idle");
    }

    if(strcmp(name, "senpai") == 0){
        load_sprite(character, "assets/shared/images/characters/senpai.png");
        animation_load_atlas(&character->animation, "assets/shared/images/characters/senpai.xml");
        animation_add_prefix(&character->animation, "idle", "Senpai Idle instance 1", false, 24);
        animation_add_prefix(&character->animation, "singLEFT", "SENPAI LEFT NOTE instance 1", false, 24);
        animation_add_prefix(&character->animation, "singDOWN", "SENPAI DOWN NOTE instance 1", false, 24);
        animation_add_prefix(&character->animation, "singUP", "SENPAI UP NOTE instance 1", false, 24);
        animation_add_prefix(&character->animation, "singRIGHT", "SENPAI RIGHT NOTE instance 1", false, 24);

        scale_sprite(character, 6.0f, 6.0f);

        animation_play(&character->animation, "idle");
    }

    if(strcmp(name, "senpai-angry") == 0){
        load_sprite(character, "assets/shared/images/characters/senpai.png");
        animation_load_atlas(&character->animation, "assets/shared/images/characters/senpai.xml");
        animation_add_prefix(&character->animation, "idle", "Angry Senpai Idle instance 1", false, 24);
        animation_add_prefix(&character->animation, "singLEFT", "Angry Senpai LEFT NOTE instance 1", false, 24);
        animation_add_prefix(&character->animation, "singDOWN", "Angry Senpai DOWN NOTE instance 1", false, 24);
        animation_add_prefix(&character->animation, "singUP", "Angry Senpai UP NOTE instance 1", false, 24);
        animation_add_prefix(&character->animation, "singRIGHT", "Angry Senpai RIGHT NOTE instance 1", false, 24);

        scale_sprite(character, 6.0f, 6.0f);

        animation_play(&character->animation, "idle");
    }

    if(strcmp(name, "spirit") == 0){
        load_sprite(character, "assets/shared/images/characters/spirit.png");
        animation_load_packer(&character->animation, "assets/shared/images/characters/spirit.txt");
        animation_add_prefix(&character->animation, "idle", "idle spirit_", false, 24);
        animation_add_prefix(&character->animation, "singLEFT", "left_", false, 24);
        animation_add_prefix(&character->animation, "singDOWN", "down_", false, 24);
        animation_add_prefix(&character->animation, "singUP", "up_", false, 24);
        animation_add_prefix(&character->animation, "singRIGHT", "right_", false, 24);
        animation_play(&character->animation, "idle");
    }

    if(strcmp(name, "tankman") == 0){
        load_sprite(character, "assets/shared/images/characters/tankmanCaptain.png");
        animation_load_atlas(&character->animation, "assets/shared/images/characters/tankmanCaptain.xml");
        animation_add_prefix(&character->animation, "idle", "Tankman Idle Dance instance 1", false, 24);
        animation_add_prefix(&character->animation, "singLEFT", "Tankman Right Note instance 1", false, 24);
        animation_add_prefix(&character->animation, "singDOWN", "Tankman DOWN note instance 1", false, 24);
        animation_add_prefix(&character->animation, "singUP", "Tankman UP note instance 1", false, 24);
        animation_add_prefix(&character->animation, "singRIGHT", "Tankman Note Left instance 1", false, 24);
        animation_play(&character->animation, "idle");

        move_sprite(character, character->x, character->y + 180);
        set_flip(character, X);
    }

    char path[256];
    sprintf(path, "assets/shared/images/characters/%sOffsets.txt", name);
    FILE* offsets = fopen(path, "r");
    if(!offsets) return character;
    char c[256];
    while(fgets(c, 256, offsets)){
        char abuf[64];
        int x = 0, y = 0;
        sscanf(c, "%63s %i %i", abuf, &x, &y);
        animation_set_offset(&character->animation, abuf, x, y);
    }
    fclose(offsets);

    return character;
}

fnf_sprite* create_icon(float x, float y, const char* name){
    fnf_sprite* icon = make_sprite(x, y, true);

    char path[256];
    sprintf(path, "assets/images/icons/icon-%s.png", name);

    load_sprite(icon, path);
    fnf_animation* animation = &icon->animation.collection->animations[0];
    strcpy(animation->name, "icon");
    icon->animation.collection->animationsSize++;

    animation->frames[0].x = 0;
    animation->frames[0].y = 0;
    animation->frames[0].w = 150;
    animation->frames[0].h = 150;

    animation->frames[1].x = 150;
    animation->frames[1].y = 0;
    animation->frames[1].w = 150;
    animation->frames[1].h = 150;

    animation->framesSize += 2;

    animation_add_prefix(&icon->animation, "idle", "icon", false, 30);
    animation_play(&icon->animation, "idle");
    icon->animation.finished = true;

    return icon;
}

fnf_sprite* create_arrows(){
    fnf_sprite* arrow_base = make_sprite(0, 0, true);
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

    for(int i=0; i<4; i++)
        animation_set_offset(&arrow_base->animation, directions_confirm[i], 20.f, 20.f);
    //fnf_sprite* rarrow = clone_sprite(arrow_base);
    animation_play(&arrow_base->animation, "redScroll");
    //rarrow->animated = false;
    load_sprite(arrow_base, "assets/images/NOTE_assets.png");

    return arrow_base;
}

void create_stage(fnf_stage_t stage, fnf_camera* play_camera) {
    switch(stage){
        case STAGE: {
            fnf_sprite *stage_back = make_sprite(-600, -200, false);
            stage_back->camera = play_camera;
            stage_back->scroll.x = 0.9f;
            stage_back->scroll.x = 0.9f;
            load_sprite(stage_back, "assets/shared/images/stageback.png");

            fnf_sprite *stage_front = make_sprite(-650, 600, false);
            stage_front->camera = play_camera;
            stage_front->scroll.x = 0.9f;
            stage_front->scroll.x = 0.9f;
            //scale_sprite(stage_front, 1.1f, 1.0f);
            load_sprite(stage_front, "assets/shared/images/stagefront.png");
            resize_sprite(stage_front, stage_front->w * 1.1, stage_front->h);

            fnf_sprite *stage_curtains = make_sprite(-500, -300, false);
            stage_curtains->camera = play_camera;
            stage_curtains->scroll.x = 1.3f;
            stage_curtains->scroll.y = 1.3f;
            scale_sprite(stage_curtains, 0.9f, 1.0f);
            load_sprite(stage_curtains, "assets/shared/images/stagecurtains.png");
            resize_sprite(stage_curtains, stage_curtains->w * 0.9, stage_curtains->h);

            add_sprite(stage_back);
            add_sprite(stage_front);
            add_sprite(stage_curtains);
            break;
        }
        case SPOOKY: {
            fnf_sprite *halloween_bg = make_sprite(-200, -100, true);
            halloween_bg->camera = play_camera;

            load_sprite(halloween_bg, "assets/week2/images/halloween_bg.png");
            animation_load_atlas(&halloween_bg->animation, "assets/week2/images/halloween_bg.xml");

            animation_add_prefix(&halloween_bg->animation, "idle", "halloweem bg0", false, 30);
            animation_add_prefix(&halloween_bg->animation, "lightning", "halloweem bg lightning strike", false, 24);
            animation_play(&halloween_bg->animation, "idle");

            add_sprite(halloween_bg);
            break;
        }
        case PHILLY: {
            fnf_sprite *bg = make_sprite(-100, 0, false);
            load_sprite(bg, "assets/week3/images/philly/sky.png");
            bg->scroll = (fnf_vector) {0.1f, 0.1f};
            add_sprite(bg);

            fnf_sprite *city = make_sprite(-10, 0, false);
            load_sprite(city, "assets/week3/images/philly/city.png");
            city->scroll = (fnf_vector) {0.3f, 0.3f};
            resize_sprite(city, city->w * 0.85f, city->h);
            add_sprite(city);

            fnf_sprite *street_behind = make_sprite(-40, 50, false);
            load_sprite(street_behind, "assets/week3/images/philly/behindTrain.png");
            add_sprite(street_behind);

            //fnf_sprite*	philly_train = make_sprite(2000, 360, false);
            //load_sprite(philly_train, "assets/week3/images/philly/train.png");
            //add_sprite(philly_train);

            // var cityLights:FlxSprite = new FlxSprite().loadGraphic(AssetPaths.win0.png);

            fnf_sprite *street = make_sprite(-40, street_behind->y, false);
            load_sprite(street, "assets/week3/images/philly/street.png");
            add_sprite(street);

            play_camera->zoom = 1.05f;
            bg->camera = play_camera;
            city->camera = play_camera;
            street_behind->camera = play_camera;
            //philly_train->camera = play_camera;
            street->camera = play_camera;
            break;
        }
        case LIMO: {
            play_camera->zoom = 0.9f;
            fnf_sprite *sky = make_sprite(-120, -50, false);
            sky->scroll = (fnf_vector) {0.1f, 0.1f};

            load_sprite(sky, "assets/week4/images/limo/limoSunset.png");
            add_sprite(sky);

            fnf_sprite *limo = make_sprite(-200, 480, true);
            limo->scroll = (fnf_vector) {0.4f, 0.4f};

            load_sprite(limo, "assets/week4/images/limo/bgLimo.png");
            animation_load_atlas(&limo->animation, "assets/week4/images/limo/bgLimo.xml");
            animation_add_prefix(&limo->animation, "drive", "background limo pink", true, 24);
            animation_play(&limo->animation, "drive");
            add_sprite(limo);

            sky->camera = play_camera;
            limo->camera = play_camera;
            break;
        }
        case MALL: {
            fnf_sprite *bg = make_sprite(-1000, -500, false);
            load_sprite(bg, "assets/week5/images/christmas/bgWalls.png");
            bg->scroll = (fnf_vector) {0.2f, 0.2f};
            resize_sprite(bg, bg->w * 0.8f, bg->h);
            add_sprite(bg);

            fnf_sprite *upper = make_sprite(-240, -90, true);
            load_sprite(upper, "assets/week5/images/christmas/upperBop.png");
            animation_load_atlas(&upper->animation, "assets/week5/images/christmas/upperBop.xml");
            animation_add_prefix(&upper->animation, "bop", "Upper Crowd Bob", true, 24);
            animation_play(&upper->animation, "bop");
            upper->scroll = (fnf_vector) {0.33f, 0.33f};
            resize_sprite(upper, upper->w * 0.85f, upper->h);
            add_sprite(upper);

            fnf_sprite *escalator = make_sprite(-1100, -600, false);
            load_sprite(escalator, "assets/week5/images/christmas/bgEscalator.png");
            escalator->scroll = (fnf_vector) {0.3f, 0.3f};
            resize_sprite(escalator, escalator->w * 0.9f, escalator->h);
            add_sprite(escalator);

            fnf_sprite *tree = make_sprite(370, -250, false);
            load_sprite(tree, "assets/week5/images/christmas/christmasTree.png");
            tree->scroll = (fnf_vector) {0.4f, 0.4f};
            add_sprite(tree);

            fnf_sprite *bottom = make_sprite(-300, 140, true);
            load_sprite(bottom, "assets/week5/images/christmas/bottomBop.png");
            animation_load_atlas(&bottom->animation, "assets/week5/images/christmas/bottomBop.xml");
            animation_add_prefix(&bottom->animation, "bop", "Bottom Level Boppers", true, 24);
            animation_play(&bottom->animation, "bop");
            bottom->scroll = (fnf_vector) {0.9f, 0.9f};
            add_sprite(bottom);

            fnf_sprite *snow = make_sprite(-600, 700, false);
            load_sprite(snow, "assets/week5/images/christmas/fgSnow.png");
            snow->scroll = (fnf_vector) {0.4f, 0.4f};
            add_sprite(snow);

            fnf_sprite *santa = make_sprite(-840, 150, true);
            load_sprite(santa, "assets/week5/images/christmas/santa.png");
            animation_load_atlas(&santa->animation, "assets/week5/images/christmas/santa.xml");
            animation_add_prefix(&santa->animation, "bop", "santa idle in fear", true, 24);
            animation_play(&santa->animation, "bop");
            add_sprite(santa);

            play_camera->zoom = 0.8f;

            bg->camera = play_camera;
            upper->camera = play_camera;
            escalator->camera = play_camera;
            tree->camera = play_camera;
            bottom->camera = play_camera;
            snow->camera = play_camera;
            santa->camera = play_camera;
            break;
        }
        case SCHOOL: {
            break;
        }
        case TANK: {
            break;
        }
    }
}