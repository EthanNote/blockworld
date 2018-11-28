#pragma once
#include<GL/glew.h>
#include <GLFW/glfw3.h>

//void get_file_text(const char* fname, char** text);
//
//GLuint create_shader_from_source(const char* fname, GLuint ShaderType);

void init_shaders();

char** get_vertex_shaders();

char** get_fragment_shaders();