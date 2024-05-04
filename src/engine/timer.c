#include "timer.h"
#include "engine.h"

fnf_timer timer_start(uint32_t duration){
    fnf_timer timer = {
            .completed = false,
            .elapsed = 0.0f,
            .target = duration
    };

    return timer;
}

fnf_timer* timer_step(fnf_timer* timer){
    timer->elapsed += 16.67f;//get_delta();
    timer->completed = timer->elapsed > timer->target;

    return timer;
}