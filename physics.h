#pragma once
#include"vector3.h"
#include"model.h"

struct RAY {
	VECTOR3DF start_pos;
	VECTOR3DF end_pos;
};

struct RAY_HIT_INFO {
	VECTOR3DF hit_position;
	WORLD_BLOCK* hit_block;
	int hit_face;
};

void ray_cast(RAY* ray, WORLD_BLOCK* root, RAY_HIT_INFO* output_info);