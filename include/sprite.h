#ifndef SPRITE_H
#define SPRITE_H
#include "types.h"
#include "shader.h"
#include "animation.h"
#include "camera.h"

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
    float alpha;
    struct {
        unsigned int VAO,VBO,EBO;
        float vertices[20];
    } gl;
    fnf_vector scale, offset, scroll;
    fnf_rect clip;
    fnf_shader shader;
    fnf_camera* camera;
    fnf_axis_t flip;
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
fnf_sprite* create_shape(fnf_sprite* sprite);
fnf_sprite* set_color(fnf_sprite* sprite, uint32_t color);
fnf_sprite* set_alpha(fnf_sprite* sprite, float alpha);
fnf_sprite* set_flip(fnf_sprite* sprite, fnf_axis_t axis);

void draw_sprite(fnf_sprite *sprite);
void delete_sprite(fnf_sprite *sprite);
#endif // SPRITE_H