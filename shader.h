#pragma once
#include<GL/glew.h>
#include <GLFW/glfw3.h>

void program_load(GLuint program, const char* vs_fname, const char* fs_fname, const char* gs_fname);

void program_load_from_string(GLuint program,
	const char* vs_str, const char* fs_str, const char* gs_str,
	const char* vs_name, const char* fs_name, const char* gs_name);