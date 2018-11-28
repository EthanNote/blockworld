#include<Windows.h>
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include<GL/GLU.h>
#include<stdio.h>
#include <stdlib.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
//#include<GL/glew.h>
//#include<GLFW/glfw3.h>
//#include<GLFW/glfw3native.h>
//#include<gl/glew.h>
//#include<GLFW/glfw3.h>
#include<time.h>

//#pragma comment(lib, "glfw3.lib")
//#pragma comment(lib, "glew32.lib")
//#pragma comment(lib, "opengl32.lib")

#include "input.h"
#include "scene.h"
#include "technique.h"
//#include "shader.h"

struct SCENE scene;
struct TECHNIQUE depth_gray_tech;
struct TECHNIQUE screen_quad_tech;

extern const char* depth_gray_vs;
extern const char* depth_gray_fs;
extern const char* quad_vs;
extern const char* quad_fs;

float quad[] = {
	-1.0f,  1.0f, 0.0f, 0.0f, //vec2 pos, vec2 tex_coord
	-1.0f, -1.0f, 0.0f, 1.0f,
	1.0f,  -1.0f, 1.0f, 1.0f,
	1.0f,  1.0f,  1.0f, 0.0f,
};

//float quad2[] = {
//	-1.0f,  1.0f, 0.0f, //vec2 pos, vec2 tex_coord
//	-1.0f, -1.0f, 0.0f,
//	1.0f,  -1.0f, 0.0f,
//	1.0f,  1.0f,  0.0f,
//};

int main() {
	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(800, 600, "World::Blocks", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	//GLFWwindow* window;

	/* Initialize the library */
	//if (!glfwInit())
	//	return -1;

	///* Create a windowed mode window and its OpenGL context */
	//window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	//if (!window)
	//{
	//	glfwTerminate();
	//	return -1;
	//}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	/*GLuint vbo;
	glGenBuffers(1, &vbo);*/
	glEnableClientState(GL_VERTEX_ARRAY);
	/*glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);*/
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);


	set_main_window(window);
	input_setup(window);
	////cursor_lock(0, 0);
	scene_init(&scene);
	scene_enable_crafting(&scene);

	drag_init();

	GLuint fbo;
	GLuint shadow_map;

	glGenFramebuffers(1, &fbo);
	glGenTextures(1, &shadow_map);
	glBindTexture(GL_TEXTURE_2D, shadow_map);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 640, 480, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_map, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", status);
		return -1;
	}

	GLuint quad_buffer = 0;
	glGenBuffers(1, &quad_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, quad, GL_STATIC_DRAW);



	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	init_shaders();

	struct TECHNIQUE* tech = &depth_gray_tech;
	char buffer[1024];
	technique_init(tech);
	if (!technique_add_shader(tech, depth_gray_vs, GL_VERTEX_SHADER, buffer)) {
		printf("Vertex shader error: \n%s\n", buffer);
	}
	if (!technique_add_shader(tech, depth_gray_fs, GL_FRAGMENT_SHADER, buffer)) {
		printf("Fragment shader error: \n%s\n", buffer);
	}
	technique_finalize(tech);

	tech = &screen_quad_tech;
	technique_init(tech);
	if (!technique_add_shader(tech, quad_vs, GL_VERTEX_SHADER, buffer)) {
		printf("Vertex shader error: \n%s\n", buffer);
	}
	if (!technique_add_shader(tech, quad_fs, GL_FRAGMENT_SHADER, buffer)) {
		printf("Fragment shader error: \n%s\n", buffer);
	}
	technique_finalize(tech);

	GLuint tex = technique_get_uniform_location(tech, "quad_texture");
	glUniform1i(tex, 0);


	

	GLuint loc_ModelView = technique_get_uniform_location(&depth_gray_tech, "ModelView");
	GLuint loc_Projection = technique_get_uniform_location(&depth_gray_tech, "Projection");



	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		drag_update();
		camera_frame_update();


		scene_update(&scene);

		//GLfloat mat_modelview[16];
		//GLfloat mat_projection[16];
		//glGetFloatv(GL_MODELVIEW_MATRIX, mat_modelview);
		//glUniformMatrix4fv(loc_ModelView, 1, GL_FALSE, mat_modelview);
		//glGetFloatv(GL_PROJECTION_MATRIX, mat_projection);
		//glUniformMatrix4fv(loc_Projection, 1, GL_FALSE, mat_projection);


		//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
		//glEnable(GL_DEPTH_TEST);
		//glEnable(GL_TEXTURE_2D);
		//glDrawBuffer(GL_NONE);
		//glReadBuffer(GL_NONE);

		///* Render here */
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//technique_enable(&depth_gray_tech);

		glEnable(GL_VERTEX_ARRAY);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(0);
		scene_render(&scene);
		glDisable(GL_VERTEX_ARRAY);

		//glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		///*glPolygonMode(GL_FRONT, GL_LINE);
		//glPolygonMode(GL_BACK, GL_LINE);*/
		//glBindBuffer(GL_ARRAY_BUFFER, quad_buffer);
		//glDisable(GL_VERTEX_ARRAY);

		//technique_enable(&screen_quad_tech);
		//
		////glVertexPointer(2, GL_FLOAT, 16, 0);
		//glEnableVertexAttribArray(0);
		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, 0);
		//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, 8);

		//glUniform1i(tex, 0);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, shadow_map);
		////glTexCoordPointer(2, GL_FLOAT, 3, quad + 3);
		///*glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, 0);
		//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, 16);*/
		//glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

	}
	glfwTerminate();
	technique_destroy(&depth_gray_tech);
	return 0;

}