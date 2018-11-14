#pragma once

#include "physics.h"

struct CRAFTING_BOX {
	int x;
	int y;
	int z;
	int level;
};

void set_crafting_box_position(struct CRAFTING_BOX* box, struct RAY_HIT_INFO* hit_info);


void draw_crafting_box(struct CRAFTING_BOX* box);
