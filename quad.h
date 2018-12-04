#pragma once
#include <GLFW/glfw3.h>


struct QUAD {
	GLuint vbo;
	GLuint vao;
	GLuint program;
};


struct QUAD_DEFAULT {
	struct QUAD _;
};


void quad_create_default(struct QUAD* quad);

void quad_set_program(struct QUAD* quad, GLuint program);

void quad_render_default(struct QUAD* quad);
