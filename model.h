//
// Created by snipe_000 on 2018/10/16.
//

#ifndef WORLD_MODEL_H
#define WORLD_MODEL_H

#include <stdbool.h>
#include <rpcndr.h>

typedef struct {
    int x;
    int y;
    int z;
}VECTOR3I;

typedef struct {
    bool is_solid;
    byte visible_faces;
    int material;
}BLOCK_VISUAL_EFFECT;

typedef struct {
    VECTOR3I position;
    BLOCK_VISUAL_EFFECT visual_effect;
    int level;
    int type;
}WORLD_BLOCK;

typedef struct {
    float px;
    float py;
    float pz;
    float size;
}WORLD_CUBE_TRANSFORM;

typedef struct {
    float maxRange;
}WORLD_SIZE_CONFIG;


void get_cube_transform(WORLD_CUBE_TRANSFORM* transform, WORLD_BLOCK* block, WORLD_SIZE_CONFIG* config);

typedef struct {
    WORLD_BLOCK* root;
}WORLD_TREE;

void insert_block(WORLD_TREE* tree, WORLD_BLOCK* block);

typedef struct {
    WORLD_BLOCK* result;
}WORLD_BLOCK_QUERY_RESULT;


typedef struct {
    void* pvertices;
    void* pindices;
}BUFFERDATA;

BUFFERDATA* calc_buffer_data(WORLD_TREE* root);
void* calc_visible_faces(WORLD_BLOCK* node);

void query_block(WORLD_TREE* tree, int x, int y, int z, int level);

#endif //WORLD_MODEL_H
