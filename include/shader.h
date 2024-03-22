#ifndef SHADER_H
#define SHADER_H

#define glsl(x) "#version 330\n"#x

typedef struct {
    int program;
} fnf_shader;

fnf_shader create_shader_text(const char* vshader, const char* fshader);
fnf_shader create_shader();
#endif // SHADER_H