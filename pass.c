#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "technique.h"
#include "rendering.h"
#include "scene.h"

#include <stdio.h>
#include <string.h>
#include "fbo.h"
#include "quad.h"
#include "shader.h"

#define USE_PROGRAM 1

int WindowWidth = 640;
int WindowHeight = 480;


struct QUAD simple_quad;


GLuint program;

struct GEOMETRY_PIPLINE geo_pipline;

void init_fbos() {
	
	init_geometry_pipline(&geo_pipline, NULL, NULL);
	fbo_verify(geo_pipline.render_target);
}

#define INVALID_UNIFORM_LOCATION 0xffffffff

void init_techniques(){
	program = glCreateProgram();
	program_load(program, "quad.vs", "quad.fs", 0);
	GLuint success = 1, Location = glGetUniformLocation(program, "depth_texture");
	if (Location == INVALID_UNIFORM_LOCATION) {
		fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", "depth_texture");
		success = 0;
	}
	if (success) {
		glUseProgram(program);
		glUniform1i(Location, 0);
	}
}


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


	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(0);


	technique_enable(geo_pipline.technique);
	set_pipline_transforms(&geo_pipline, &scene->camera);

	render_buffer_list(&geo_pipline, scene->crafting_buffer_list);

	glUseProgram(0);
}

int pass_init() {
	
	init_fbos();
	init_techniques();

	quad_create_default(&simple_quad);
	quad_set_program(&simple_quad, program);

	return 1;
}

void pass_geometry() {

	fbo_bind(geo_pipline.render_target);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(0);
}


void pass_quad() {

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, geo_pipline.render_target->color_buffer_list[1]);
	   	
	quad_render_default(&simple_quad);

}