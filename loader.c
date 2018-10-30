//
// Created by snipe_000 on 2018/10/21.
//

#include "loader.h"
#include <stdlib.h>
#include <stdio.h>
#include<string.h>

#include <sys/stat.h>

int file_size2(const char* filename)
{
	struct stat statbuf;
	stat(filename, &statbuf);
	int size = statbuf.st_size;

	return size;
}

#define STR_EQUAL(a, b) strcmp(a,b)==0

struct WORLD_BLOCK* load_world_tree(cJSON* node) {
	struct WORLD_BLOCK* result = create_block(0, 0, 0, -1);
	cJSON* p_attribute = node->child;
	while (p_attribute) {
		// level
		if (STR_EQUAL(p_attribute->string, "level")) {
			result->level = p_attribute->valueint;
		}

		// children
		else if (STR_EQUAL(p_attribute->string, "children")) {
			cJSON* p_index = p_attribute->child;
			while (p_index) {
				int index = -1;
				sscanf_s(p_index->string, "%d", &index);
				result->children[index] = load_world_tree(p_index);
				p_index = p_index->next;
			}
		}

		p_attribute = p_attribute->next;
	}
	return result;
}


cJSON* load_json_file(const char* fname) {
	int size = file_size2(fname);
	char* buffer = (char*)malloc(size);
	FILE* fp = NULL;
	fopen_s(&fp, fname, "r");

	fread(buffer, 1, size, fp);
	fclose(fp);
	cJSON *json = cJSON_Parse(buffer);
	free(buffer);
	return json;
}

void load_world(struct WORLD_TREE* world_tree, const char* fname) {
	// int size = file_size2(fname);
	// char* buffer = (char*)malloc(size);
	// FILE* fp = NULL;
	// fopen_s(&fp, fname, "r");

	// fread(buffer, 1, size, fp);
	// fclose(fp);

	// //    struct WORLD_BLOCK* root=create_block(0,0,0,0);

	// cJSON *json = cJSON_Parse(buffer);

	cJSON* json = load_json_file(fname);

	struct WORLD_BLOCK* root = NULL;
	if (STR_EQUAL(json->child->string, "root")) {
		root = load_world_tree(json->child);
	}
	world_tree->root = root;
	//free(buffer);
}


cJSON* dump_node(struct WORLD_BLOCK* node) {
	cJSON* result = cJSON_CreateObject();
	//    cJSON* children=cJSON_CreateArray();
	cJSON* children = cJSON_CreateObject();

	char name[2] = { '0', '\0' };
	for (int i = 0; i < 8; i++) {
		//        if(!node->children[i]){
		//            cJSON_AddItemToArray(children, cJSON_CreateNull());
		//        } else{
		//            cJSON_AddItemToArray(children, dump_node(node->children[i]));
		//        }

		if (node->children[i]) {
			cJSON_AddItemToObject(children, name, dump_node(node->children[i]));
		}
		name[0]++;
	}
	cJSON_AddItemToObject(result, "children", children);
	cJSON_AddItemToObject(result, "level", cJSON_CreateNumber(node->level));
	return result;
}


void dump_world(struct WORLD_TREE* world_tree, const char* fname) {
	cJSON* world = cJSON_CreateObject();
	cJSON* root = dump_node(world_tree->root);
	cJSON_AddItemToObject(world, "root", root);
	char* datastr = cJSON_Print(world);
	FILE* fp = NULL;
	fopen_s(&fp, fname, "w");
	fputs(datastr, fp);
	fclose(fp);
	free(datastr);

}

//void load_material(const char* fname){
//	cJSON* json=load_json_file(fname);
//	set_material_json(json);
//	cJSON_free(json);
//}

void load_material(const char* fname, struct BLOCK_MATERIAL_LIST* output_block_material_list) {
	cJSON* json = load_json_file(fname);
	cJSON* material_collection_node = json->child;
	cJSON* current_material_node = material_collection_node->child;
	while (current_material_node)
	{
		char* name = current_material_node->string;
		struct BLOCK_MATERIAL block_material;
		strcpy_s(block_material.name, 16, name);
		cJSON* current_face_node = current_material_node->child;
		for (int face = 0; face < 6; face++) {
			cJSON* current_color_value = current_face_node->child;
			for (int color = 0; color < 3; color++) {
				block_material.face_material[face].final_color[color]
					= current_color_value->valuedouble;
				current_color_value = current_color_value->next;
			}
			current_face_node = current_face_node->next;
		}
		
		if (output_block_material_list->capacity <= output_block_material_list->count)
		{
			int new_size = output_block_material_list->count * 2;
			output_block_material_list->materials = realloc(
				output_block_material_list->materials,
				sizeof(struct BLOCK_MATERIAL)*new_size);
			output_block_material_list->capacity = new_size;
		}

		output_block_material_list
			->materials[output_block_material_list->count] = block_material;
		output_block_material_list->count++;
		current_material_node = current_material_node->next;
	}
}