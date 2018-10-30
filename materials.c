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


//struct BLOCK_MATERIAL_LIST* material_list = NULL;

//void set_material_json(struct cJSON* json) {
//	if (!material_list) {
//		material_list = malloc(sizeof(struct BLOCK_MATERIAL_LIST));
//		material_list->count = 0;
//		material_list->capacity = 200;
//		material_list->materials =
//			malloc(
//				sizeof(struct BLOCK_MATERIAL)* material_list->capacity
//			);
//	}
//
//	cJSON* material_collection_node = json->child;
//	cJSON* current_material_node = material_collection_node->child;
//	while (current_material_node)
//	{
//		char* name = current_material_node->string;
//		struct BLOCK_MATERIAL block_material;
//		strcpy_s(block_material.name, 16, name);
//		cJSON* current_face_node = current_material_node->child;
//		for (int face = 0; face < 6; face++) {
//			cJSON* current_color_value = current_face_node->child;
//			for (int color = 0; color < 3; color++) {
//				block_material.face_material[face].final_color[color]
//					= current_color_value->valuedouble;
//				current_color_value = current_color_value->next;
//			}
//			current_face_node = current_face_node->next;
//		}
//		//memccpy(&material_list->materials[material_list->count], &block_material, )
//		/*memcpy_s(
//			&material_list->materials[material_list->count], sizeof(struct BLOCK_MATERIAL),
//			&block_material, sizeof(struct BLOCK_MATERIAL)
//		);*/
//		material_list->materials[material_list->count] 
//			= block_material;
//		material_list->count++;
//		current_material_node = current_material_node->next;
//	}
//
//}

//struct FACE_MATERIAL* get_material(const char* name, int direction) {
//	if (!material_list || direction < 0 || direction>5) {
//		return NULL;
//	}
//
//	for (int i = 0; i < material_list->count; i++) {
//		if (strcmp(name, material_list->materials[i].name) == 0) {
//			return &material_list->materials[i].face_material[direction];
//		}
//	}
//}