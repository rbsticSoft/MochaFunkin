#ifndef FONT_H
#define FONT_H

typedef struct {
    
} fnf_font;

void init_font();
void draw_text(const unsigned char* text, float x, float y, float scale);
void draw_alphabet(const unsigned char* text, float x, float y, float alpha);

#endif