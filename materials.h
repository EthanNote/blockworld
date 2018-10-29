#include "cJSON.h"

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

void set_material_json(struct cJSON* json);

struct FACE_MATERIAL* get_material(const char* name, int direction);