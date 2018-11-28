#include "crafting.h"

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "model.h"
#include "view.h"

void set_crafting_box_position(struct CRAFTING_BOX* box, struct RAY_HIT_INFO* hit_info) {
	box->x = hit_info->hit_block->position.x;
	box->y = hit_info->hit_block->position.y;
	box->z = hit_info->hit_block->position.z;
	int mask = (-1) << box->level;
	int crafting_size = 1 << box->level;
	int block_size = 1 << (hit_info->hit_block->level);
	//box->x &= mask;
	//box->y &= mask;
	//box->z &= mask;

	//printf("\rF: %d  L:%d    <%d,%d,%d>", hit_info->hit_face, hit_info->hit_block->level, hit_info->hit_block->position.x, hit_info->hit_block->position.y, hit_info->hit_block->position.z);

	//if (box->level <= hit_info->hit_block->level) {
	switch (hit_info->hit_face)
	{
	case 0:
		box->x &= mask;
		if (box->level <= hit_info->hit_block->level || !((~mask) & hit_info->hit_block->position.x)) {
			box->x -= crafting_size;
		}

		box->y = (int)hit_info->hit_position.y & mask;
		box->z = (int)hit_info->hit_position.z & mask;
		break;
	case 2:
		box->y &= mask;
		if (box->level <= hit_info->hit_block->level || !((~mask) & hit_info->hit_block->position.y)) {
			box->y -= crafting_size;
		}

		box->x = (int)hit_info->hit_position.x & mask;
		box->z = (int)hit_info->hit_position.z & mask;
		break;
	case 4:
		box->z &= mask;
		if (box->level <= hit_info->hit_block->level || !((~mask) & hit_info->hit_block->position.z)) {
			box->z -= crafting_size;
		}

		box->x = (int)hit_info->hit_position.x & mask;
		box->y = (int)hit_info->hit_position.y & mask;
		break;

	case 1:
		if (box->level <= hit_info->hit_block->level || !((~mask) & (hit_info->hit_block->position.x + block_size))) {
			box->x += block_size;
		}
		box->y = (int)hit_info->hit_position.y & mask;
		box->z = (int)hit_info->hit_position.z & mask;
		break;
	case 3:
		if (box->level <= hit_info->hit_block->level || !((~mask) & (hit_info->hit_block->position.y + block_size))) {
			box->y += block_size;
		}
		box->x = (int)hit_info->hit_position.x & mask;
		box->z = (int)hit_info->hit_position.z & mask;
		break;
	case 5:
		if (box->level <= hit_info->hit_block->level || !((~mask) & (hit_info->hit_block->position.z + block_size))) {
			box->z += block_size;
		}
		box->x = (int)hit_info->hit_position.x & mask;
		box->y = (int)hit_info->hit_position.y & mask;
		break;
	default:
		break;
	}

}

static float buffer[72];
void draw_crafting_box(struct CRAFTING_BOX* box) {
	int size = 1 << box->level;
	for (int i = 0; i < 72; i += 3) {
		buffer[i] = box->x;
		buffer[i + 1] = box->y;
		buffer[i + 2] = box->z;
	}
	//[0,0,0, 1,0,0, 0,0,0, 0,1,0, 0,0,0, 0,0,1] 0-17   3,10,17
	//[1,1,0, 0,1,0, 1,1,0, 1,0,0, 1,1,0, 1,1,1] 18-35  18,19,22,24,25,27,30,31,33,34,35
	//[1,0,1, 0,0,1, 1,0,1, 1,1,1, 1,0,1, 1,0,0] 36-53  36,38,41,42,44,45,46,47,48,50,51
	//[0,1,1, 1,1,1, 0,1,1, 0,0,1, 0,1,1, 0,1,0] 54-71  55,56,57,58,59,61,62,65,67,68,70
	static int inc[] = { 3,10,17,18,19,22,24,25,27,30,31,33,34,35,36,38,41,42,44,45,46,47,48,50,51,55,56,57,58,59,61,62,65,67,68,70 };

	for (int i = 0; i < sizeof(inc) / sizeof(int); i++) {
		buffer[inc[i]] += size;
	}
	//glBegin(GL_LINES);
	//for (int i = 0; i < 72; i += 6) {
	//	glVertex3fv(buffer + i);
	//	glVertex3fv(buffer + i + 3);
	//}
	//glEnd();
	//glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &buffer[0]);
	glDrawArrays(GL_LINES, 0, 24);
	//glDisableClientState(GL_VERTEX_ARRAY);
}





struct WORLD_BLOCK* put_block(struct CRAFTING_BOX* box, struct WORLD_TREE* tree) {
	printf("CRF %d %d %d %d\n", box->x, box->y, box->z, box->level);
	struct WORLD_BLOCK* new_block=create_block(box->x, box->y, box->z, box->level);
	new_block->visual_effect.is_visible = true;
	if (!insert_block(tree, new_block)) {
		printf("FAILED\n");
		free(new_block);
		return NULL;
	}
	return new_block;
}



struct CRAFTING_BOX crafting_box;

struct CRAFTING_BOX* get_crafting_box() {
	return &crafting_box;
}

struct WORLD_TREE* crafting_tree;
struct RAY_HIT_INFO *last_hit_info = NULL;



void crafting_update(struct RAY_HIT_INFO *hit_info) {
	last_hit_info = hit_info;
	if (hit_info->hit_block) {
		//draw_block_hit_normal(hit_info);
		crafting_box.level = 0;
		set_crafting_box_position(&crafting_box, hit_info);
		//draw_crafting_box(&crafting_box);
	}
}

void crafting_key(int key, int action, int mods) {
	if (key == 0 && action) {
		if (last_hit_info && last_hit_info->hit_block) {
			put_block(&crafting_box, crafting_tree);
		}
	}
}

void init_crafting_control(struct WORLD_TREE* tree) {
	crafting_tree = tree;
	add_key_event_handler(crafting_key);
}
