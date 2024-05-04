#ifndef SAVES_H
#define SAVES_H

void init_saves();
void uninit_saves();
void set_highscore(char* song, uint32_t score);
uint32_t get_highscore(char* song);

#endif //SAVES_H
