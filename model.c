//
// Created by snipe_000 on 2018/10/16.
//

#include <malloc.h>
//#include <mem.h>
#include "model.h"
#include<memory.h>

//void get_cube_transform(struct WORLD_CUBE_TRANSFORM* transform, struct WORLD_BLOCK* block, struct WORLD_SIZE_CONFIG* config) {
//	transform->px = config->min_distance*block->position.x;
//	transform->py = config->min_distance*block->position.x;
//	transform->pz = config->min_distance*block->position.x;
//	transform->size = config->min_distance*(0x1 << block->level);
//}

//#define INDEX(x, y, z, level) (((x)>>(level))&0x01)|(((y)>>((level)-1))&0x02)|(((z)>>((level)-2))&0x04)

int get_index(int x, int y, int z, int level) {
	//    return (((x)>>(level))&0x01)|(((y)>>((level)-1))&0x02)|(((z)>>((level)-2))&0x04);
	int bitmask = 1 << level;
	int bx = (x & bitmask) >> level;
	int by = (y & bitmask) >> level;
	int bz = (z & bitmask) >> level;
	int result = bx | by << 1 | bz << 2;
	return result;
}

struct WORLD_BLOCK* find_child_block(struct WORLD_BLOCK* block, int x, int y, int z, int level) {
	int next_level = block->level - 1;
	struct WORLD_BLOCK* presult = block;
	while (next_level >= level) {
		int index_mask = 0x1 << next_level;
		int index = (x & index_mask) | (y & index_mask) << 1 | (z & index_mask) << 2;
		if (!presult->children[index]) {
			break;
		}
		presult = presult->children[index];
		next_level--;
	}
	return presult;
}

//LEVELMASK(level)
//1s and (level+1) 0s

#define LEVELMASK(level) ~((0x1<<((level)+1))-1)

struct WORLD_BLOCK* find_nearby_block(struct WORLD_BLOCK* node, int x, int y, int z, int level) {
	while (node->level < level) {
		node = node->parent;
	}
	int level_mask = LEVELMASK(level);

	while (((node->position.x ^ x) & level_mask) |
		((node->position.y ^ y) & level_mask) |
		((node->position.z ^ z) & level_mask)) {
		node = node->parent;
	}

}

void get_adj_cube(struct WORLD_BLOCK* node, int direction) {

}

void get_adj_cubes(struct WORLD_BLOCK* node, struct WORLD_BLOCK** result) {

}

void* calc_visible_faces(struct WORLD_BLOCK* node) {

}

void calc_visible_nodes(struct WORLD_BLOCK* root, VISIBLE_EVAL_FUNC eval) {
	if (!root) {
		return;
	}
	if (eval) {
		root->visual_effect.is_visible = eval(root);
	}
	int child_count = 0;
	for (int i = 0; i < 8; i++) {
		if (root->children[i]) {
			child_count++;
			calc_visible_nodes(root->children[i], eval);
		}
	}
	if (!child_count) {
		root->visual_effect.is_visible = true;
	}

}

int insert_block(struct WORLD_TREE* tree, struct WORLD_BLOCK* node) {
	struct WORLD_BLOCK* root = tree->root;
	if (root->level <= node->level)
		return 0;

	if (node->position.x >= root->position.x && node->position.x < root->position.x | (0x01 << (root->level - 1)) &&
		node->position.y >= root->position.y && node->position.y < root->position.y | (0x01 << (root->level - 1)) &&
		node->position.z >= root->position.z && node->position.z < root->position.z | (0x01 << (root->level - 1))) {

	}
	else {

		return 0;
	}

	struct WORLD_BLOCK* current_node = root;
	while (current_node->level - node->level > 1) {
		int index = get_index(node->position.x, node->position.y, node->position.z, current_node->level - 1);

		if (!current_node->children[index]) {
			int level_mask = LEVELMASK(current_node->level - 1);
			current_node->children[index] = create_block(node->position.x & level_mask, node->position.y & level_mask, node->position.z & level_mask, current_node->level - 1);
			current_node->children[index]->parent = current_node;
			current_node->children[index]->index = index;
		}
		current_node = current_node->children[index];
	}
	int index = get_index(node->position.x, node->position.y, node->position.z, current_node->level - 1);
	if (!current_node->children[index]) {
		current_node->children[index] = node;
		node->parent = current_node;
		node->index = index;
		tree->dirty = 1;
		return 1;

	}
	return 0;

}

int insert_block_ex(struct WORLD_BLOCK* root, struct WORLD_BLOCK* node, int is_replace, struct WORLD_BLOCK** old_node) {
	if (root->level <= node->level)
		return 0;

	if (node->position.x >= root->position.x && node->position.x < root->position.x | (0x01 << (root->level - 1)) &&
		node->position.y >= root->position.y && node->position.y < root->position.y | (0x01 << (root->level - 1)) &&
		node->position.z >= root->position.z && node->position.z < root->position.z | (0x01 << (root->level - 1))) {

	}
	else {

		return 0;
	}

	struct WORLD_BLOCK* current_node = root;
	while (current_node->level - node->level > 1) {
		int index = get_index(node->position.x, node->position.y, node->position.z, current_node->level - 1);

		if (!current_node->children[index]) {
			int level_mask = LEVELMASK(current_node->level - 1);
			current_node->children[index] = create_block(node->position.x & level_mask, node->position.y & level_mask, node->position.z & level_mask, current_node->level - 1);

		}
		current_node = current_node->children[index];
	}
	int index = get_index(node->position.x, node->position.y, node->position.z, current_node->level - 1);
	if (!current_node->children[index]) {
		current_node->children[index] = node;
		old_node = NULL;
		return 1;

	}
	else {
		*old_node = current_node->children[index];
		current_node->children[index] = node;
		return 1;
	}
	return 0;

}

struct WORLD_BLOCK* create_block(int x, int y, int z, int level) {
	struct WORLD_BLOCK* pblock = (struct WORLD_BLOCK*)malloc(sizeof(struct WORLD_BLOCK));
	memset(pblock, 0, sizeof(struct WORLD_BLOCK));

	pblock->position.x = x;
	pblock->position.y = y;
	pblock->position.z = z;
	pblock->level = level;

	return pblock;
}


void calc_child_node_position(struct WORLD_BLOCK* node) {
	if (node->level < 1) {
		return;
	}
	int inc = 1 << (node->level - 1);

	for (int i = 0; i < 8; i++) {
		if (node->children[i]) {
			struct VECTOR3I* ppos = &(node->children[i]->position);
			*ppos = node->position;
			if (i & 1) {
				ppos->x |= inc;
			}
			if (i & 2) {
				ppos->y |= inc;
			}
			if (i & 4) {
				ppos->z |= inc;
			}
			if (node->children[i]->level != node->level - 1) {
				// fix level error
				node->children[i]->level = node->level - 1;
			}
			struct WORLD_BLOCK* child_node = node->children[i];
			calc_child_node_position(child_node);
		}
	}
}

void calc_positions(struct WORLD_TREE* tree) {
	calc_child_node_position(tree->root);
}

//void add_block_flag(struct WORLD_BLOCK* node, unsigned char flag) {
//	if (!node) {
//		return;
//	}
//	node->visual_effect.blocked_faces |= flag;
//	//if (node->visual_effect.is_visible) {
//	//	return;
//	//}
//	// 000001 0 2 4 6  000 010 100 110
//	// 000010 1 3 5 7  001 011 101 111
//	// 000100 0 1 4 5  000 001 100 101
//	// 001000 2 3 6 7
//	// 010000 0 1 2 3
//	// 100000 4 5 6 7
//	for (unsigned char i = 0; i < 8; i++) {
//		if (!(i&flag)) {
//			add_block_flag(node->children[i], flag);
//		}
//	}
//}
//
//#define add_block_flag_x_negative(node) add_block_flag(node, 0x01)
//#define add_block_flag_x_positive(node) add_block_flag(node, 0x02)
//#define add_block_flag_y_negative(node) add_block_flag(node, 0x04)
//#define add_block_flag_y_positive(node) add_block_flag(node, 0x08)
//#define add_block_flag_z_negative(node) add_block_flag(node, 0x10)
//#define add_block_flag_z_positive(node) add_block_flag(node, 0x20)

void add_block_flag_x_negative(struct WORLD_BLOCK* node) {
	if (!node) { return; }
	node->visual_effect.blocked_faces |= 0x01;
	if (node->visual_effect.is_visible) { return; }

	add_block_flag_x_negative(node->children[0]);
	add_block_flag_x_negative(node->children[2]);
	add_block_flag_x_negative(node->children[4]);
	add_block_flag_x_negative(node->children[6]);
}

void add_block_flag_x_positive(struct WORLD_BLOCK* node) {
	if (!node) { return; }
	node->visual_effect.blocked_faces |= 0x02;
	if (node->visual_effect.is_visible) { return; }

	add_block_flag_x_positive(node->children[1]);
	add_block_flag_x_positive(node->children[3]);
	add_block_flag_x_positive(node->children[5]);
	add_block_flag_x_positive(node->children[7]);
}

void add_block_flag_y_negative(struct WORLD_BLOCK* node) {
	if (!node) { return; }
	node->visual_effect.blocked_faces |= 0x04;
	if (node->visual_effect.is_visible) { return; }

	add_block_flag_y_negative(node->children[0]);
	add_block_flag_y_negative(node->children[1]);
	add_block_flag_y_negative(node->children[4]);
	add_block_flag_y_negative(node->children[5]);
}

void add_block_flag_y_positive(struct WORLD_BLOCK* node) {
	if (!node) { return; }
	node->visual_effect.blocked_faces |= 0x08;
	if (node->visual_effect.is_visible) { return; }

	add_block_flag_y_positive(node->children[2]);
	add_block_flag_y_positive(node->children[3]);
	add_block_flag_y_positive(node->children[6]);
	add_block_flag_y_positive(node->children[7]);
}

void add_block_flag_z_negative(struct WORLD_BLOCK* node) {
	if (!node) { return; }
	node->visual_effect.blocked_faces |= 0x10;
	if (node->visual_effect.is_visible) { return; }

	add_block_flag_z_negative(node->children[0]);
	add_block_flag_z_negative(node->children[1]);
	add_block_flag_z_negative(node->children[2]);
	add_block_flag_z_negative(node->children[3]);
}

void add_block_flag_z_positive(struct WORLD_BLOCK* node) {
	if (!node) { return; }
	node->visual_effect.blocked_faces |= 0x20;
	if (node->visual_effect.is_visible) { return; }

	add_block_flag_z_positive(node->children[4]);
	add_block_flag_z_positive(node->children[5]);
	add_block_flag_z_positive(node->children[6]);
	add_block_flag_z_positive(node->children[7]);
}

void calc_blocked_faces_x(struct WORLD_BLOCK* x1, struct WORLD_BLOCK* x2) {
	if (!x1 || !x2) {
		return;
	}

	if (x1->visual_effect.is_visible) {
		add_block_flag_x_negative(x2);
	}
	if (x2->visual_effect.is_visible) {
		add_block_flag_x_positive(x1);
	}

	calc_blocked_faces_x(x1->children[1], x2->children[0]);
	calc_blocked_faces_x(x1->children[3], x2->children[2]);
	calc_blocked_faces_x(x1->children[5], x2->children[4]);
	calc_blocked_faces_x(x1->children[7], x2->children[6]);
}

void calc_blocked_faces_y(struct WORLD_BLOCK* y1, struct WORLD_BLOCK* y2) {
	if (!y1 || !y2) {
		return;
	}

	if (y1->visual_effect.is_visible) {
		add_block_flag_y_negative(y2);
	}
	if (y2->visual_effect.is_visible) {
		add_block_flag_y_positive(y1);
	}

	calc_blocked_faces_y(y1->children[2], y2->children[0]);
	calc_blocked_faces_y(y1->children[3], y2->children[1]);
	calc_blocked_faces_y(y1->children[6], y2->children[4]);
	calc_blocked_faces_y(y1->children[7], y2->children[5]);
}

void calc_blocked_faces_z(struct WORLD_BLOCK* z1, struct WORLD_BLOCK* z2) {
	if (!z1 || !z2) {
		return;
	}

	if (z1->visual_effect.is_visible) {
		add_block_flag_z_negative(z2);
	}
	if (z2->visual_effect.is_visible) {
		add_block_flag_z_positive(z1);
	}

	calc_blocked_faces_z(z1->children[4], z2->children[0]);
	calc_blocked_faces_z(z1->children[5], z2->children[1]);
	calc_blocked_faces_z(z1->children[6], z2->children[2]);
	calc_blocked_faces_z(z1->children[7], z2->children[3]);
}

void calc_blocked_faces(struct WORLD_BLOCK* node) {

	if (!node) {
		return;
	}

	calc_blocked_faces_x(node->children[0], node->children[1]);
	calc_blocked_faces_x(node->children[2], node->children[3]);
	calc_blocked_faces_x(node->children[4], node->children[5]);
	calc_blocked_faces_x(node->children[6], node->children[7]);

	calc_blocked_faces_y(node->children[0], node->children[2]);
	calc_blocked_faces_y(node->children[1], node->children[3]);
	calc_blocked_faces_y(node->children[4], node->children[6]);
	calc_blocked_faces_y(node->children[5], node->children[7]);

	calc_blocked_faces_z(node->children[0], node->children[4]);
	calc_blocked_faces_z(node->children[1], node->children[5]);
	calc_blocked_faces_z(node->children[2], node->children[6]);
	calc_blocked_faces_z(node->children[3], node->children[7]);

	calc_blocked_faces(node->children[0]);
	calc_blocked_faces(node->children[1]);
	calc_blocked_faces(node->children[2]);
	calc_blocked_faces(node->children[3]);
	calc_blocked_faces(node->children[4]);
	calc_blocked_faces(node->children[5]);
	calc_blocked_faces(node->children[6]);
	calc_blocked_faces(node->children[7]);

}

struct WORLD_BLOCK* find_block(struct WORLD_BLOCK* root, int x, int y, int z, int level) {
	if (root->level <= level)
		return 0;

	if (x >= root->position.x && x < root->position.x | (0x01 << (root->level - 1)) &&
		y >= root->position.y && y < root->position.y | (0x01 << (root->level - 1)) &&
		z >= root->position.z && z < root->position.z | (0x01 << (root->level - 1))) {

	}
	else {

		return 0;
	}

	struct WORLD_BLOCK* current_node = root;
	while (current_node->level - level > 0) {
		int index = get_index(x, y, z, current_node->level - 1);

		if (!current_node->children[index]) {
			/*int level_mask = LEVELMASK(current_node->level - 1);
			current_node->children[index] = create_block(node->position.x & level_mask, node->position.y & level_mask, node->position.z & level_mask, current_node->level - 1);*/
			return current_node;
		}
		current_node = current_node->children[index];
	}

	return current_node;
	/*int index = get_index(node->position.x, node->position.y, node->position.z, current_node->level - 1);
	if (!current_node->children[index]) {
		current_node->children[index] = node;
		old_node = NULL;
		return 1;

	}
	else {
		*old_node = current_node->children[index];
		current_node->children[index] = node;
		return 1;
	}
	return 0;*/
}


void transfer_tree(struct WORLD_TREE* src, struct WORLD_TREE* target, struct WORLD_BLOCK* node) {

}