#pragma once
#include <GLFW/glfw3.h>
struct SHADER {
	GLuint obj;
	GLuint type;
	const char* source;
};

struct VERTEXFORMATITEM {
	GLuint index;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	const void* pointer;
};

struct VERTEXFORMAT {
	struct VERTEXFORMATITEM* items;
	int count;
	int capacity;
};

struct TECHNIQUE {

	GLuint program;

	struct SHADER* shader_list;
	int shader_count;
	int shader_capacity;

	struct VERTEXFORMAT vertex_format;

	void* extra_data;
};

#define INIT_TECHINQUE(pTechnique) technique_init(pTechnique)
#define INIT_TECHINQUE(pTechnique,extra_data_class) technique_init_ex(pTechnique,sizeof(extra_data_class))

void technique_init(struct TECHNIQUE* technique);

void technique_init_ex(struct TECHNIQUE* technique, int ext_size);

void technique_destroy(struct TECHNIQUE* technique);

int technique_add_shader(struct TECHNIQUE* technique, const char* shader_source, GLuint shader_type, char* error_log);

int technique_add_shader_from_source_file(struct TECHNIQUE* technique, const char* fname, GLuint shader_type);

void technique_enable(struct TECHNIQUE* technique);

GLuint technique_get_uniform_location(struct TECHNIQUE* technique, const char* uniform_name);

int technique_finalize(struct TECHNIQUE* technique);

int technique_set_uniform_1i(struct TECHNIQUE* technique, const char* uniform_name, GLint value);

int technique_set_uniform_3fv(struct TECHNIQUE* technique, const char* uniform_name, GLsizei count, const GLfloat* value);

int technique_set_uniform_matrix4fv(struct TECHNIQUE* technique, const char* uniform_name, GLboolean transpose, const GLfloat* value);