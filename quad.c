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
	
	glGenVertexArrays(1, &quad->vao);
	glBindVertexArray(quad->vao);

	glGenBuffers(1, &quad->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, quad->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(simple_quad_vertices), simple_quad_vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, (const void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, (const void*)8);
	
	glBindVertexArray(0);

}

void quad_set_program(struct QUAD* quad, GLuint program) {
	/*glBindVertexArray(quad->vao);
	glUseProgram(program);
	glBindVertexArray(0);*/
	quad->program = program;
}


void quad_update_vbo(struct QUAD* quad) {
	
}

void quad_render_default(struct QUAD* quad) {

	if (quad->program) {
		glUseProgram(quad->program);
	}
	glBindVertexArray(quad->vao);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4); 


	if (quad->program) {
		glUseProgram(0);
	}
	glBindVertexArray(0);

}