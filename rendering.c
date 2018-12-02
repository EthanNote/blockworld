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
#include "fbo.h"
#include "view.h"
#include "technique.h"
struct FACEBUFFER_GL_CONTEXT {
	struct FACE_BUFFER* facebuffer;
	GLuint vbo;
	struct FACE_MATERIAL* face_material;
};




#define GEOMETRY_TECHNIQUE_EXT_FLAG_MAT_MVP 1
#define GEOMETRY_TECHNIQUE_EXT_FLAG_MAT_MV 2
#define GEOMETRY_TECHNIQUE_EXT_FLAG_MAT_P 4


struct GEOMETRY_TECHNIQUE_EXT {
	int flags;
};

extern const char* geo_vs;
extern const char* geo_fs;

void init_geometry_pipline(struct GEOMETRY_PIPLINE *pipline, struct FBO* render_target, struct TECHNIQUE* technique) {
	if (render_target) {
		pipline->render_target = render_target;
	}
	else {
		pipline->render_target = malloc(sizeof(struct FBO));
		fbo_init(pipline->render_target, 640, 480, 3, 1);
		//fbo_create_color_buffer(pipline->render_target, 3);
		/*fbo_create_color_buffer(pipline->render_target, 1, GL_RGB32F);
		fbo_create_color_buffer(pipline->render_target, 2, GL_RGB32F);*/
		//fbo_create_color_buffers(pipline->render_target, 1);
		//fbo_create_depth_buffer(pipline->render_target);
		
	}

	if (technique) {
		pipline->technique = technique;
	}
	else {
		pipline->technique = malloc(sizeof(struct TECHNIQUE));
		technique_init(pipline->technique);
		technique_add_shader(pipline->technique, geo_vs, GL_VERTEX_SHADER, NULL);
		technique_add_shader(pipline->technique, geo_fs, GL_FRAGMENT_SHADER, NULL);
		technique_finalize(pipline->technique);
	}
}



void render_buffer_list(struct GEOMETRY_PIPLINE *pipline, struct BUFFER_LIST* buffer_list) {
	//printf("render_buffer_list _ I = %d\n", glGetError());
	//technique_set_uniform_matrix4fv(pipline->technique, "mat_MVP", GL_FALSE, pipline->mat_projection);
	technique_enable(pipline->technique);
	technique_set_uniform_matrix4fv(pipline->technique, "mat_MVP", GL_FALSE, pipline->mat_MVP);

	GLfloat color[] = { 1.0,1.0,1.0 };
	GLfloat normal_list[] = {
		-1.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, -1.0,
		0.0, 0.0, 1.0,
	};
	//technique_set_uniform_3fv(pipline->technique, "color", 1, color);
	//technique_set_uniform_3fv(pipline->technique, "normal", 1, normal_list);

	//technique_enable(pipline->technique);
	//printf("E1 = %d\n", glGetError());
	//draw buffer list
	for (int i = 0; i < buffer_list->count; i++) {
		for (int j = 0; j < 6; j++) {
			struct FACE_BUFFER* facebuffer = &buffer_list->named_buffers[i].facebuffer[j];
			//draw buffer
			//draw_buffer(facebuffer);
			if (facebuffer->face_material) {
				//apply_face_material(facebuffer->face_material);
				//technique_set_uniform_3fv(pipline->technique, "color", 1, facebuffer->face_material->main_color);
			}
			else {
				//technique_set_uniform_3fv(pipline->technique, "color", 1, color);
			}
			//technique_set_uniform_3fv(pipline->technique, "normal", 1, normal_list + j * 3);

			
		/*	glEnable(GL_VERTEX_ARRAY);

			glBindBuffer(GL_ARRAY_BUFFER, ((struct FACEBUFFER_GL_CONTEXT*)facebuffer->low_level_context)->vbo);
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glDrawArrays(GL_TRIANGLES, 0, facebuffer->facecount * 6);


			glDisable(GL_VERTEX_ARRAY);*/


			


			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, facebuffer->data);
			glDrawArrays(GL_TRIANGLES, 0, facebuffer->facecount * 6);
			glDisableVertexAttribArray(0);

			//printf("E2 = %d\n", glGetError());

			/*

			glBindBuffer(GL_ARRAY_BUFFER, quad_buffer);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, 0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, 8);


			glEnable(GL_VERTEX_ARRAY);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);

			*/
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}


void set_pipline_transforms(struct GEOMETRY_PIPLINE *pipline, struct CAMERA* camera) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	camera_frame_update();
	glGetFloatv(GL_MODELVIEW_MATRIX, pipline->mat_modelview);
	glGetFloatv(GL_PROJECTION_MATRIX, pipline->mat_projection);

	glMultMatrixf(pipline->mat_modelview);
	glGetFloatv(GL_PROJECTION_MATRIX, pipline->mat_MVP);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}



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