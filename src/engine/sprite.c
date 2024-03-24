#include "sprite.h"
#include "glad/glad.h"
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "cglm/mat4.h"
#include "cglm/cglm.h"

#include <GL/gl.h>
#include <malloc.h>
#include <string.h>
#include <SDL2/SDL.h>
//#include <png.h>

bool init = false;
mat4 ortho;

void generateVertices(float* vertices, float x, float y, float w, float h){
    float z = 0.0f;

    vertices[0] = x + w;
    vertices[1] = y;
    vertices[2] = z;

    vertices[5] = x + w;
    vertices[6] = y + h;
    vertices[7] = z;

    vertices[10] = x;
    vertices[11] = y + h;
    vertices[12] = z;

    vertices[15] = x;
    vertices[16] = y;
    vertices[17] = z;
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

void _init_sprite(fnf_sprite* sprite){
    static uint32 indices[] = {0, 1, 3, 1, 2, 3};
    generateVertices(sprite->gl.vertices, 0, 0, sprite->w, sprite->h);
    generate2DVertices(sprite->gl.vertices, 0, 0, sprite->w, sprite->h);

    glGenVertexArrays(1, &sprite->gl.VAO); 
    glGenBuffers(1, &sprite->gl.VBO);
    glGenBuffers(1, &sprite->gl.EBO);  

    glBindVertexArray(sprite->gl.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, sprite->gl.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sprite->gl.vertices), sprite->gl.vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->gl.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(0);
}

fnf_sprite* make_sprite(float x, float y, bool animated) {
    glm_ortho(0.0f, SCREEN_WIDTHF, SCREEN_HEIGHTF, 0.0f, -1000.0f, 1000.0f, ortho);

    fnf_sprite* sprite = FNF_NEW(fnf_sprite);
    FNF_ZERO(sprite, sizeof(fnf_sprite));

    sprite->x = x;
    sprite->y = y;
    sprite->animated = animated;
    sprite->enabled = true;
    sprite->offset = (fnf_vector){0.0f, 0.0f};
    sprite->scroll = (fnf_vector){1.0f, 1.0f};

    if(animated){
        fnf_animation_collection* colptr = FNF_NEW(fnf_animation_collection);

        sprite->animation.collection = colptr;
        FNF_ZERO(colptr, sizeof(fnf_animation_collection));
    }

    sprite->color = 0xFFFFFFFF;
    sprite->shader = create_shader();

    glGenTextures(1, &sprite->graphic.spr);
    glUseProgram(sprite->shader.program);

    glUniform2f(glGetUniformLocation(sprite->shader.program, "ScreenResolution"), SCREEN_WIDTHF, SCREEN_HEIGHTF);
    glUniformMatrix4fv(glGetUniformLocation(sprite->shader.program, "projection"), 1, false, (float*)ortho);

    return sprite;
}

static const char* SHAPE_SHADER = glsl(
    uniform sampler2D bitmap;
    uniform vec2 ScreenResolution;

    out vec4 FragColor;
    in vec2 fragCoord;
    in vec3 SColor;

    void main() {    
        vec2 uv = fragCoord;
        FragColor = vec4(SColor, 1.0);
    }
);

fnf_sprite* create_shape(fnf_sprite* sprite){
    sprite->shader = create_shader_text(0, SHAPE_SHADER);
    glUseProgram(sprite->shader.program);
    glUniformMatrix4fv(glGetUniformLocation(sprite->shader.program, "projection"), 1, false, (float*)ortho);
    sprite->graphic.w = sprite->w;
    sprite->graphic.h = sprite->h;

    _init_sprite(sprite);

    scale_sprite(sprite, 1.f, 1.f);    
    resize_sprite(sprite, sprite->w, sprite->h);

    return sprite;
}

fnf_sprite* set_color(fnf_sprite* sprite, uint32_t color){
    sprite->color = color;
    
    glUseProgram(sprite->shader.program);
    glUniform3f(glGetUniformLocation(sprite->shader.program, "color"), sprite->r / 255.f, sprite->g / 255.f, sprite->b / 255.f);

    return sprite;
}

void delete_sprite(fnf_sprite* sprite) {
    glDeleteProgram(sprite->shader.program);
    glDeleteTextures(1, &sprite->graphic.spr);

    glDeleteVertexArrays(1, &sprite->gl.VAO);
    glDeleteBuffers(1, &sprite->gl.VBO);
    glDeleteBuffers(1, &sprite->gl.EBO);

    //if(sprite->animation)
    //    FNF_DELETE(sprite->animation);
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
    glGenerateMipmap(GL_TEXTURE_2D);
    //glUniform1i(glGetUniformLocation(sprite->shader.program, "bitmap"), sprite->spr);
    /*
    for (int y = 0; y < sprite->h; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    */
    _init_sprite(sprite);

    stbi_image_free(row_pointers);

    scale_sprite(sprite, 1.f, 1.f);    
    resize_sprite(sprite, sprite->w, sprite->h);
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

    generateVertices(sprite->gl.vertices, 0, 0, w, h);
    glBindBuffer(GL_ARRAY_BUFFER, sprite->gl.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(sprite->gl.vertices), sprite->gl.vertices);

    return move_sprite(sprite, sprite->x, sprite->y);
}

fnf_sprite* scale_sprite(fnf_sprite* sprite, float x, float y){
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_scale(model, (vec3){x, y, 0.f});

    sprite->scale.x = x;
    sprite->scale.y = y;

    glUniformMatrix4fv(glGetUniformLocation(sprite->shader.program, "model"), 1, false, (float*)model);
    return sprite;
}

fnf_sprite* move_sprite(fnf_sprite* sprite, float x, float y)
{
    sprite->x = x;// - (sprite->w);
    sprite->y = y;//; - (sprite->h);

    mat4 view = GLM_MAT4_IDENTITY_INIT;
    glm_translate(view, (vec3){sprite->x + sprite->offset.x, sprite->y + sprite->offset.y, 0.0f});

    glUniformMatrix4fv(glGetUniformLocation(sprite->shader.program, "view"), 1, false, (float*)view);

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
    sprite->graphic.w = frame->w + frame->fw;
    sprite->graphic.h = frame->h + frame->fh;
    generateVertices(sprite->gl.vertices, 0, 0, (float)frame->fw, (float)frame->fh);
    generate2DVertices(sprite->gl.vertices, frame->x, frame->y, frame->w, frame->h);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(sprite->gl.vertices), sprite->gl.vertices);

    uint32 dt = SDL_GetTicks() - sprite->animation.lastFrame;

    if(!sprite->animation.finished){
        sprite->animation.frameNum++; //= ((SDL_GetTicks() / (1000 / sprite->animation.currentAnimation->fps)) % sprite->animation.currentAnimation->playlistSize + 1);//++;
        //sprite->animation.frameNum = (int)floor(sprite->animation.currentAnimation->fps * SDL_GetTicks() / 1000) % sprite->animation.currentAnimation->playlistSize;
        sprite->animation.lastFrame = SDL_GetTicks();
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

    glBindVertexArray(sprite->gl.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, sprite->gl.VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->gl.EBO);

    glUseProgram(sprite->shader.program);

    if(sprite->animated)
        handle_animations(sprite);

    static float xpos = 0.0f, ypos = 0.0f;
    mat4 cam = GLM_MAT4_IDENTITY_INIT;

    xpos = sprite->x;
    ypos = sprite->y;

    move_sprite(sprite, xpos, ypos);

    scale_sprite(sprite, sprite->scale.x, sprite->scale.y);

    if(sprite->camera){
        float camx = (sprite->camera->x * sprite->scroll.x) - SCREEN_WIDTHF * 0.5f;
        float camy = (sprite->camera->y * sprite->scroll.y) - SCREEN_HEIGHTF * 0.5f;
        camx *= sprite->camera->zoom;
        camy *= sprite->camera->zoom;

        glm_scale(cam, (vec3){sprite->camera->zoom, sprite->camera->zoom, 0.0f});
        glm_translate(cam, (vec3){camx, camy, 0.0f});

        mat4 view = GLM_MAT4_IDENTITY_INIT;
        glm_translate(view, (vec3){(sprite->x + sprite->offset.x) * sprite->scroll.x * sprite->camera->zoom, (sprite->y + sprite->offset.y) * sprite->scroll.y * sprite->camera->zoom, 0.0f});

        glUniformMatrix4fv(glGetUniformLocation(sprite->shader.program, "view"), 1, false, (float*)view);
    }

    glUniformMatrix4fv(glGetUniformLocation(sprite->shader.program, "camera"), 1, false, (float*)cam);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}