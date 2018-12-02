#pragma once
#include <GLFW/glfw3.h>
#include <stdlib.h>
struct FBO {
	GLuint _fbo;

	GLuint* color_buffer_list;
	GLuint depth_buffer;
	GLenum m_internalType;

	int width;
	int height;

	int color_buffer_count;
};

void fbo_init(struct FBO* fbo, int width, int height, int color_buffer_count, int with_depth);

void fbo_create_color_buffer(struct FBO* fbo, int count);

//void fbo_create_color_buffers(struct FBO* fbo, int count);

void fbo_create_depth_buffer(struct FBO* fbo);

int fbo_verify(struct FBO* fbo);

void fbo_bind(struct FBO* fbo);