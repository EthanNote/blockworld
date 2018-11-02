#include "view.h"
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<GL/GLU.h>

#include "input.h"

void create_camera_option(int camera_type, int width, int height, struct CAMERA_OPTION* output_option){
	output_option->camera_type = camera_type;
	output_option->frame_buffer_option.width = width;
	output_option->frame_buffer_option.height = height;
	
	output_option->position.x = 3;
	output_option->position.y = 4;
	output_option->position.z = 8;

	output_option->direction.x = -3;
	output_option->direction.y = -4;
	output_option->direction.z = -8;

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
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(3, 4, 5, 0, 0, 0, 0, 1, 0);
	gluLookAt(
		option->position.x, 
		option->position.y, 
		option->position.z,

		option->position.x + option->direction.x,
		option->position.y + option->direction.y,
		option->position.z + option->direction.z,

		option->up.x,
		option->up.y,
		option->up.z
		);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		option->perspective_option.field_of_view,
		1.0*option->frame_buffer_option.width/ option->frame_buffer_option.height, 
		option->perspective_option.near_distance, 
		option->perspective_option.far_distance
	);
}

CAMERA_OPTION* current_activated_option;
GLFWwindow* main_window

void set_main_window(void* window){
    main_window=window;
}

double last_xpos;
double last_ypos
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    double dx=xpos-last_xpos;
    double dy=ypos-last_ypos;


    last_xpos=xpos;
    last_ypos=ypos;
}

void activate_camera_control(struct CAMERA_OPTION* option){
    current_activated_option=ortho_option;
    glfwGetCursorPos(main_window, &last_xpos, &last_ypos);
    glfwSetCursorPosCallback(main_window, cursor_pos_callback);
}

void deactivate_camera_control(struct CAMERA_OPTION* option);
