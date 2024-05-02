#include "sprite.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <GL/gl.h>
#include <malloc.h>
#include <string.h>
#include <SDL2/SDL.h>
//#include <png.h>

bool init = false;

void generate_vertices(float x, float y, float w, float h){
    glColor3f( 1, 1, 1 );
    glVertex2f(x + w, y);

    glVertex2f(x + w, y + h);

    glVertex2f(x, y + h);

    glVertex2f( x, y );

    glTexCoord2f(0, 1);
    glTexCoord2f(1, 1);
    glTexCoord2f(1, 0);
    glTexCoord2f(0, 0);
}

void generate2DVertices(float* vertices, float x, float y, float w, float h){
    vertices[3] = x + w;
    vertices[4] = y;

    vertices[8] = x + w;
    vertices[9] = y + h;

    vertices[13] = x;
    vertices[14] = y + h;

    vertices[18] = x;
    vertices[19] = y;
}

fnf_sprite* make_sprite(float x, float y, bool animated) {
    fnf_sprite* sprite = FNF_NEW(fnf_sprite);
    FNF_ZERO(sprite, sizeof(fnf_sprite));

    sprite->x = x;
    sprite->y = y;
    sprite->animated = animated;
    sprite->enabled = true;
    sprite->offset = (fnf_vector){0.0f, 0.0f};
    sprite->scroll = (fnf_vector){1.0f, 1.0f};
    sprite->flip = NONE;

    if(animated){
        fnf_animation_collection* colptr = FNF_NEW(fnf_animation_collection);

        sprite->animation.collection = colptr;
        FNF_ZERO(colptr, sizeof(fnf_animation_collection));
    }

    sprite->color = 0xFFFFFFFF;
    sprite->shader.shared = false;

    glGenTextures(1, &sprite->graphic.spr);
    set_alpha(sprite, 1.0f);

    return sprite;
}

fnf_sprite* create_shape(fnf_sprite* sprite){
    sprite->graphic.w = sprite->w;
    sprite->graphic.h = sprite->h;

    scale_sprite(sprite, 1.f, 1.f);    
    resize_sprite(sprite, sprite->w, sprite->h);

    return sprite;
}

fnf_sprite* set_color(fnf_sprite* sprite, uint32_t color){
    sprite->color = color;

    return sprite;
}

fnf_sprite* set_alpha(fnf_sprite* sprite, float alpha){
    sprite->a = alpha;
    return sprite;
}

fnf_sprite* set_flip(fnf_sprite* sprite, fnf_axis_t axis){
    sprite->flip = axis;
    return sprite;
}

void delete_sprite(fnf_sprite* sprite) {
    glDeleteTextures(1, &sprite->graphic.spr);

    if(sprite->animated)
        FNF_DELETE(sprite->animation.collection);
    FNF_DELETE(sprite);
}

fnf_sprite *load_sprite(fnf_sprite *sprite, const char *path)
{
    int c;
    void* row_pointers = stbi_load(path, &sprite->w, &sprite->h, &c, 0);
    if(!row_pointers)
        printf("%s does not exist\n", path);
    int format = c == 3 ? GL_RGB : GL_RGBA;

    sprite->graphic.w = sprite->w;
    sprite->graphic.h = sprite->h;

    glBindTexture(GL_TEXTURE_2D, sprite->graphic.spr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, format, sprite->w, sprite->h, 0, format, GL_UNSIGNED_BYTE, row_pointers);

    stbi_image_free(row_pointers);

    resize_sprite(sprite, sprite->w, sprite->h);
    scale_sprite(sprite, 1.f, 1.f);    
    return sprite;
}

fnf_sprite* clone_sprite(fnf_sprite* sprite){
    fnf_sprite* target = FNF_NEW(fnf_sprite);

    memcpy(target, sprite, sizeof(fnf_sprite));

    return target;
}

fnf_sprite* resize_sprite(fnf_sprite* sprite, uint16 w, uint16 h){
    sprite->w = w;
    sprite->h = h;

    return move_sprite(sprite, sprite->x, sprite->y);
}

fnf_sprite* scale_sprite(fnf_sprite* sprite, float x, float y){
    sprite->scale.x = x;
    sprite->scale.y = y;
    return sprite;
}

fnf_sprite* move_sprite(fnf_sprite* sprite, float x, float y)
{
    sprite->x = x;// - (sprite->w);
    sprite->y = y;//; - (sprite->h);

    return sprite;
}

fnf_sprite* center_sprite(fnf_sprite* sprite, fnf_axis_t axis){
    if(axis == XY)
        return move_sprite(sprite, (SCREEN_WIDTHF - sprite->graphic.w) / 2.f, (SCREEN_HEIGHTF - sprite->graphic.h) / 2.f);

    if(axis == X)
        return move_sprite(sprite, (SCREEN_WIDTHF - sprite->graphic.w) / 2.f, sprite->y);
        
    if(axis == Y)
        return move_sprite(sprite, sprite->x, (SCREEN_HEIGHTF - sprite->graphic.h) / 2.f);

    return sprite;
}

#include "engine.h"
#include <SDL2/SDL.h>

void handle_animations(fnf_sprite* sprite){
    if(!sprite->animation.currentAnimation)
        return;

    sprite->offset = sprite->animation.currentAnimation->offset;
    if(sprite->flip == X)
        sprite->offset.x = sprite->offset.x;
    int32 playlistNum = sprite->animation.currentAnimation->playlist[sprite->animation.frameNum];
    if(playlistNum == -1)
    {
        sprite->animation.finished = true;
        if(sprite->animation.currentAnimation->looped){
            sprite->animation.frameNum = 1;
            sprite->animation.finished = false;
        }
        playlistNum = sprite->animation.currentAnimation->playlist[sprite->animation.frameNum - 1];
    }
    //if(strcmp(sprite->animation.currentAnimation->name, "gf dance l")==0)
    //printf("Anim: %s\nPlaylist number: %i\n", sprite->animation.currentAnimation->name, playlistNum);
    fnf_frame* frame = &sprite->animation.currentAnimation->animation->frames[playlistNum];

    if(!frame->fw){
        frame->fx = 0;
        frame->fy = 0;
        frame->fw = frame->w;
        frame->fh = frame->h;
    }
    //float offsetX = (sprite->x + frame->fx);
    //float offsetY = (sprite->y + frame->fy);
    sprite->graphic.w = frame->w;
    sprite->graphic.h = frame->h;

    generate2DVertices(sprite->gl.vertices, frame->x, frame->y, frame->w, frame->h);

    if(!sprite->animation.finished){
        sprite->animation.frameNum = ((get_ticks() - sprite->animation.lastFrame) * (sprite->animation.currentAnimation->fps) / 1000) % sprite->animation.currentAnimation->playlistSize + 1; //= ((SDL_GetTicks() / (1000 / sprite->animation.currentAnimation->fps)) % sprite->animation.currentAnimation->playlistSize + 1);//++;
        //sprite->animation.frameNum = (int)floor(sprite->animation.currentAnimation->fps * SDL_GetTicks() / 1000) % sprite->animation.currentAnimation->playlistSize;
    }
}

bool screen_intersect(fnf_sprite* sprite){
    int Amin, Amax, Bmin, Bmax;
    Amin = sprite->x;
    Amax = Amin + sprite->w;
    Bmin = 0;
    Bmax = Bmin + SCREEN_WIDTH;
    if (Bmin > Amin)
        Amin = Bmin;
    if (Bmax < Amax)
        Amax = Bmax;
    if (Amax <= Amin)
        return false;

    /* Vertical intersection */
    Amin = sprite->y;
    Amax = Amin + sprite->h;
    Bmin = 0;
    Bmax = Bmin + SCREEN_HEIGHT;
    if (Bmin > Amin)
        Amin = Bmin;
    if (Bmax < Amax)
        Amax = Bmax;
    if (Amax <= Amin)
        return false;
    return true;
}

void draw_sprite(fnf_sprite* sprite) {
    if(sprite == 0) return;
    if(!sprite->enabled) return;
    if(!screen_intersect(sprite)) return;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sprite->graphic.spr);

    if(sprite->animated)
        handle_animations(sprite);

    static float xpos = 0.0f, ypos = 0.0f;
    xpos = sprite->x;
    ypos = sprite->y;

    // this interferes with something, but i am not sure what that thing is
    //if(sprite->shader.shared){
        move_sprite(sprite, xpos, ypos);
        scale_sprite(sprite, sprite->scale.x, sprite->scale.y);
    //}

    if(sprite->camera){
        float camx =  (SCREEN_WIDTHF * 0.5f);
        float camy = (SCREEN_HEIGHTF * 0.5f);
        camx -= (sprite->camera->x);
        camy -= (sprite->camera->y);
        camx /= sprite->scale.x;
        camy /= sprite->scale.y;
        camx *= sprite->scroll.x;
        camy *= sprite->scroll.y;
        //camx *= sprite->camera->zoom;
        //camy *= sprite->camera->zoom;


        float flip = sprite->flip == X ? sprite->graphic.w : 0.0f;
        camx = ((sprite->x + sprite->offset.x + flip)) * sprite->camera->zoom;
        camy = ((sprite->y + sprite->offset.y)) * sprite->camera->zoom;
    }

    glBegin(GL_QUADS);
    generate_vertices(sprite->x, sprite->y, sprite->w, sprite->h);
    glEnd();
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}