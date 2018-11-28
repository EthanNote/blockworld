//
// Created by snipe_000 on 2018/10/23.
//

#include <stdlib.h>
#include "rendering.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include "materials.h"

#include "utils.h"

struct FACEBUFFER_GL_CONTEXT {
	struct FACE_BUFFER* facebuffer;
	GLuint vbo;
	struct FACE_MATERIAL* face_material;
};

void init_face_buffer(struct FACE_BUFFER* facebuffer, int size) {
	int face_size = sizeof(struct FACE);
	facebuffer->data = (struct FACE*)malloc(size*face_size);
	//facebuffer->data = new FACE[size];
	facebuffer->capasity = size;
	facebuffer->facecount = 0;

	struct FACEBUFFER_GL_CONTEXT* gl_context = malloc(sizeof(struct FACEBUFFER_GL_CONTEXT));
	gl_context->facebuffer = facebuffer;
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	gl_context->vbo = vbo;
	facebuffer->low_level_context = gl_context;

	//pthread_rwlock_init(&facebuffer->buffer_lock, NULL);
}

void clean_face_buffer(struct FACE_BUFFER* facebuffer) {
	facebuffer->facecount = 0;
}

void resize_face_buffer(struct FACE_BUFFER* facebuffer, int size) {
	facebuffer->data = realloc(facebuffer->data, size * sizeof(struct FACE));
	facebuffer->capasity = size;

}

//void init_face_buffer_writer(struct FACEBUFFERWRITER* facebufferwriter, struct FACEBUFFER* facebuffer){
//    facebufferwriter->facebuffer=facebuffer;
//    facebufferwriter->index=0;
//}

void fill_face(struct WORLD_BLOCK* node, int direction, struct FACE* face) {

	int size = 1 << (node->level);

	switch (direction) {
	case 0: //x-
		face->vertices[0].x = node->position.x;
		face->vertices[0].y = node->position.y;
		face->vertices[0].z = node->position.z;

		face->vertices[1].x = node->position.x;
		face->vertices[1].y = node->position.y;
		face->vertices[1].z = node->position.z + size;

		face->vertices[2].x = node->position.x;
		face->vertices[2].y = node->position.y + size;
		face->vertices[2].z = node->position.z + size;

		face->vertices[3].x = node->position.x;
		face->vertices[3].y = node->position.y + size;
		face->vertices[3].z = node->position.z;

		break;

	case 1: //x+
		face->vertices[0].x = node->position.x + size;
		face->vertices[0].y = node->position.y;
		face->vertices[0].z = node->position.z;

		face->vertices[1].x = node->position.x + size;
		face->vertices[1].y = node->position.y + size;
		face->vertices[1].z = node->position.z;

		face->vertices[2].x = node->position.x + size;
		face->vertices[2].y = node->position.y + size;
		face->vertices[2].z = node->position.z + size;

		face->vertices[3].x = node->position.x + size;
		face->vertices[3].y = node->position.y;
		face->vertices[3].z = node->position.z + size;

		break;
	case 2: //y-
		face->vertices[0].x = node->position.x;
		face->vertices[0].y = node->position.y;
		face->vertices[0].z = node->position.z;

		face->vertices[1].x = node->position.x + size;
		face->vertices[1].y = node->position.y;
		face->vertices[1].z = node->position.z;

		face->vertices[2].x = node->position.x + size;
		face->vertices[2].y = node->position.y;
		face->vertices[2].z = node->position.z + size;

		face->vertices[3].x = node->position.x;
		face->vertices[3].y = node->position.y;
		face->vertices[3].z = node->position.z + size;

		break;

	case 3: //y+

		face->vertices[0].x = node->position.x;
		face->vertices[0].y = node->position.y + size;
		face->vertices[0].z = node->position.z;

		face->vertices[1].x = node->position.x;
		face->vertices[1].y = node->position.y + size;
		face->vertices[1].z = node->position.z + size;

		face->vertices[2].x = node->position.x + size;
		face->vertices[2].y = node->position.y + size;
		face->vertices[2].z = node->position.z + size;

		face->vertices[3].x = node->position.x + size;
		face->vertices[3].y = node->position.y + size;
		face->vertices[3].z = node->position.z;

		break;

	case 4: //z-

		face->vertices[0].x = node->position.x;
		face->vertices[0].y = node->position.y;
		face->vertices[0].z = node->position.z;

		face->vertices[1].x = node->position.x;
		face->vertices[1].y = node->position.y + size;
		face->vertices[1].z = node->position.z;

		face->vertices[2].x = node->position.x + size;
		face->vertices[2].y = node->position.y + size;
		face->vertices[2].z = node->position.z;

		face->vertices[3].x = node->position.x + size;
		face->vertices[3].y = node->position.y;
		face->vertices[3].z = node->position.z;
		break;

	case 5: //z+

		face->vertices[0].x = node->position.x;
		face->vertices[0].y = node->position.y;
		face->vertices[0].z = node->position.z + size;

		face->vertices[1].x = node->position.x + size;
		face->vertices[1].y = node->position.y;
		face->vertices[1].z = node->position.z + size;

		face->vertices[2].x = node->position.x + size;
		face->vertices[2].y = node->position.y + size;
		face->vertices[2].z = node->position.z + size;

		face->vertices[3].x = node->position.x;
		face->vertices[3].y = node->position.y + size;
		face->vertices[3].z = node->position.z + size;

		break;


	default:
		break;


	}
	face->vertices[4] = face->vertices[0];
	face->vertices[5] = face->vertices[2];
}

//void face_buffer_write(struct FACEBUFFER* facebuffer, struct FACE* face){
//    memcpy(&facebuffer->data[facebuffer->facecount], face, sizeof(struct FACE));
//    facebuffer->facecount++;
//}

void fill_face_buffer(struct WORLD_BLOCK* node, struct FACE_BUFFER* facebuffer) {
	if (!node) {
		return;
	}

	if (node->visual_effect.is_visible) {
		for (int i = 0; i < 6; i++) {
			if (node->visual_effect.blocked_faces ^ (0x01 << i)) {
				//pthread_rwlock_wrlock(&facebuffer->buffer_lock);
				if (facebuffer->facecount >= facebuffer->capasity) {
					resize_face_buffer(facebuffer, facebuffer->capasity * 2);
				}
				fill_face(node, i, facebuffer->data + facebuffer->facecount);
				facebuffer->facecount++;

				//pthread_rwlock_unlock(&facebuffer->buffer_lock);
			}

		}
	}
	else {
		for (int i = 0; i < 8; i++) {
			fill_face_buffer(node->children[i], facebuffer);
		}
	}

}

void feed_buffer(struct FACE_BUFFER* facebuffer) {
	glBindBuffer(GL_ARRAY_BUFFER, ((struct FACEBUFFER_GL_CONTEXT*)facebuffer->low_level_context)->vbo);
	glBufferData(GL_ARRAY_BUFFER, facebuffer->facecount * sizeof(struct FACE), facebuffer->data, GL_STATIC_DRAW);
}

void feed_buffer_list(struct BUFFER_LIST* buffer_list) {
	for (int i = 0; i < buffer_list->count; i++) {
		for (int j = 0; j < 6; j++) {
			feed_buffer(&buffer_list->named_buffers[i].facebuffer[j]);
		}
	}
}

void apply_face_material(struct FACE_MATERIAL* face_material) {
	glColor3fv(face_material->main_color);
}

void draw_buffer(struct FACE_BUFFER* facebuffer) {
	if (facebuffer->face_material) {
		apply_face_material(facebuffer->face_material);
	}
	glBindBuffer(GL_ARRAY_BUFFER, ((struct FACEBUFFER_GL_CONTEXT*)facebuffer->low_level_context)->vbo);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, facebuffer->facecount * 6);
	/*glBegin(GL_TRIANGLES);
	GLfloat* pv = facebuffer->data;
	for (int i = 0; i < facebuffer->facecount; i++) {
		for (int j = 0; j < 6; j++) {
			glVertex3fv(pv);
			pv += 3;
		}
	}
	glEnd();*/

}

void draw_buffer_list(struct BUFFER_LIST* buffer_list) {
	for (int i = 0; i < buffer_list->count; i++) {
		for (int j = 0; j < 6; j++) {
			struct FACE_BUFFER* facebuffer = &buffer_list->named_buffers[i].facebuffer[j];
			draw_buffer(facebuffer);
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

struct BUFFER_LIST* create_buffer_list_from_materials(
	struct BLOCK_MATERIAL_LIST* block_material_list,
	int init_face_buffer_capacity
) {
	struct BUFFER_LIST* buffer_list = malloc(sizeof(struct BUFFER_LIST));
	buffer_list->count = block_material_list->count;
	buffer_list->capacity = buffer_list->count;
	buffer_list->named_buffers
		= malloc(sizeof(struct BLOCK_BUFFER)*buffer_list->capacity);

	for (int i = 0; i < buffer_list->count; i++) {
		strcpy(buffer_list->named_buffers[i].material_name, block_material_list->materials[i].material_name);
		for (int j = 0; j < 6; j++) {
			init_face_buffer(&buffer_list->named_buffers[i].facebuffer[j], init_face_buffer_capacity);
			struct FACEBUFFER_GL_CONTEXT* gl_context = buffer_list->named_buffers[i].facebuffer[j].low_level_context;
			gl_context->face_material = &block_material_list->materials[i].face_material[j];
		}
	}
	update_buffer_list_material(buffer_list, block_material_list);
	return buffer_list;
}

void init_buffer_list_from_materials(
	struct BLOCK_MATERIAL_LIST* block_material_list,
	int init_face_buffer_capacity,
	struct BUFFER_LIST* buffer_list
) {
	//struct BUFFER_LIST* buffer_list = malloc(sizeof(struct BUFFER_LIST));
	buffer_list->count = block_material_list->count;
	buffer_list->capacity = buffer_list->count;
	buffer_list->named_buffers
		= malloc(sizeof(struct BLOCK_BUFFER)*buffer_list->capacity);

	for (int i = 0; i < buffer_list->count; i++) {
		strcpy(buffer_list->named_buffers[i].material_name, block_material_list->materials[i].material_name);
		for (int j = 0; j < 6; j++) {
			init_face_buffer(&buffer_list->named_buffers[i].facebuffer[j], init_face_buffer_capacity);
			struct FACEBUFFER_GL_CONTEXT* gl_context = buffer_list->named_buffers[i].facebuffer[j].low_level_context;
			gl_context->face_material = &block_material_list->materials[i].face_material[j];
		}
	}
	update_buffer_list_material(buffer_list, block_material_list);
}


/*
block_material_list:
[
	{name, face_material[6]}
	,
	...
]

buffer_list:
[
	named_buffers:
	{
		name,
		facebuffer : {face_material, data}[6]
	},
	...
]

*/
void update_buffer_list_material(struct BUFFER_LIST* buffer_list, struct BLOCK_MATERIAL_LIST* block_material_list) {
	for (int i = 0; i < buffer_list->count; i++) {
		char* buffer_name = buffer_list->named_buffers[i].material_name;
		buffer_list->named_buffers[i].facebuffer[0].face_material = NULL;
		buffer_list->named_buffers[i].facebuffer[1].face_material = NULL;
		buffer_list->named_buffers[i].facebuffer[2].face_material = NULL;
		buffer_list->named_buffers[i].facebuffer[3].face_material = NULL;
		buffer_list->named_buffers[i].facebuffer[4].face_material = NULL;
		buffer_list->named_buffers[i].facebuffer[5].face_material = NULL;
		for (int j = 0; j < block_material_list->count; j++) {
			char* material_name = block_material_list->materials[j].material_name;
			if (STR_EQUAL(buffer_name, material_name)) {
				buffer_list->named_buffers[i].facebuffer[0].face_material = &block_material_list->materials[j].face_material[0];
				buffer_list->named_buffers[i].facebuffer[1].face_material = &block_material_list->materials[j].face_material[1];
				buffer_list->named_buffers[i].facebuffer[2].face_material = &block_material_list->materials[j].face_material[2];
				buffer_list->named_buffers[i].facebuffer[3].face_material = &block_material_list->materials[j].face_material[3];
				buffer_list->named_buffers[i].facebuffer[4].face_material = &block_material_list->materials[j].face_material[4];
				buffer_list->named_buffers[i].facebuffer[5].face_material = &block_material_list->materials[j].face_material[5];
				break;
			}
		}
	}
}

void fill_buffer_list(
	struct WORLD_BLOCK* node,
	struct BUFFER_LIST* buffer_list
)
{
	if (!node) {
		return;
	}
	char* node_material_name = node->visual_effect.material_name;
	int i = buffer_list->count - 1;
	for (; i > 0; i--) {
		char* material_name = buffer_list->named_buffers[i].material_name;
		if (STR_EQUAL(node_material_name, material_name)) {
			break;
		}
	}

	//int i = node->visual_effect.material_id;

	if (node->visual_effect.is_visible) {
		for (int j = 0; j < 6; j++) {
			if (~node->visual_effect.blocked_faces & (0x01 << j)) {
				//pthread_rwlock_wrlock(&facebuffer->buffer_lock);
				struct FACE_BUFFER* facebuffer = &buffer_list->named_buffers[i].facebuffer[j];
				if (facebuffer->facecount >= facebuffer->capasity) {
					resize_face_buffer(facebuffer, facebuffer->capasity * 2);
				}
				fill_face(node, j, facebuffer->data + facebuffer->facecount);
				facebuffer->facecount++;

				//pthread_rwlock_unlock(&facebuffer->buffer_lock);
			}

		}
	}
	else {
		for (int j = 0; j < 8; j++) {
			fill_buffer_list(node->children[j], buffer_list);
		}
	}

}

