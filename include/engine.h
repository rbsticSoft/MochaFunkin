#ifndef ENGINE_H
#define ENGINE_H
#include "types.h"

void init_fnfc();
void start_fnfc();
void init_render_loop();
uint32 get_delta();
uint32 get_ticks();
uint32 current_frame();

#endif // ENGINE_H