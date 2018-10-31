#include "materials.h"
#include<string.h>
#include<stdlib.h>

struct BLOCK_MATERIAL_LIST* create_block_material_list() {
	struct BLOCK_MATERIAL_LIST* list = malloc(sizeof(struct BLOCK_MATERIAL_LIST));
	list->capacity = 100;
	list->materials = malloc(sizeof(struct BLOCK_MATERIAL)*list->capacity);
	list->count = 0;
	return list;
}

