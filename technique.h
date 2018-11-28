#pragma once
#include <GLFW/glfw3.h>
struct SHADER {
	GLuint obj;
	GLuint type;
	const char* source;
};

struct TECHNIQUE {

	GLuint program;

	struct SHADER* shader_list;
	int shader_count;
	int shader_capacity;


};

void technique_init(struct TECHNIQUE* technique);

void technique_destroy(struct TECHNIQUE* technique);

int technique_add_shader(struct TECHNIQUE* technique, const char* shader_source, GLuint shader_type, char* error_log);

int technique_add_shader_from_source_file(struct TECHNIQUE* technique, const char* fname, GLuint shader_type);

void technique_enable(struct TECHNIQUE* technique);

GLuint technique_get_uniform_location(struct TECHNIQUE* technique, const char* uniform_name);

int technique_finalize(struct TECHNIQUE* technique);

int technique_set_uniform1i(struct TECHNIQUE* technique, const char* uniform_name, GLint value);