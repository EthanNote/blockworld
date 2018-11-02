#pragma once
#include "vector3.h"

#define CAMERA_TYPE_PERSPECTIVE 0
#define CAMERA_TYPE_ORTHO 1




struct FRAME_BUFFER_OPTION {
	int width;
	int height;
};


struct PERSPECTIVE_OPTION {
	float field_of_view;
	float near_distance;
	float far_distance;
};

struct ORTHO_OPTION {
	float zoom_rate;
};

struct CAMERA_FPS_CONTROL {
	float angle_y;
	float angle_x;
};

struct CAMERA_ORBIT_CONTROL {
	struct VECTOR3F center;
	float distance;
	float angle_y;
	float angle_x;
};

struct CAMERA_OPTION {
	struct VECTOR3F position;
	struct VECTOR3F direction;
	struct VECTOR3F up;
	int camera_type;

	struct FRAME_BUFFER_OPTION frame_buffer_option;

	union {
		struct PERSPECTIVE_OPTION perspective_option;
		struct ORTHO_OPTION ortho_option;
	};

	int controller_type;
	union {
		struct CAMERA_FPS_CONTROL fps_control;
	};
};




void set_main_window(void* window);

void create_camera_option(int camera_type, int width, int height, struct CAMERA_OPTION* output_option);

void apply_camera_option(struct CAMERA_OPTION* option);

void activate_camera_control(struct CAMERA_OPTION* option);

void deactivate_camera_control(struct CAMERA_OPTION* option);
