#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "technique.h"
#include "rendering.h"
#include "scene.h"

#include <stdio.h>

#include "fbo.h"



#define USE_PROGRAM 1

//GLuint m_shadowMap;
int WindowWidth = 640;
int WindowHeight = 480;


//GLuint quad_buffer = 0;
float quad[] = {
	-0.8f,  1.0f, 0.0f, 1.0f, //vec2 pos, vec2 tex_coord
	-1.0f, -1.0f, 0.0f, 0.0f,
	1.0f,  -1.0f, 1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f,
};


struct TECHNIQUE technique;

extern const char* depth_texture_fs;
extern const char* depth_texture_vs;


struct GEOMETRY_PIPLINE geo_pipline;

void init_fbos() {
	
	init_geometry_pipline(&geo_pipline, NULL, NULL);
	fbo_verify(geo_pipline.render_target);
}


void init_techniques(){
	struct TECHNIQUE* tech = &technique;
	technique_init(tech);
	technique_add_shader(tech, depth_texture_vs, GL_VERTEX_SHADER, NULL);
	technique_add_shader(tech, depth_texture_fs, GL_FRAGMENT_SHADER, NULL);
	technique_finalize(tech);
	technique_set_uniform_1i(tech, "depth_texture", 0);
}


//void create_quad_vertex_buffer() {
//	//quad vertex object
//	glGenBuffers(1, &quad_buffer);
//	printf("create_quad_vertex_buffer() _ glGenBuffers(1, &quad_buffer);; = %d\n", glGetError());
//
//	glBindBuffer(GL_ARRAY_BUFFER, quad_buffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, quad, GL_STATIC_DRAW);
//
//
//	printf("create_quad_vertex_buffer() _ glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, quad, GL_STATIC_DRAW);; = %d\n", glGetError());
//}


/*

void scene_render(struct SCENE* scene) {
	set_active_camera(&scene->camera);
	draw_buffer_list(scene->scene_buffer_list);
	draw_buffer_list(scene->crafting_buffer_list);
	crafting_update(&scene->forward_hit_info);
	if (scene->forward_hit_info.hit_block) {
		draw_block_hit_normal(&scene->forward_hit_info);
		draw_crafting_box(&scene->crafting_box);
	}
}
*/
void render_scene_trees(struct SCENE* scene) {
	fbo_bind(geo_pipline.render_target);

	//printf("fbo_bind(geo_pipline.render_target); = %d\n", glGetError());

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(0);

	//printf("glUseProgram(0); = %d\n", glGetError());

	technique_enable(geo_pipline.technique);
	set_pipline_transforms(&geo_pipline, &scene->camera);

	//camera_frame_update();
	//render_buffer_list(&geo_pipline, scene->scene_buffer_list);
	render_buffer_list(&geo_pipline, scene->crafting_buffer_list);
	//scene_render(scene);

	glUseProgram(0);
}

int pass_init() {
	
	init_fbos();
	init_techniques();
	//create_quad_vertex_buffer();

	return 1;
}

void pass_geometry() {

	//fbo_bind(&depth_render_target);
	fbo_bind(geo_pipline.render_target);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(0);
}


void pass_quad() {

	//printf("pass_quad _ I .. ; = %d\n", glGetError());

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//printf("pass_quad _ T .. ; = %d\n", glGetError());
	glEnable(GL_TEXTURE_2D);
	//printf("pass_quad _ T1 .. ; = %d\n", glGetError());
	//glEnable(GL_TEXTURE0);
	//printf("pass_quad _ T2 .. ; = %d\n", glGetError());
	glActiveTexture(GL_TEXTURE0);
	//printf("pass_quad _ T3 .. ; = %d\n", glGetError());


	glBindTexture(GL_TEXTURE_2D, geo_pipline.render_target->depth_buffer);

	//printf("pass_quad _ glBindTexture .. ; = %d\n", glGetError());

#ifdef USE_PROGRAM
	
	technique_enable(&technique);

	//glBindBuffer(GL_ARRAY_BUFFER, quad_buffer);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, &quad[0]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, &quad[2]);


	//glEnable(GL_VERTEX_ARRAY);
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