#pragma once
#include <GLFW/glfw3.h>


struct QUAD {
	GLuint vbo;
};


struct QUAD_DEFAULT {
	struct QUAD _;
};


void quad_create_default(struct QUAD* quad);

void quad_render_default(struct QUAD* quad);
