#include<Windows.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<GL/GLU.h>
#include<time.h>

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include "model.h"
#include "rendering.h"
#include "loader.h"
#include "view.h"
#include "input.h"
#include "physics.h"
#include "crafting.h"

struct CAMERA camera;
//struct CRAFTING_BOX crafting_box;

int main() {

	struct WORLD_TREE tree;
	/*load_world(&tree, "world.json");
	calc_positions(&tree);
	calc_visible_nodes(tree.root, NULL);
	calc_blocked_faces(tree.root);*/

	tree.root = create_block(0, 0, 0, 16);
	for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < 1000; j++) {
			insert_block(&tree, create_block(i, 0, j, 0));
		}
	}
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
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);



	/*struct FACE_BUFFER buffer;
	init_face_buffer(&buffer, 1000);
	fill_face_buffer(tree.root, &buffer);

	feed_buffer(&buffer);*/

	struct BLOCK_MATERIAL_LIST* material_list = create_block_material_list();
	load_material("materials.json", material_list);
	struct BUFFER_LIST* buffer_list = create_buffer_list_from_materials(material_list, 1);
	fill_buffer_list(tree.root, buffer_list);
	update_buffer_list_material(buffer_list, material_list);
	feed_buffer_list(buffer_list);


	create_camera(CAMERA_TYPE_PERSPECTIVE, &camera);
	set_active_camera(&camera);
	set_main_window(window);
	setup_view_control();
	//init_camera_orbit_control(&cam_opt);
	init_camera_fps_control(&camera);
	input_setup(window);

	//cursor_lock(0, 0);
	drag_init();
	init_crafting_control(&tree);

	struct RAY ray;
	struct RAY_HIT_INFO hit_info;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		int clock_begin = 0;
		int time1 = 0;
		int time2 = 0;
		int time3 = 0;
		int time4 = 0;
		clock_begin = clock();

		if (tree.dirty) {
			for (int i = 0; i < buffer_list->count; i++) {
				for (int j = 0; j < 6; j++) {
					clean_face_buffer(&buffer_list->named_buffers[i].facebuffer[j]);
				}
			}
			calc_positions(&tree);
			time1 = clock();
			calc_visible_nodes(tree.root, NULL);
			time2 = clock();
			calc_blocked_faces(tree.root);
			time3 = clock();
			fill_buffer_list(tree.root, buffer_list);
			feed_buffer_list(buffer_list);
			tree.dirty = 0;
			time4 = clock();
			printf("TIME: %d %d %d %d\n", time1 - clock_begin, time2 - clock_begin, time3 - clock_begin, time4 - clock_begin);
		}
		//cursor_update();
		drag_update();
		camera_frame_update();
		ray.start_pos.x = camera.position.x;
		ray.start_pos.y = camera.position.y;
		ray.start_pos.z = camera.position.z;

		ray.end_pos.x = camera.lookat.x;
		ray.end_pos.y = camera.lookat.y;
		ray.end_pos.z = camera.lookat.z;
		ray_cast(&ray, tree.root, &hit_info);

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		set_active_camera(&camera);

		//draw_buffer(&buffer);
		draw_buffer_list(buffer_list);

		crafting_update(&hit_info);
		/*if (hit_info.hit_block) {
			draw_block_hit_normal(&hit_info);
			crafting_box.level = 1;
			set_crafting_box_position(&crafting_box, &hit_info);
			draw_crafting_box(&crafting_box);
		}*/
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