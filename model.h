//
// Created by snipe_000 on 2018/10/16.
//

#ifndef WORLD_MODEL_H
#define WORLD_MODEL_H

typedef struct {
    int x;
    int y;
    int z;
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

void query_block(WORLD_TREE* tree, int x, int y, int z, int level);

#endif //WORLD_MODEL_H
