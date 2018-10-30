#include "cJSON.h"

#ifndef MATERIALS_H
#define MATERIALS_H


struct FACE_MATERIAL
{
	float final_color[3];
};

struct BLOCK_MATERIAL
{
	struct FACE_MATERIAL face_material[6];
	char name[16];
};

struct BLOCK_MATERIAL_LIST
{
	struct BLOCK_MATERIAL* materials;
	int count;
	int capacity;
};

struct BLOCK_MATERIAL_LIST* create_block_material_list();

//void set_material_json(struct cJSON* json);

//struct FACE_MATERIAL* get_material(const char* name, int direction);


#endif // !MATERIALS_H