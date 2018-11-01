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
#include "loader.h"
#include "view.h"

int main() {

	struct WORLD_TREE tree;
	load_world(&tree, "world.json");
	calc_positions(&tree);
	calc_visible_nodes(tree.root, NULL);
	calc_blocked_faces(tree.root);
	dump_world(&tree, "out.json");

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
	//glPolygonMode(GL_FRONT, GL_LINE);
	//glPolygonMode(GL_BACK, GL_LINE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	/*struct FACE_BUFFER buffer;
	init_face_buffer(&buffer, 1000);
	fill_face_buffer(tree.root, &buffer);

	feed_buffer(&buffer);*/

	struct BLOCK_MATERIAL_LIST* material_list = create_block_material_list();
	load_material("materials.json", material_list);
	struct BUFFER_LIST* buffer_list = create_buffer_list_from_materials(material_list, 100);
	fill_buffer_list(tree.root, buffer_list);
	update_buffer_list_material(buffer_list, material_list);
	feed_buffer_list(buffer_list);

	struct CAMERA_OPTION cam_opt;
	create_camera_option(CAMERA_TYPE_PERSPECTIVE, 640, 480, &cam_opt);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		apply_camera_option(&cam_opt);


		//draw_buffer(&buffer);
		draw_buffer_list(buffer_list);

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