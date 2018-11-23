#pragma once
#include "model.h"
#include "view.h"
#include "materials.h"
#include "rendering.h"
#include "physics.h"
#include "crafting.h"
struct SCENE {
	struct WORLD_TREE scene_tree;
	struct WORLD_TREE crafting_tree;
	struct CAMERA camera;
	struct BLOCK_MATERIAL_LIST* material_list;
	struct BUFFER_LIST* scene_buffer_list;
	struct BUFFER_LIST* crafting_buffer_list;

	struct RAY cam_forward_ray;
	struct RAY_HIT_INFO forward_hit_info;
	struct CRAFTING_BOX crafting_box;
};

void scene_activate_crafting_area(struct SCENE* scene, int x, int y, int z, int level);

void scene_render(struct SCENE* scene);

void scene_init(struct SCENE* scene);

void scene_update(struct SCENE* scene);

void scene_enable_crafting(struct SCENE* scene);

void scene_on_crafting_command(struct SCENE* scene);