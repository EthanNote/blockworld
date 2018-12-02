#define _CRT_SECURE_NO_WARNINGS

#include "shader.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define INSERT_BEGIN
const char* shadow_map_vs = 
"#version 330\n"
"\n"
"layout (location = 0) in vec3 Position;\n"
"\n"
"uniform mat4 gWVP;\n"
"\n"
"out vec2 TexCoordOut;\n"
"\n"
"void main()\n"
"{\n"
"	gl_Position = gWVP * vec4( Position, 1.0 );\n"
"	TexCoordOut = ((gWVP * vec4( Position, 1.0 )).xy + vec2(1.0)) * 0.5;\n"
"}\n"
;

const char* shadow_map_fs = 
"#version 330\n"
"\n"
"in vec2 TexCoordOut;\n"
"uniform sampler2D gShadowMap;\n"
"\n"
"out vec4 FragColor;\n"
"\n"
"void main(){\n"
"	float Depth = texture(gShadowMap, TexCoordOut).x;\n"
"	Depth = 1.0 - (1.0 - Depth)*25.0;\n"
"	FragColor = vec4(Depth);\n"
"}\n"
;

const char* depth_gray_vs = 
"#version 330\n"
"\n"
"layout (location = 0) in vec3 Position;\n"
"\n"
"uniform mat4 ModelView;\n"
"uniform mat4 Projection;\n"
"\n"
"out float depth;\n"
"\n"
"void main()\n"
"{\n"
"	vec4 pos = Projection * ModelView * vec4( Position, 1.0 );\n"
"	gl_Position = pos;\n"
"	depth=pos.z;\n"
"}\n"
;

const char* depth_gray_fs = 
"#version 330\n"
"\n"
"in float gray;\n"
"\n"
"out vec4 FragColor;\n"
"\n"
"void main(){	\n"
"	FragColor = vec4(abs(gray), 1.0, 0.0, 1.0);\n"
"}\n"
;

const char* quad_vs = 
"#version 330\n"
"\n"
"layout (location = 0) in vec2 Position;\n"
"layout (location = 1) in vec2 TexCoord;\n"
"\n"
"out vec2 pos;\n"
"\n"
"void main()\n"
"{	\n"
"	gl_Position = vec4(Position, 0.0, 1.0);\n"
"	pos = TexCoord;\n"
"}\n"
;

const char* quad_fs = 
"#version 330\n"
"\n"
"uniform sampler2D quad_texture;\n"
"in vec2 pos;\n"
"\n"
"out vec4 FragColor;\n"
"\n"
"void main(){	\n"
"	FragColor = vec4(pos.x/2+0.5, 1.0, pos.y/2+0.5, 1.0);\n"
"	//FragColor = vec4(texture(quad_texture, pos).r,0.0,0.0,1);\n"
"}\n"
;

const char* depth_texture_vs = 
"#version 330\n"
"layout (location = 0) in vec2 position;\n"
"layout (location = 1) in vec2 texcoord;\n"
"\n"
"out vec2 frag_texcoord;\n"
"\n"
"void main(){\n"
"	gl_Position = vec4(position, 0.0, 1.0);\n"
"	frag_texcoord = texcoord;\n"
"}\n"
;

const char* depth_texture_fs = 
"#version 330\n"
"uniform sampler2D depth_texture;\n"
"\n"
"in vec2 frag_texcoord;\n"
"\n"
"out vec4 FragColor;\n"
"\n"
"void main(){\n"
"	float depth=texture(depth_texture, frag_texcoord).x;\n"
"	depth = 1.0 - (1.0 -depth )*25.0;\n"
"	FragColor = vec4(depth);\n"
"}\n"
;

const char* geo_vs = 
"#version 330\n"
"layout (location=0) in vec3 position;\n"
"\n"
"uniform mat4 mat_MVP;\n"
"\n"
"out vec3 view_position;\n"
"\n"
"void main(){\n"
"	gl_Position = mat_MVP * vec4(position, 1.0);\n"
"	view_position = (mat_MVP * vec4(position, 1.0)).xyz;\n"
"}\n"
;

const char* geo_fs = 
"#version 330\n"
"\n"
"uniform vec3 normal;\n"
"uniform vec3 color;\n"
"\n"
"out vec4 FragColor;\n"
"\n"
"in vec3 view_position;\n"
"\n"
"void main(){\n"
"	FragColor = vec4(1.0,0.0,0.0,1.0);\n"
"}\n"
;

#define INSERT_END

//char** vert_shaders[2];
//char** frag_shaders[2];
//
//void init_shaders() {
//	vert_shaders[0] = shadow_map_vs;
//	vert_shaders[1] = depth_gray_vs;
//
//	frag_shaders[0] = shadow_map_fs;
//	frag_shaders[1] = depth_gray_fs;
//}
//
//char** get_vertex_shaders() {
//	return vert_shaders;
//}
//
//char** get_fragment_shaders() {
//	return frag_shaders;
//}


