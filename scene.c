#include "scene.h"
#include "crafting.h"
#include "materials.h"
#include "loader.h"
#include "input.h"

void scene_activate_crafting_area(struct SCENE* scene, int x, int y, int z, int level) {
	struct WORLD_BLOCK* node = find_block(scene->scene_tree.root, x, y, z, level);
	if (!node->level == level) {
		return;
	}
	if (node->parent && node->parent->children[node->index] == node) {
		node->parent->children[node->index] = NULL;
	}
	insert_block(&scene->crafting_tree, node);
}


void scene_render(struct SCENE* scene) {
	set_active_camera(&scene->camera);
	draw_buffer_list(scene->scene_buffer_list);
	draw_buffer_list(scene->crafting_buffer_list);
	crafting_update(&scene->forward_hit_info);
	if (scene->forward_hit_info.hit_block) {
		draw_block_hit_normal(&scene->forward_hit_info);
		draw_crafting_box(&scene->crafting_box);
	}
}

void scene_init(struct SCENE* scene) {

	struct BLOCK_MATERIAL_LIST* material_list = create_block_material_list();
	load_material("materials.json", material_list);
	struct BUFFER_LIST* scene_buffer_list = create_buffer_list_from_materials(material_list, 1);
	//update_buffer_list_material(scene_buffer_list, material_list);

	struct BUFFER_LIST* crafting_buffer_list = create_buffer_list_from_materials(material_list, 1);

	scene->scene_tree.root = create_block(0, 0, 0, 16);
	scene->crafting_tree.root = create_block(0, 0, 0, 16);

	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			for (int k = 0; k < j; k++) {
				struct WORLD_BLOCK* block = create_block(i, k, j, 0);
				block->visual_effect.is_visible = true;
				insert_block(&scene->scene_tree, block);
			}
		}
	}


	scene_activate_crafting_area(scene, 0, 0, 0, 5);

	calc_positions(&scene->scene_tree);
	//calc_visible_nodes(scene->scene_tree.root, NULL);
	calc_blocked_faces(scene->scene_tree.root);

	fill_buffer_list(scene->scene_tree.root, scene_buffer_list);
	feed_buffer_list(scene_buffer_list);


	calc_positions(&scene->crafting_tree);
	//calc_visible_nodes(scene->crafting_tree.root, NULL);
	calc_blocked_faces(scene->crafting_tree.root);

	fill_buffer_list(scene->crafting_tree.root, crafting_buffer_list);
	feed_buffer_list(crafting_buffer_list);

	scene->material_list = material_list;
	scene->scene_buffer_list = scene_buffer_list;
	scene->crafting_buffer_list = crafting_buffer_list;


	create_camera(CAMERA_TYPE_PERSPECTIVE, &scene->camera);
	set_active_camera(&scene->camera);
	setup_view_control();
	init_camera_fps_control(&scene->camera);

	//scene->material_list = create_block_material_list();
	//load_material("materials.json", &scene->material_list);

	//init_buffer_list_from_materials(&scene->material_list, 1, &scene->scene_buffer_list);

	//fill_buffer_list(scene->scene_tree.root, &scene->scene_buffer_list);
	//update_buffer_list_material(&scene->scene_buffer_list, &scene->material_list);
	//feed_buffer_list(&scene->scene_buffer_list);
}



void scene_update(struct SCENE* scene) {
	scene->cam_forward_ray.start_pos.x = scene->camera.position.x;
	scene->cam_forward_ray.start_pos.y = scene->camera.position.y;
	scene->cam_forward_ray.start_pos.z = scene->camera.position.z;

	scene->cam_forward_ray.end_pos.x = scene->camera.lookat.x;
	scene->cam_forward_ray.end_pos.y = scene->camera.lookat.y;
	scene->cam_forward_ray.end_pos.z = scene->camera.lookat.z;

	struct RAY_HIT_INFO scene_hit, crafting_hit;

	//ray_cast(&scene->cam_forward_ray, scene->scene_tree.root, &scene->forward_hit_info);
	ray_cast(&scene->cam_forward_ray, scene->scene_tree.root, &scene_hit);
	ray_cast(&scene->cam_forward_ray, scene->crafting_tree.root, &crafting_hit);

	if (scene_hit.hit_block && crafting_hit.hit_block) {
		if (scene_hit.hit_distance < crafting_hit.hit_distance) {
			scene->forward_hit_info = scene_hit;
			printf("H1\n");
		}
		else {
			scene->forward_hit_info = crafting_hit;
			printf("H2\n");
		}
	}
	else {
		if (crafting_hit.hit_block) {
			scene->forward_hit_info = crafting_hit;
			printf("H3\n");
		}
		else {
			scene->forward_hit_info = scene_hit;
			printf("H4\n");
		}
	}


	if (scene->forward_hit_info.hit_block) {


		scene->crafting_box.level = 0;
		//crafting_box.level = 0;
		set_crafting_box_position(&scene->crafting_box, &scene->forward_hit_info);
		//draw_crafting_box(&crafting_box);
	}
}

struct SCENE* current_crafting_scene;


void scene_on_crafting_command(struct SCENE* scene) {
	put_block(&current_crafting_scene->crafting_box, &current_crafting_scene->crafting_tree);
	calc_positions(&scene->crafting_tree);
	calc_blocked_faces(scene->crafting_tree.root);
	
	for (int i = 0; i < scene->crafting_buffer_list->count; i++) {
		for (int j = 0; j < 6; j++) {
			clean_face_buffer(&scene->crafting_buffer_list->named_buffers[i].facebuffer[j]);
		}
	}

	fill_buffer_list(scene->crafting_tree.root, scene->crafting_buffer_list);
	feed_buffer_list(scene->crafting_buffer_list);
}

void scene_crafting_key(int key, int action, int mods) {
	if (key == 0 && action) {
		if (current_crafting_scene->forward_hit_info.hit_block) {
			scene_on_crafting_command(current_crafting_scene);
		}
	}
}

void scene_enable_crafting(struct SCENE* scene) {
	current_crafting_scene = scene;
	add_key_event_handler(scene_crafting_key);
}

