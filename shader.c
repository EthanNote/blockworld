#define _CRT_SECURE_NO_WARNINGS

#include "shader.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


char* readfile(const char* fname) {

	FILE* fp = fopen(fname, "r");
	if (!fp) {
		fprintf(stderr, "Failed to open file %s\n", fname);
		return NULL;
	}
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);

	char* buffer = malloc(size + 1);
	memset(buffer, 0, size + 1);
	fseek(fp, 0, SEEK_SET);
	fread(buffer, 1, size, fp);
	fclose(fp);

	return buffer;
}

void program_load(GLuint program, const char* vs_fname, const char* fs_fname, const char* gs_fname) {
	const char* vs_str = 0;
	const char* fs_str = 0;
	const char* gs_str = 0;

	if (vs_fname) {
		vs_str = readfile(vs_fname);
	}

	if (fs_fname) {
		fs_str = readfile(fs_fname);
	}

	if (gs_fname) {
		gs_str = readfile(gs_fname);
	}

	program_load_from_string(program, vs_str, fs_str, gs_str, vs_fname, fs_fname, gs_fname);
	
	if (vs_str) { free(vs_str); }
	if (fs_str) { free(fs_str); }
	if (gs_str) { free(gs_str); }
	
}


GLuint compile_shader(GLuint shader_type, const char* source, GLuint* ShaderObj) {
	*ShaderObj = glCreateShader(shader_type);

	if (*ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", shader_type);
		return 0;
	}
	char* source_list[1] = { source };
	GLuint length_list[1] = { strlen(source) };
	glShaderSource(*ShaderObj, 1, source_list, length_list);

	glCompileShader(*ShaderObj);

	GLint success;
	glGetShaderiv(*ShaderObj, GL_COMPILE_STATUS, &success);

	return success;
}



void program_load_from_string(GLuint program, 
	const char* vs_str, const char* fs_str, const char* gs_str, 
	const char* vs_name, const char* fs_name, const char* gs_name) 
{


	GLuint vs = 0, fs = 0, gs = 0;
	if (!vs_name) {
		vs_name = "'string'";
	}

	if (!fs_name) {
		fs_name = "'string'";
	}

	if (!gs_name) {
		gs_name = "'string'";
	}

	char error_log_buffer[1024];

	if (vs_str) {
		if (!compile_shader(GL_VERTEX_SHADER, vs_str, &vs)) {
			glGetShaderInfoLog(vs, 1024, NULL, error_log_buffer);
			fprintf(stderr, "Vertex Shader %s compiling failed\n%s\n", vs_name,  error_log_buffer);
		}
		else {
			glAttachShader(program, vs);
		}
	}

	if (fs_str) {
		if (!compile_shader(GL_FRAGMENT_SHADER, fs_str, &fs)) {
			glGetShaderInfoLog(fs, 1024, NULL, error_log_buffer);
			fprintf(stderr, "Fragment Shader %s compiling failed\n%s\n", fs_name, error_log_buffer);
		}
		else {
			glAttachShader(program, fs);
		}
	}

	if (gs_str) {
		if (!compile_shader(GL_GEOMETRY_SHADER, gs_str, &gs)) {
			glGetShaderInfoLog(gs, 1024, NULL, error_log_buffer);
			fprintf(stderr, "Geometry Shader %s compiling failed\n%s\n", gs_name,  error_log_buffer);
		}
		else {
			glAttachShader(program, gs);
		}
	}


	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(program, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
	}

	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(program, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
	}

	if (vs) {
		glDeleteShader(vs);
	}
	if (fs) {
		glDeleteShader(fs);
	}
	if (gs) {
		glDeleteShader(gs);
	}
}