#ifndef TIMER_H
#define TIMER_H
#include "types.h"

typedef struct {
    bool completed;
    float elapsed;
    uint32_t target;
} fnf_timer;

fnf_timer timer_start(uint32_t duration);
fnf_timer* timer_step(fnf_timer* timer);

#endif //TIMER_H
