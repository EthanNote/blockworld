#include "view.h"
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<GL/GLU.h>
#include<stdio.h>
#include<math.h>

#include "input.h"

struct CAMERA* active_camera;
GLFWwindow* main_window;

struct INPUT_AXIS {
	double x;
	double y;
	int _is_enabled;
	int _key_states[4];
};

struct INPUT_AXIS axis;


void create_camera(int camera_type, /*int width, int height, */struct CAMERA* output_option) {
	output_option->camera_type = camera_type;
	/*output_option->frame_buffer_option.width = width;
	output_option->frame_buffer_option.height = height;*/

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

void set_active_camera(struct CAMERA* option) {
	active_camera = option;
}

void camera_frame_update() {
	double x, y;
	input_get_mouse_pos(&x, &y);

	if (active_camera->controller_type == CAMERA_CONTROLLER_ORBIT &&
		active_camera->orbit_control.is_active) {

		double dx, dy;
		drag_get_vector(&dx, &dy);

		active_camera->orbit_control.angle_x += dy * 0.005;
		active_camera->orbit_control.angle_y += dx * 0.005;

		double ax = active_camera->orbit_control.angle_x;
		double ay = active_camera->orbit_control.angle_y;
		double d = active_camera->orbit_control.distance;

		double px = cos(ay)*cos(ax)*d;
		double pz = sin(ay)*cos(ax)*d;
		double py = sin(ax)*d;

		active_camera->position = active_camera->orbit_control.center;
		active_camera->lookat = active_camera->orbit_control.center;
		active_camera->position.x += px;
		active_camera->position.y += py;
		active_camera->position.z += pz;
	}
	if (active_camera->controller_type == CAMERA_CONTROLLER_FPS) {
		double dx, dy;
		/*if (0&&cursor_islocked()) {
			cursor_get_lock_drag(&dx, &dy);
		}
		else {*/
		drag_get_vector(&dx, &dy);
		//}
		active_camera->fps_control.angle_x -= dy * 0.005;
		active_camera->fps_control.angle_y += dx * 0.005;


		double ax = active_camera->fps_control.angle_x;
		double ay = active_camera->fps_control.angle_y;

		double fx = cos(ay)*cos(ax);
		double fz = sin(ay)*cos(ax);
		double fy = sin(ax);

		double rx = -sin(ay);
		double rz = cos(ay);
		double ry = 0;

		double speed_time_dt = 0.05;

		//printf("AXIS: %lf, %lf\n", axis.x, axis.y);

		double mx = fx * axis.y*speed_time_dt + rx * axis.x*speed_time_dt;
		double my = fy * axis.y*speed_time_dt + ry * axis.x*speed_time_dt;
		double mz = fz * axis.y*speed_time_dt + rz * axis.x*speed_time_dt;


		active_camera->fps_control.position.x += mx;
		active_camera->fps_control.position.y += my;
		active_camera->fps_control.position.z += mz;

		active_camera->position = active_camera->fps_control.position;
		active_camera->lookat = active_camera->fps_control.position;
		active_camera->lookat.x += fx;
		active_camera->lookat.y += fy;
		active_camera->lookat.z += fz;



	}





	//mouse_control.last_x = x;
	//mouse_control.last_y = y;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(3, 4, 5, 0, 0, 0, 0, 1, 0);
	gluLookAt(
		active_camera->position.x,
		active_camera->position.y,
		active_camera->position.z,

		active_camera->lookat.x,
		active_camera->lookat.y,
		active_camera->lookat.z,

		active_camera->up.x,
		active_camera->up.y,
		active_camera->up.z
	);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int width = 800;
	int height = 600;
	if (main_window) {
		glfwGetWindowSize(main_window, &width, &height);
		//printf("%d %d\n", width, height);
	}

	gluPerspective(
		active_camera->perspective_option.field_of_view,
		1.0*width / height,
		active_camera->perspective_option.near_distance,
		active_camera->perspective_option.far_distance
	);

	glViewport(0, 0, width, height);

}

void orbit_rotate_toggle(int key, int action, int mods) {
	if (key == 0 && active_camera->controller_type == CAMERA_CONTROLLER_ORBIT) {
		active_camera->orbit_control.is_active = action;
	}
}






void clear_axis() {
	memset(&axis, 0, sizeof(struct INPUT_AXIS));
}

void enable_axis_keys() {
	clear_axis();
	axis._is_enabled = 1;
}

void disable_axis_keys() {
	clear_axis();
}

#define AXIS_KEY_UP GLFW_KEY_W
#define AXIS_KEY_DOWN GLFW_KEY_S
#define AXIS_KEY_LEFT GLFW_KEY_A
#define AXIS_KEY_RIGHT GLFW_KEY_D

void axis_key(int key, int action, int mods) {
	if (!axis._is_enabled) {
		return;
	}

	int new_state = action > 0 ? 1 : 0;

	switch (key)
	{
	case AXIS_KEY_UP:
		axis._key_states[0] = new_state;
		break;
	case AXIS_KEY_DOWN:
		axis._key_states[1] = new_state;
		break;
	case AXIS_KEY_LEFT:
		axis._key_states[2] = new_state;
		break;
	case AXIS_KEY_RIGHT:
		axis._key_states[3] = new_state;
		break;
	default:
		break;
	}

	axis.x = axis._key_states[3] - axis._key_states[2];
	axis.y = axis._key_states[0] - axis._key_states[1];
}


void cam_key(int key, int action, int mods) {
	if (active_camera->controller_type == CAMERA_CONTROLLER_ORBIT) {
		orbit_rotate_toggle(key, action, mods);
	}
	if (active_camera->controller_type == CAMERA_CONTROLLER_FPS) {
		if (key == GLFW_KEY_LEFT_ALT) {
			if (action)
			{
				drag_stop();
				glfwSetInputMode(main_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else
			{
				drag_init();
				glfwSetInputMode(main_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}
	}
}

void setup_view_control() {
	add_key_event_handler(cam_key);
	add_key_event_handler(axis_key);
	enable_axis_keys();
}

void set_main_window(void* window) {
	main_window = window;
}

//double last_xpos;
//double last_ypos;
//static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
//{
//	double dx = xpos - last_xpos;
//	double dy = ypos - last_ypos;
//
//
//	last_xpos = xpos;
//	last_ypos = ypos;
//}

void activate_camera_control(struct CAMERA* option) {
	active_camera = option;
	/*glfwGetCursorPos(main_window, &last_xpos, &last_ypos);
	glfwSetCursorPosCallback(main_window, cursor_position_callback);*/
}

void deactivate_camera_control(struct CAMERA* option);


void init_camera_orbit_control(struct CAMERA* camera) {
	camera->controller_type = CAMERA_CONTROLLER_ORBIT;
	camera->orbit_control.angle_x = 0;
	camera->orbit_control.angle_y = 0;
	camera->orbit_control.center.x = 0;
	camera->orbit_control.center.y = 0;
	camera->orbit_control.center.z = 0;
	camera->orbit_control.distance = 10;
}

void init_camera_fps_control(struct CAMERA* camera) {
	camera->controller_type = CAMERA_CONTROLLER_FPS;
	camera->fps_control.angle_x = 0;
	camera->fps_control.angle_y = 0;

	camera->fps_control.position.x = 0;
	camera->fps_control.position.y = 0;
	camera->fps_control.position.z = 0;

	glfwSetInputMode(main_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}