//
// Created by snipe_000 on 2018/10/23.
//

#include <malloc.h>
#include "rendering.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "materials.h"

struct FACEBUFFER_GL_CONTEXT {
	struct FACE_BUFFER* facebuffer;
	GLuint vbo;
	struct FACE_MATERIAL* face_material;
};

void init_face_buffer(struct FACE_BUFFER* facebuffer, int size) {
	facebuffer->data = malloc(size * sizeof(struct FACE));
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

void apply_face_material(struct FACE_MATERIAL* face_material){
    glColor3fv(face_material->final_color);
}

void draw_buffer(struct FACE_BUFFER* facebuffer) {
    if(facebuffer->face_material){
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

void draw_buffer_list(struct BUFFER_LIST* buffer_list){
    for(int i=0;i<buffer_list->count;i++){
        for(int j=0;j<6;j++){
            struct FACE_BUFFER* facebuffer=&buffer_list->named_buffers[i].facebuffer[j];
            draw_buffer(facebuffer);
        }
    }
}

struct BUFFER_LIST* create_buffer_list_from_materials(
    struct* BLOCK_MATERIAL_LIST block_material_list
    int init_face_buffer_capacity=1000
    ) {
	struct BUFFER_LIST* buffer_list = malloc(sizeof(struct BUFFER_LIST));
    buffer_list->count=block_material_list->count;
    buffer_list->capacity=buffer_list->count;
	buffer_list->named_buffers
        =malloc(sizeof(struct NAMED_BLOCK_FACE_BUFFER)*buffer_list->capacity);

    for(int i=0;i<buffer_list->count;i++){
        strcpy(buffer_list->named_buffers[i].name, block_material_list->materials[i].name);
        for(int j=0;j<6;j++){
            init_face_buffer(&buffer_list->named_buffers[i].facebuffer[j], init_face_buffer_capacity);
            struct FACEBUFFER_GL_CONTEXT* gl_context=buffer_list->named_buffers[i].facebuffer[j].low_level_context;
            gl_context->face_material=&block_material_list->materials[i].face_material[j];
        }
    }
    return buffer_list;
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
    int i=buffer_list->count-1;
    for(;i>0;i--){
        char* material_name=buffer_list->named_buffers[i].name;
        if(strcmp(node_material_name, material_name)==0){
            break;
        }
    }
    if (node->visual_effect.is_visible) {
		for (int j = 0; j < 6; j++) {
			if (node->visual_effect.blocked_faces ^ (0x01 << j)) {
				//pthread_rwlock_wrlock(&facebuffer->buffer_lock);
				struct FACE_BUFFER* facebuffer = &buffer_list->named_buffers[i].facebuffer[j];
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


// void fill_material_face_buffer(
// 	struct WORLD_BLOCK* node,
// 	struct BLOCK_MATERIAL_LIST* material_list,
// 	struct BUFFER_LIST* materialfacebuffer
// ) {
// 	if (!node) {
// 		return;
// 	}

// 	char* material_name = node->visual_effect.material_name;
// 	int i = material_list->count - 1;
// 	for (; i > 0; i--) {
// 		if (strcmp(material_list->materials[i].name, material_name) == 0) {
// 			break;
// 		}
// 	}
// 	if (node->visual_effect.is_visible) {
// 		for (int j = 0; j < 6; j++) {
// 			if (node->visual_effect.blocked_faces ^ (0x01 << j)) {
// 				//pthread_rwlock_wrlock(&facebuffer->buffer_lock);
// 				struct FACE_BUFFER* facebuffer = &materialfacebuffer->buffers[i * 6 + j];
// 				fill_face(node, j, facebuffer->data + facebuffer->facecount);
// 				facebuffer->facecount++;

// 				//pthread_rwlock_unlock(&facebuffer->buffer_lock);
// 			}

// 		}
// 	}
// 	else {
// 		for (int j = 0; j < 8; j++) {
// 			fill_material_face_buffer(node->children[j], material_list, materialfacebuffer);
// 		}
// 	}
// }