#pragma once
#include"vector3.h"
#include"model.h"

struct RAY {
	struct VECTOR3DF start_pos;
	struct VECTOR3DF end_pos;
};

struct RAY_VERTICAL {
	struct VECTOR3DF start_pos;
	double distance;
};

struct RAY_HIT_INFO {
	struct VECTOR3DF hit_position;
	struct WORLD_BLOCK* hit_block;
	int hit_face;
};

int hit_detect(struct RAY* ray, struct WORLD_BLOCK* block, struct HIT_DETECT_CALC* calc_out, struct WORLD_BLOCK** hit_block);

void ray_cast(struct  RAY* ray, struct WORLD_BLOCK* root, struct RAY_HIT_INFO* output_info);