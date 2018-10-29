#include<Windows.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<GL/GLU.h>

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include "model.h"
#include "rendering.h"

int main() {

	struct WORLD_TREE tree;
	load_world(&tree, "world.json");
	calc_positions(&tree);
	calc_visible_nodes(tree.root, NULL);
	dump_world(&tree, "out.json");

	load_material("materials.json");

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
	glPolygonMode(GL_FRONT, GL_LINE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	struct FACE_BUFFER buffer;
	init_face_buffer(&buffer, 1000);
	fill_face_buffer(tree.root, &buffer);

	feed_buffer(&buffer);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor3f(1, 1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(3, 4, 5, 0, 0, 0, 0, 1, 0);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, 1, 0.1, 10);


		draw_buffer(&buffer);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;

	glfwInit();



	return 0;
}