#include "shader.h"
#include "glad/glad.h"
#include "types.h"

#include <GL/gl.h>
#include <string.h>
#include <stdio.h>

static const char* vshaderd = glsl(
    layout(location = 0) in vec3 vertexPos;
    layout(location = 1) in vec2 vertexCoord;

    out vec2 fragCoord;
    out vec3 Color;

    uniform mat4 projection;
    uniform mat4 model;
    uniform mat4 view;

    void main() {
        fragCoord = vertexCoord;
        gl_Position = projection * view * model * vec4(vertexPos, 1.0f); // projection * * model 
    }
);

static const char* fshaderd = glsl(
    uniform sampler2D bitmap;
    uniform vec2 ScreenResolution;

    out vec4 FragColor;
    in vec2 fragCoord;

    void main() {    
        vec2 uv = fragCoord / vec2(textureSize(bitmap, 0));
        FragColor = texture(bitmap, uv);
    }
);

void checkShaderStatus(int shader)
{

    int compiled = false;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
	    char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Error compiling shader: %s\n",infoLog);
    }
}

fnf_shader create_shader_text(const char* vshader, const char* fshader){
    fnf_shader shader;

    shader.program = glCreateProgram();

    int vs = glCreateShader(GL_VERTEX_SHADER);
    int fs = glCreateShader(GL_FRAGMENT_SHADER);

    int vshaderlength = strlen(vshader);
    int fshaderlength = strlen(fshader);

    glShaderSource(vs, 1, &vshader, &vshaderlength);
    glShaderSource(fs, 1, &fshader, &fshaderlength);

    glCompileShader(vs);
    glCompileShader(fs);

    checkShaderStatus(vs);
    checkShaderStatus(fs);

    glAttachShader(shader.program, vs);
    glAttachShader(shader.program, fs);

    glLinkProgram(shader.program);

    return shader;
}

fnf_shader create_shader()
{
    create_shader_text(vshaderd, fshaderd);
}