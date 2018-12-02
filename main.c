#include<Windows.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<GLFW/glfw3native.h>
#include<GL/GLU.h>
#include<time.h>
#include<gl/glew.h>
#include<GLFW/glfw3.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include "scene.h"
#include "shader.h"

struct SCENE scene;

extern int pass_init();
extern void pass_geometry();
extern void pass_quad();
extern void render_scene_trees(struct SCENE* scene);

int main() {

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glewInit();
	/*GLuint vbo;
	glGenBuffers(1, &vbo);*/
	glEnableClientState(GL_VERTEX_ARRAY);
	/*glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);*/
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);



	set_main_window(window);
	input_setup(window);
	////cursor_lock(0, 0);
	scene_init(&scene);
	scene_enable_crafting(&scene);

	drag_init();

	/*GLuint fbo;
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

	char* vs_source = NULL;
	get_file_text("shadow_map.vs", &vs_source);
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, vs_source, strlen(vs_source));*/

	//int fp = glCreateShader;

	//GLuint vs = create_shader_from_source("shadow_map.vs", GL_VERTEX_SHADER);
	//GLuint fs = create_shader_from_source("shadow_map.fs", GL_VERTEX_SHADER);

	pass_init();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

		drag_update();
		camera_frame_update();
	

		scene_update(&scene);

		/* Render here */
	/*	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		scene_render(&scene);*/
		
		//pass_geometry();
		//scene_render(&scene);
		render_scene_trees(&scene);
		pass_quad();
		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
		/*Sleep(1000);*/
	}

	glfwTerminate();
	return 0;

	glfwInit();



	return 0;
}