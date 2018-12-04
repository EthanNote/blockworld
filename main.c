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
	//glDisableClientState(GL_VERTEX_ARRAY);
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


	pass_init();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

		drag_update();
		camera_frame_update();
	

		scene_update(&scene);
			
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