#include "view.h"
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<GL/GLU.h>
//#include<stdio.h>
#include<math.h>

#include "input.h"

struct CAMERA_OPTION* current_activated_option;
GLFWwindow* main_window;

void create_camera_option(int camera_type, int width, int height, struct CAMERA_OPTION* output_option) {
	output_option->camera_type = camera_type;
	output_option->frame_buffer_option.width = width;
	output_option->frame_buffer_option.height = height;

	output_option->position.x = 3;
	output_option->position.y = 4;
	output_option->position.z = 8;

	output_option->lookat.x = -3;
	output_option->lookat.y = -4;
	output_option->lookat.z = -8;

	output_option->up.x = 0;
	output_option->up.y = 1;
	output_option->up.z = 0;

	if (camera_type == CAMERA_TYPE_PERSPECTIVE) {
		output_option->perspective_option.far_distance = 100;
		output_option->perspective_option.near_distance = 1;
		//output_option->perspective_option.aspect = 1.0*width / height;
		output_option->perspective_option.field_of_view = 60;
	}
}

void apply_camera_option(struct CAMERA_OPTION* option) {
	current_activated_option = option;
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	////gluLookAt(3, 4, 5, 0, 0, 0, 0, 1, 0);
	//gluLookAt(
	//	option->position.x,
	//	option->position.y,
	//	option->position.z,

	//	option->position.x + option->lookat.x,
	//	option->position.y + option->lookat.y,
	//	option->position.z + option->lookat.z,

	//	option->up.x,
	//	option->up.y,
	//	option->up.z
	//);

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(
	//	option->perspective_option.field_of_view,
	//	1.0*option->frame_buffer_option.width / option->frame_buffer_option.height,
	//	option->perspective_option.near_distance,
	//	option->perspective_option.far_distance
	//);
}




struct CAMERA_MOUSE_CONTROL {
	bool is_active;
	double last_x;
	double last_y;
};

struct CAMERA_MOUSE_CONTROL mouse_control;

void camera_frame_update() {
	double x, y;
	input_get_mouse_pos(&x, &y);
	if (mouse_control.is_active) {
		double dx = x - mouse_control.last_x;
		double dy = y - mouse_control.last_y;
		//printf("CAM_MOVE %lf, %lf\n", dx, dy);

		if (current_activated_option->controller_type == 0) {
			current_activated_option->orbit_control.angle_x += dy*0.005;
			current_activated_option->orbit_control.angle_y += dx*0.005;

			double ax = current_activated_option->orbit_control.angle_x;
			double ay = current_activated_option->orbit_control.angle_y;
			double d = current_activated_option->orbit_control.distance;

			double px = cos(ay)*cos(ax)*d;
			double pz = sin(ay)*cos(ax)*d;
			double py = sin(ax)*d;

			current_activated_option->position = current_activated_option->orbit_control.center;
			current_activated_option->lookat = current_activated_option->orbit_control.center;
			current_activated_option->position.x += px;
			current_activated_option->position.y += py;
			current_activated_option->position.z += pz;


		}

		

	}

	mouse_control.last_x = x;
	mouse_control.last_y = y;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(3, 4, 5, 0, 0, 0, 0, 1, 0);
	gluLookAt(
		current_activated_option->position.x,
		current_activated_option->position.y,
		current_activated_option->position.z,

		current_activated_option->lookat.x,
		current_activated_option->lookat.y,
		current_activated_option->lookat.z,

		current_activated_option->up.x,
		current_activated_option->up.y,
		current_activated_option->up.z
	);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		current_activated_option->perspective_option.field_of_view,
		1.0*current_activated_option->frame_buffer_option.width / current_activated_option->frame_buffer_option.height,
		current_activated_option->perspective_option.near_distance,
		current_activated_option->perspective_option.far_distance
	);
}

void cam_key(int key, int action, int mods) {
	if (key == 0) {
		if (action == 1) {
			mouse_control.is_active = true;
		}
		else {
			mouse_control.is_active = false;
		}
	}
}

void setup_view_control() {
	add_key_event_handler(cam_key);
}

void set_main_window(void* window) {
	main_window = window;
}

double last_xpos;
double last_ypos;
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	double dx = xpos - last_xpos;
	double dy = ypos - last_ypos;


	last_xpos = xpos;
	last_ypos = ypos;
}

void activate_camera_control(struct CAMERA_OPTION* option) {
	current_activated_option = option;
	glfwGetCursorPos(main_window, &last_xpos, &last_ypos);
	glfwSetCursorPosCallback(main_window, cursor_position_callback);
}

void deactivate_camera_control(struct CAMERA_OPTION* option);


void init_camera_orbit_control(struct CAMERA_OPTION* option) {
	option->controller_type = 0;
	option->orbit_control.angle_x = 0;
	option->orbit_control.angle_y = 0;
	option->orbit_control.center.x = 0;
	option->orbit_control.center.y = 0;
	option->orbit_control.center.z = 0;
	option->orbit_control.distance = 10;
}