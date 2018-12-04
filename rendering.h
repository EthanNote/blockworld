//
// Created by snipe_000 on 2018/10/23.
//

//#define _CRT_SECURE_NO_WARNINGS
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "model.h"
#include "materials.h"
//#include <pthread.h>

#ifndef WORLD_RENDERING_H
#define WORLD_RENDERING_H

struct VERTEX {
	float x;
	float y;
	float z;
};

struct FACE {
	struct VERTEX vertices[6];
};


struct FACE_BUFFER {
	struct FACE* data;
	int capasity;
	int facecount;
	struct FACE_MATERIAL* face_material;
	void* low_level_context;

	//pthread_rwlock_t buffer_lock;
};

struct BLOCK_BUFFER
{
	char material_name[16];
	struct FACE_BUFFER facebuffer[6];
};

struct BUFFER_LIST {
	struct BLOCK_BUFFER* named_buffers;
	int count;
	int capacity;
};

struct GEOMETRY_PIPLINE {
	float mat_modelview[16];
	float mat_projection[16];
	float mat_MVP[16];
	struct FBO* render_target;
	//struct TECHNIQUE* technique;

	GLuint program;
	GLuint loc_mat_MVP;
	GLuint loc_mat_modelview;
	GLuint loc_normal;
	GLuint loc_color;
};

//struct BUFFER_LIST* create_buffer_list_from_materials(struct BLOCK_MATERIAL_LIST* block_material_list) {
//	struct BUFFER_LIST* buffer_list = malloc(sizeof(struct BUFFER_LIST));
//	buffer_list->count = block_material_list->count;
//	buffer_list->capacity = buffer_list->count;
//	buffer_list->named_buffers = malloc(sizeof(struct NAMED_BLOCK_FACE_BUFFER)*buffer_list->capacity);
//	for (int i = 0; i < buffer_list->count; i++) {
//		//strcpy(buffer_list->named_buffers[i].name, block_material_list->materials[i].name);
//		strcpy_s(buffer_list->named_buffers[i].name, 16, block_material_list->materials[i].name);
//	}
//}

struct BUFFER_LIST* create_buffer_list_from_materials(
	struct BLOCK_MATERIAL_LIST* block_material_list,
	int init_face_buffer_capacity
);

void init_buffer_list_from_materials(
	struct BLOCK_MATERIAL_LIST* block_material_list,
	int init_face_buffer_capacity,
	struct BUFFER_LIST* buffer_list
);

void fill_material_face_buffer(struct WORLD_BLOCK* node, struct FACE_BUFFER* facebuffer);

void fill_buffer_list(
	struct WORLD_BLOCK* node,
	struct BUFFER_LIST* buffer_list
);

void update_buffer_list_material(
	struct BUFFER_LIST* buffer_list,
	struct BLOCK_MATERIAL_LIST* block_material_list
);

void draw_buffer_list(struct BUFFER_LIST* buffer_list);

//struct FACEBUFFERWRITER{
//    struct FACEBUFFER* facebuffer;
//    int index;
//};

//struct LAYER{
//
//};


void init_face_buffer(struct FACE_BUFFER* facebuffer, int size);

void clean_face_buffer(struct FACE_BUFFER* facebuffer);

void resize_face_buffer(struct FACE_BUFFER* facebuffer, int size);

//void init_face_buffer_writer(struct FACEBUFFERWRITER* facebufferwriter, struct FACEBUFFER* facebuffer   );

void fill_face(struct WORLD_BLOCK* node, int direction, struct FACE* face);

void fill_face_buffer(struct WORLD_BLOCK* node, struct FACE_BUFFER* facebuffer);

void feed_buffer(struct FACE_BUFFER* facebuffer);

void draw_buffer(struct FACE_BUFFER* facebuffer);

void feed_buffer_list(struct BUFFER_LIST* buffer_list);

void apply_face_material(struct FACE_MATERIAL* face_material);


//void init_geometry_pipline(struct GEOMETRY_PIPLINE *pipline, struct FBO* render_target, struct TECHNIQUE* technique);
void init_geometry_pipline(struct GEOMETRY_PIPLINE *pipline, struct FBO* render_target);

void set_pipline_transforms(struct GEOMETRY_PIPLINE *pipline, struct CAMERA* camera);

void render_buffer_list(struct GEOMETRY_PIPLINE *pipline, struct BUFFER_LIST* buffer_list);

#endif //WORLD_RENDERING_H
