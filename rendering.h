//
// Created by snipe_000 on 2018/10/23.
//

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

struct NAMED_BLOCK_FACE_BUFFER
{
	char name[16];
	struct FACE_BUFFER facebuffer[6];
};

struct BUFFER_LIST {
	struct NAMED_BLOCK_FACE_BUFFER* named_buffers;
	int count;
	int capacity;
};



struct BUFFER_LIST* create_buffer_list_from_materials(struct BLOCK_MATERIAL_LIST* block_material_list) {
	struct BUFFER_LIST* buffer_list = malloc(sizeof(struct BUFFER_LIST));
	buffer_list->count=block_material_list->count;
	buffer_list->capasity = buffer_list->count;
	buffer_list->buffers = malloc(sizeof(struct NAMED_BLOCK_FACE_BUFFER)*buffer_list->capasity);
	for (int i = 0; i < buffer_list->count; i++) {
		
		
		strcpy(buffer_list->buffers[i].name,block_material_list->materials[i].name);

	}
}

void fill_material_face_buffer(struct WORLD_BLOCK* node, struct FACE_BUFFER* facebuffer);

//struct FACEBUFFERWRITER{
//    struct FACEBUFFER* facebuffer;
//    int index;
//};

//struct LAYER{
//
//};


void init_face_buffer(struct FACE_BUFFER* facebuffer, int size);

void resize_face_buffer(struct FACE_BUFFER* facebuffer, int size);

//void init_face_buffer_writer(struct FACEBUFFERWRITER* facebufferwriter, struct FACEBUFFER* facebuffer   );

void fill_face_buffer(struct WORLD_BLOCK* node, struct FACE_BUFFER* facebuffer);

void feed_buffer(struct FACE_BUFFER* facebuffer);

void draw_buffer(struct FACE_BUFFER* facebuffer);

#endif //WORLD_RENDERING_H
