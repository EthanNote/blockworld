#include <GL/glew.h>
#include "quad.h"

#include <stdlib.h>
#include <string.h>


GLfloat simple_quad_vertices[] = {
	-1.0f,  1.0f, 0.0f, 1.0f, //vec2 pos, vec2 tex_coord
	-1.0f, -1.0f, 0.0f, 0.0f,
	1.0f,  -1.0f, 1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f,
};


void quad_create_default(struct QUAD* quad) {
	
	glGenBuffers(1, &quad->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, quad->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(simple_quad_vertices), simple_quad_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void quad_update_vbo(struct QUAD* quad) {
	
}

void quad_render_default(struct QUAD* quad) {

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, quad->vbo);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, (const void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, (const void*)8);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}