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

struct WORLD_BLOCK* put_block(struct CRAFTING_BOX* box, struct WORLD_TREE* tree);

void init_crafting_control(struct WORLD_TREE* tree);

void crafting_update(struct RAY_HIT_INFO *hit_info);

struct CRAFTING_BOX* get_crafting_box();