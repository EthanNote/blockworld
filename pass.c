#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "technique.h"
#include <stdio.h>

#include "fbo.h"



//struct PASS {
//	struct FBO* render_target;
//	struct TECHNIQUE* technique;
//
//	GLfloat mat_modelview[16];
//	GLfloat mat_projection[16];
//
//	void (*run)(struct PASS* pass);
//};
//
//void pass_matrix(struct PASS* pass) {
//
//}
//
//void run_pass(struct PASS* pass) {
//
//}

struct FBO depth_render_target;

#define USE_PROGRAM 1

GLuint m_fbo;
GLuint m_shadowMap;
int WindowWidth = 640;
int WindowHeight = 480;


GLuint quad_buffer = 0;
float quad[] = {
	-1.0f,  1.0f, 0.0f, 1.0f, //vec2 pos, vec2 tex_coord
	-1.0f, -1.0f, 0.0f, 0.0f,
	1.0f,  -1.0f, 1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f,
};


struct TECHNIQUE technique;

extern const char* depth_texture_fs;
extern const char* depth_texture_vs;


void init_fbos() {
	fbo_init(&depth_render_target, WindowWidth, WindowHeight, 0);
	fbo_create_depth_buffer(&depth_render_target);
	fbo_verify(&depth_render_target);
}


void init_techniques(){
	struct TECHNIQUE* tech = &technique;
	technique_init(tech);
	technique_add_shader(tech, depth_texture_vs, GL_VERTEX_SHADER, NULL);
	technique_add_shader(tech, depth_texture_fs, GL_FRAGMENT_SHADER, NULL);
	technique_finalize(tech);
	technique_set_uniform1i(tech, "depth_texture", 0);
}


void init_geometry() {
	//quad vertex object
	glGenBuffers(1, &quad_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, quad, GL_STATIC_DRAW);
}

int pass_init() {
	
	init_fbos();
	init_techniques();
	init_geometry();

	return 1;
}

void pass_depth() {

	fbo_bind(&depth_render_target);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(0);
}


void pass_render() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depth_render_target.depth_buffer);


#ifdef USE_PROGRAM
	
	technique_enable(&technique);

	glBindBuffer(GL_ARRAY_BUFFER, quad_buffer);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, 8);


	glEnable(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

#else
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1, 1, 1);
	glVertex2f(-1, 1);
	glTexCoord2f(0, 0);

	glVertex2f(-1, -1);
	glTexCoord2f(1, 0);

	glVertex2f(1, -1);
	glTexCoord2f(1, 1);

	glVertex2f(1, 1);
	glTexCoord2f(0, 1);
	glEnd();
#endif
}