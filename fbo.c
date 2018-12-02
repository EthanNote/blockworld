#include <GL/glew.h>
#include "fbo.h"
#include<stdio.h>

void fbo_init(struct FBO* fbo, int width, int height, int color_buffer_count) {
	glGenFramebuffers(1, &fbo->_fbo);
	//printf("fbo_init _ glGenFramebuffers(1, &fbo->_fbo); = %d\n", glGetError());
	if (color_buffer_count > 0) {
		fbo->color_buffer_list = calloc(color_buffer_count, sizeof(GLuint));
		fbo->color_buffer_count = color_buffer_count;
	}
	else {
		fbo->color_buffer_list = 0;
		fbo->color_buffer_count = 0;
	}
	fbo->width = width;
	fbo->height = height;
}


/*
index: buffer list index
InternalType: GL_RGB32F or GL_R32F
*/
void fbo_create_color_buffer(struct FBO* fbo, int index, GLenum InternalType) {
	GLenum Format, Type;

	switch (InternalType) {
	case GL_RGB32F:
		Format = GL_RGB;
		Type = GL_FLOAT;
		break;
	case GL_R32F:
		Format = GL_RED;
		Type = GL_FLOAT;
		break;
	case GL_NONE:
		break;
	default:
		fprintf(stderr, "Invalid internal type\n");
	}


	glBindFramebuffer(GL_FRAMEBUFFER, fbo->_fbo);
	//printf("fbo_create_color_buffer _ glBindFramebuffer(GL_FRAMEBUFFER, fbo->_fbo); = %d\n", glGetError());
	// Create the textures
	GLuint texture;
	if (InternalType != GL_NONE) {
		glGenTextures(1, &texture);
		int attachment = GL_COLOR_ATTACHMENT0 + index;
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, InternalType, fbo->width, fbo->height, 0, Format, Type, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);

		//GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0+index };

		//glDrawBuffers(ARRAY_SIZE_IN_ELEMENTS(DrawBuffers), DrawBuffers);
		glDrawBuffers(1, &attachment);

		fbo->color_buffer_list[index] = texture;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//printf("fbo_create_color_buffer _ glBindFramebuffer(GL_FRAMEBUFFER, 0); = %d\n", glGetError());
}

void fbo_create_depth_buffer(struct FBO* fbo) {

	glBindFramebuffer(GL_FRAMEBUFFER, fbo->_fbo);
	//printf("fbo_create_depth_buffer _ glBindFramebuffer(GL_FRAMEBUFFER, fbo->_fbo); = %d\n", glGetError());
	GLuint depth_texture;
	glGenTextures(1, &depth_texture);

	// depth
	glBindTexture(GL_TEXTURE_2D, depth_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, fbo->width, fbo->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture, 0);
	//printf("fbo_create_depth_buffer _ glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture, 0); = %d\n", glGetError());

	fbo->depth_buffer = depth_texture;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//printf("fbo_create_depth_buffer _ glBindFramebuffer(GL_FRAMEBUFFER, 0); = %d\n", glGetError());
}


int fbo_verify(struct FBO* fbo) {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo->_fbo);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
		return 0;
	}

	return 1;
}

void fbo_bind(struct FBO* fbo) {
	if (fbo) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo->_fbo);
		//printf("void fbo_bind(struct FBO* fbo) _ glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo->_fbo); = %d\n", glGetError());
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}