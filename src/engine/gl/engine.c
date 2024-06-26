#include "engine.h"
#include "saves.h"
#include "sprite.h"
#include "states.h"
#include "engine.h"
#include "audio.h"
#include "font.h"
#include "conductor.h"
#include "controls.h"
#include "glad/glad.h"
#include "fnf_data.h"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <unistd.h>

uint32 frames_elapsed = 0;

bool keys[255];
bool justkeyed[255];

uint32 current_frame(){
    return frames_elapsed;
}

void keypress(uint32 key, bool press){
    fnf_key_t keynum;
    switch(key){
        case SDLK_UP: 
            keynum = UP;
            break;
        case SDLK_DOWN: 
            keynum = DOWN;
            break;
        case SDLK_LEFT: 
            keynum = LEFT;
            break;
        case SDLK_RIGHT: 
            keynum = RIGHT;
            break;
        case SDLK_RETURN: 
            keynum = ENTER;
            break;
        case SDLK_ESCAPE: 
            keynum = BACK;
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

static uint32_t dt;

void start_fnfc(){
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_JOYSTICK);
    SDL_Window* win = SDL_CreateWindow("Friday Night Funkin'", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    SDL_JoystickEventState(SDL_ENABLE);
    SDL_JoystickOpen(0);

    chdir("");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 3);

    SDL_GLContext cont = SDL_GL_CreateContext(win);
    SDL_GL_MakeCurrent(win, cont);

    gladLoadGL();

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_BLEND);

    switch_state(&title_state);
    bool close = true;

    init_audio();
    init_font();
    init_saves();
    set_highscore(week_songs[2][1], 9999);
    printf("fresh highscore: %i\n", get_highscore(week_songs[2][1]));
    fnf_conductor_t conductor;
    init_conductor(&conductor);
    current_conductor = &conductor;

    while(close){
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_KEYDOWN:
                    if(!e.key.repeat)
                        keypress(e.key.keysym.sym, true);
                    //printf( "Key press detected %i\n", (char)e.key.keysym.sym );
                    break;
                case SDL_KEYUP:
                    keypress(e.key.keysym.sym, false);
                    //printf( "Key release detected\n" );
                    break;
                case SDL_JOYBUTTONDOWN:
                    switch(e.jbutton.button){
                        case 0:
                            keypress(SDLK_RIGHT, true);
                            break;
                        case 1:
                            keypress(SDLK_DOWN, true);
                            break;
                        case 2:
                            keypress(SDLK_UP, true);
                            break;
                        case 3:
                            keypress(SDLK_LEFT, true);
                            break;
                        case 15:
                            keypress(SDLK_RETURN, true);
                            break;
                        case 10:
                            close = false;
                            break;
                    }
                    //keypress(e.key.keysym.sym, true);
                    //printf( "Key press detected %i\n", (char)e.key.keysym.sym );
                    break;
                case SDL_JOYBUTTONUP:
                    switch(e.jbutton.button){
                        case 0:
                            keypress(SDLK_RIGHT, false);
                            break;
                        case 1:
                            keypress(SDLK_DOWN, false);
                            break;
                        case 2:
                            keypress(SDLK_UP, false);
                            break;
                        case 3:
                            keypress(SDLK_LEFT, false);
                            break;
                        case 15:
                            keypress(SDLK_RETURN, false);
                            break;
                        case 10:
                            close = false;
                            break;
                    }
                    //keypress(e.key.keysym.sym, false);
                    //printf( "Key release detected\n" );
                    break;
                case SDL_QUIT:
                    close = false;
                    break;
            }
        }
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.f,0.f,0.f,1.f);

        if(get_nextState())
            _switchState();

        //printf("%f\n",conductor.songPosition);
        uint32_t start = SDL_GetTicks();
        get_curState()->draw();
        SDL_GL_SwapWindow(win);

        uint32_t end = SDL_GetTicks();
        dt = end - start;

        if (dt < (1000 / 60))
            SDL_Delay((1000 / 60) - dt);
        frames_elapsed++;
    }
    printf("Errors: %d\n", glGetError());
    
    get_curState()->uninit();
    uninit_audio();
    uninit_saves();

    SDL_DestroyWindow(win);
    SDL_Quit();
}

inline uint32 get_delta(){
    return dt;
}

inline uint32 get_ticks(){
    return SDL_GetTicks();
}