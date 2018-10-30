//
// Created by snipe_000 on 2018/10/21.
//

#ifndef WORLD_LOADER_H
#define WORLD_LOADER_H

#include "model.h"
#include "materials.h"
#include "cJSON.h"

void load_world(struct WORLD_TREE* world_tree, const char* fname);

void dump_world(struct WORLD_TREE* world_tree, const char* fname);

void load_material(const char* fname, struct BLOCK_MATERIAL_LIST* output_block_material_list);

#endif //WORLD_LOADER_H
