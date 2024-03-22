#include "font.h"
#include "sprite.h"
#include "cglm/mat4.h"
#include "cglm/cglm.h"
#include "glad/glad.h"

#include <ft2build.h>
#include FT_FREETYPE_H  
#include <GL/gl.h>
#include <ctype.h>

int oglc;

#define checkogl() oglc = glGetError(); if(oglc) printf("Errors: %d\n", oglc);

typedef struct {
    unsigned int id;  // ID handle of the glyph texture
    fnf_vector   size;       // Size of glyph
    fnf_vector   bearing;    // Offset from baseline to left/top of glyph
    unsigned int advance;    // Offset to advance to next glyph
} glyph;

glyph glyphs[256];
fnf_shader shader;
mat4 orthographic;
unsigned int VAO, VBO;// <vec2 pos, vec2 tex>

const char* vfont = glsl(
    layout (location = 0) in vec4 vertex;
    out vec2 TexCoords;

    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
        TexCoords = vertex.zw;
    }  
);

const char* ffont = glsl(
    in vec2 TexCoords;
    out vec4 color;

    uniform sampler2D text;
    uniform vec3 textColor;

    void main()
    {    
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
        color = vec4(textColor, 1.0) * sampled;
    }  
);

fnf_sprite* alphabet_base;

void init_alphabet(){
    alphabet_base = make_sprite(0, 0, true);
    load_sprite(alphabet_base, "assets/images/alphabet.png");
    animation_load_atlas(&alphabet_base->animation, "assets/images/alphabet.xml");

    char anim[6];
    for(char i = 'A'; i < 'Z'+1; i++){
        sprintf(anim, "%c bold", i);
        char prefix[2];
        prefix[0] = i;
        prefix[1] = 0x0;
        animation_add_prefix(&alphabet_base->animation, prefix, anim, true, 24);
    }
}

void init_font(){    
    init_alphabet();
    //glEnable(GL_CULL_FACE);

    shader = create_shader_text(vfont, ffont);
    glUseProgram(shader.program);
    glm_ortho(0.0f, SCREEN_WIDTHF, 0.0f, SCREEN_HEIGHTF, -1000.0f, 1000.0f, orthographic);

    glUniformMatrix4fv(glGetUniformLocation(shader.program, "projection"), 1, false, (float*)orthographic);

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
            printf("ERROR::FREETYTPE: Failed to load Glyph\n");
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, "assets/vcr.ttf", 0, &face))
    {
            printf("ERROR::FREETYTPE: Failed to load Glyph\n");
        return;
    }
    FT_Set_Pixel_Sizes(face, 0, 16);  
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
    
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
            printf("ERROR::FREETYTPE: Failed to load Glyph\n");
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        glyphs[c].id = texture;
        glyphs[c].size = (fnf_vector){face->glyph->bitmap.width, face->glyph->bitmap.rows};
        glyphs[c].bearing = (fnf_vector){face->glyph->bitmap_left, face->glyph->bitmap_top};
        glyphs[c].advance = face->glyph->advance.x;
        checkogl()

    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    //glDisable(GL_CULL_FACE);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void draw_text(const unsigned char *text, float x, float y, float scale)
{
    y -= 650;
    // activate corresponding render state	
    glUseProgram(shader.program);

    glUniformMatrix4fv(glGetUniformLocation(shader.program, "projection"), 1, false, (float*)orthographic);
    glUniform3f(glGetUniformLocation(shader.program, "textColor"), 1.f, 1.f, 1.f);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // iterate through all characters
    for (unsigned int i = 0; i < strlen(text); i++) 
    {
        unsigned char c = text[i];
        glyph ch = glyphs[c];

        //y = (SCREEN_HEIGHTF - y) - glyphs[32].bearing.y;

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.id);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_alphabet(const unsigned char *text, float x, float y)
{
    int thing = 0;
    int lastone = 0;
    int lastonex = 0;
    int lastonew = 0;
    int xpos;
    for (unsigned int i = 0; i < strlen(text); i++){

        char prefix[2];
        prefix[0] = toupper(text[i]);
        prefix[1] = 0x0;

        if(prefix[0] == ' '){thing = 40; continue;}
        animation_play(&alphabet_base->animation, prefix);
        if(!lastone)
            xpos = (x);
        else
            xpos = (lastonex + lastonew) + thing;
        move_sprite(alphabet_base, xpos, y);
        draw_sprite(alphabet_base);
        thing = 0;
        lastone = 1;
        lastonex = xpos;
        lastonew = alphabet_base->graphic.w;
    }
}
