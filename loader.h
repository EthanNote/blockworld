//
// Created by snipe_000 on 2018/10/21.
//

#ifndef WORLD_LOADER_H
#define WORLD_LOADER_H

#include "model.h"

#include "cJSON/cJSON.h"

void load_world(struct WORLD_TREE* world_tree, const char* fname);

void dump_world(struct WORLD_TREE* world_tree, const char* fname);


#endif //WORLD_LOADER_H
