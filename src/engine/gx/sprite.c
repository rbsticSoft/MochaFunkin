#include "sprite.h"

fnf_sprite* make_sprite(float x, float y, bool animated){}
fnf_sprite* load_sprite(fnf_sprite* sprite, const char* path){}
fnf_sprite* clone_sprite(fnf_sprite* sprite){}
/**
 * Resizes the rendering size of the sprite.
 * @note THIS DOES NOT CHANGE THE ``w`` AND ``h`` MEMBERS OF THE STRUCT!
*/
fnf_sprite* resize_sprite(fnf_sprite* sprite, uint16 w, uint16 h){}
fnf_sprite* scale_sprite(fnf_sprite* sprite, float x, float y){}
fnf_sprite* move_sprite(fnf_sprite* sprite, float x, float y){}
fnf_sprite* center_sprite(fnf_sprite* sprite, fnf_axis_t axis){}
fnf_sprite* create_shape(fnf_sprite* sprite){}
fnf_sprite* set_color(fnf_sprite* sprite, uint32_t color){}
fnf_sprite* set_alpha(fnf_sprite* sprite, float alpha){}
fnf_sprite* set_flip(fnf_sprite* sprite, fnf_axis_t axis){}

void draw_sprite(fnf_sprite *sprite){}
void delete_sprite(fnf_sprite *sprite){}