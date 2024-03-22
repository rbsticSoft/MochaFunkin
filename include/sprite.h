#ifndef SPRITE_H
#define SPRITE_H
#include "types.h"
#include "shader.h"
#include "animation.h"

typedef struct {
    float x,y;
} fnf_vector;

typedef struct {
    float x,y,w,h;
} fnf_rect;

typedef enum {
    X,
    Y,
    XY
} fnf_axis_t;

typedef struct {
    float x,y;
    int32 w,h;
    struct {
        uint32 spr;
        int32 w,h;
    } graphic;
    union {
        struct {
            uint8 r,g,b,a;
        };
        uint32 color;
    };
    struct {
        unsigned int VAO,VBO,EBO;
        
        float vertices[20];
    } gl;
    fnf_vector scale, offset;
    fnf_rect clip;
    fnf_shader shader;
    bool animated, enabled;
    fnf_animation_controller animation;
} fnf_sprite;

fnf_sprite* make_sprite(float x, float y, bool animated);
fnf_sprite* load_sprite(fnf_sprite* sprite, const char* path);
fnf_sprite* clone_sprite(fnf_sprite* sprite);
/**
 * Resizes the rendering size of the sprite.
 * @note THIS DOES NOT CHANGE THE ``w`` AND ``h`` MEMBERS OF THE STRUCT!
*/
fnf_sprite* resize_sprite(fnf_sprite* sprite, uint16 w, uint16 h);
fnf_sprite* scale_sprite(fnf_sprite* sprite, float x, float y);
fnf_sprite* move_sprite(fnf_sprite* sprite, float x, float y);
fnf_sprite* center_sprite(fnf_sprite* sprite, fnf_axis_t axis);
void shit(fnf_sprite * sprite);
void draw_sprite(fnf_sprite *sprite);
void delete_sprite(fnf_sprite *sprite);
#endif // SPRITE_H