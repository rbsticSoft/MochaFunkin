#include "engine.h"
#include "sprite.h"
#include "states.h"
#include "engine.h"
#include "audio.h"
#include "font.h"
#include "conductor.h"
#include "controls.h"
#include "glad/glad.h"
#include <assert.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

uint32 frames_elapsed = 0;

bool keys[255];
bool justkeyed[255];

uint32 current_frame(){
    return frames_elapsed;
}

void keypress(uint32 key, bool press){
    uint8_t keynum;
    switch(key){
        case SDLK_UP: 
            keynum = 2;
            break;
        case SDLK_DOWN: 
            keynum = 1;
            break;
        case SDLK_LEFT: 
            keynum = 0;
            break;
        case SDLK_RIGHT: 
            keynum = 3;
            break;
    }

    keys[keynum] = press;
    justkeyed[keynum] = press;
}

bool key_pressed(fnf_key_t key){
    return keys[key];
}

bool key_just_pressed(fnf_key_t key){
    bool keyed = justkeyed[key];
    if(keyed)
        justkeyed[key] = false;
    return keyed;
}

void start_fnfc(){
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
    SDL_Window* win = SDL_CreateWindow("Friday Night Funkin'", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 3);

    SDL_GL_CreateContext(win);

    gladLoadGL();

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    switch_state(&title_state);

    bool close = true;

    init_audio();
    init_font();

    fnf_conductor_t conductor;
    init_conductor(&conductor);
    current_conductor = &conductor;

    while(close){
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            switch(e.type){
                SDLK_UP;
                case SDL_KEYDOWN:
                    if(!e.key.repeat)
                        keypress(e.key.keysym.sym, true);
                    //printf( "Key press detected %i\n", (char)e.key.keysym.sym );
                    break;
                case SDL_KEYUP:
                    keypress(e.key.keysym.sym, false);
                    //printf( "Key release detected\n" );
                    break;
                case SDL_QUIT:
                    close = false;
                    break;
            }
        }
        glClearColor(0.f,0.f,0.f,1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        if(get_nextState())
            _switchState();

        //printf("%f\n",conductor.songPosition);
        get_curState()->draw();
        SDL_GL_SwapWindow(win);
        SDL_Delay(1000 / 60);
        frames_elapsed++;
    }
    printf("Errors: %d\n", glGetError());
    
    uninit_audio();
    SDL_DestroyWindow(win);
    SDL_Quit();
}