#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "technique.h"



void technique_init(struct TECHNIQUE* technique) {
	technique->program = glCreateProgram();
	technique->shader_capacity = 2;
	technique->shader_count = 0;
	technique->shader_list = malloc(sizeof(struct SHADER)*technique->shader_capacity);

	technique->vertex_format.items = malloc(sizeof(struct VERTEXFORMATITEM) * 4);
	technique->vertex_format.capacity = 4;
	technique->vertex_format.count = 0;

	technique->extra_data = 0;
}

void technique_init_ex(struct TECHNIQUE* technique, int ext_size) {
	technique_init(technique);
	technique->extra_data = malloc(ext_size);
}

void technique_destroy(struct TECHNIQUE* technique) {
	for (int i = 0; i < technique->shader_count; i++) {
		glDeleteShader(technique->shader_list[i].obj);
	}

	if (technique->program) {
		glDeleteProgram(technique->program);
		technique->program = 0;
	}
}

void _technique_add_shader(struct TECHNIQUE* technique, struct SHADER* shader) {
	if (technique->shader_count == technique->shader_capacity) {
		technique->shader_capacity *= 2;
		technique->shader_list = realloc(technique->shader_list, sizeof(struct SHADER)*technique->shader_capacity);
	}

	technique->shader_list[technique->shader_count++] = *shader;
}

char error_log_buffer[1024];

int technique_add_shader(struct TECHNIQUE* technique, char* shader_source, GLuint shader_type, char* error_log) {
	GLuint ShaderObj = glCreateShader(shader_type);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", shader_type);
		return 0;
	}
	char* source_list[1] = { shader_source };
	GLuint length_list[1] = { strlen(shader_source) };
	glShaderSource(ShaderObj, 1, source_list, length_list);

	glCompileShader(ShaderObj);

	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

	if (!success) {
		if (!error_log) {
			error_log = error_log_buffer;
			glGetShaderInfoLog(ShaderObj, 1024, NULL, error_log);
			fprintf(stderr, "Shader compiling failed [ type : %d ]\n%s\n", shader_type, error_log);
		}
		else {
			glGetShaderInfoLog(ShaderObj, 1024, NULL, error_log);
		}
		return 0;
	}

	struct SHADER shader;
	shader.source = shader_source;
	shader.obj = ShaderObj;
	shader.type = shader_type;
	_technique_add_shader(technique, &shader);

	glAttachShader(technique->program, ShaderObj);

	return 1;
}

char* get_file_data(const char* fname) {
	FILE* fp = fopen(fname, "r");

	if (!fp) {
		fprintf(stderr, "Failed to open file %s\n", fname);
		return NULL;
	}
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	char* buffer = malloc(sizeof(size + 1));
	memset(buffer, 0, size + 1);
	fseek(fp, 0, SEEK_SET);
	fread(buffer, 1, size, fp);
	fclose(fp);

	return buffer;
}

const char* get_postfix(const char* fname) {
	if (!fname) {
		return NULL;
	}

	int pos = strlen(fname) - 1;
	while (pos >= 0 && fname[pos] != '.') {
		pos--;
	}
	if (pos >= 0) {
		return fname + pos;
	}
	return NULL;
}


char error_buffer[1024] = { 0 };
int technique_add_shader_from_source_file(struct TECHNIQUE* technique, const char* fname, GLuint shader_type) {
	const char* postfix = get_postfix(fname);
	char* file_text = get_file_data(fname);
	int result = 0;

	if (!fname) {
		return 0;
	}

	result = technique_add_shader(technique, file_text, shader_type, error_buffer);
	if (!result) {
		fprintf(stderr, "Error compiling '%s': '%s'\n", fname, error_buffer);
		free(file_text);
	}

	return result;
}

void technique_enable(struct TECHNIQUE* technique) {
	glUseProgram(technique->program);
}


#define INVALID_UNIFORM_LOCATION 0xffffffff

GLuint technique_get_uniform_location(struct TECHNIQUE* technique, const char* uniform_name) {
	GLuint Location = glGetUniformLocation(technique->program, uniform_name);

	if (Location == INVALID_UNIFORM_LOCATION) {
		fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", uniform_name);
	}

	return Location;
}

int technique_set_uniform_1i(struct TECHNIQUE* technique, const char* uniform_name, GLint value) {
	GLuint success = 1, Location = glGetUniformLocation(technique->program, uniform_name);
	Location == INVALID_UNIFORM_LOCATION && fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", uniform_name) && (success = 0);
	if (success) { glUniform1i(Location, value); }
	return success;
}


//(GLint location, GLsizei count, const GLfloat* value)
int technique_set_uniform_3fv(struct TECHNIQUE* technique, const char* uniform_name, GLsizei count, const GLfloat* value) {
	GLuint success = 1, Location = glGetUniformLocation(technique->program, uniform_name);
	Location == INVALID_UNIFORM_LOCATION && fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", uniform_name) && (success = 0);
	if (success) { glUniform3fv(Location, count, value); }
	return success;
}
//GLint location, GLsizei count, GLboolean transpose, const GLfloat* value
int technique_set_uniform_matrix4fv(struct TECHNIQUE* technique, const char* uniform_name, GLboolean transpose, const GLfloat* value) {
	GLuint success = 1, Location = glGetUniformLocation(technique->program, uniform_name);
	Location == INVALID_UNIFORM_LOCATION && fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", uniform_name) && (success = 0);
	if (success) { glUniformMatrix4fv(Location, 1, transpose, value); }
	return success;
}

//#define GLCheckError() (glGetError() == GL_NO_ERROR)

int technique_finalize(struct TECHNIQUE* technique) {
	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(technique->program);

	glGetProgramiv(technique->program, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(technique->program, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		return 0;
	}

	glValidateProgram(technique->program);
	glGetProgramiv(technique->program, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(technique->program, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		//   return false;
	}

	// Delete the intermediate shader objects that have been added to the program
	for (int i = 0; i < technique->shader_count; i++) {
		glDeleteShader(technique->shader_list[i].obj);
		//free(technique->shader_list[i].source);
	}
	/*for (ShaderObjList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++) {
		glDeleteShader(*it);
	}

	m_shaderObjList.clear();*/
	//free(technique->shader_list);
	technique->shader_count = 0;
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		fprintf(stderr, "Error while finalize '0x%x'\n", error);
		return 0;
	}
	return 1;
}