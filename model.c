//
// Created by snipe_000 on 2018/10/16.
//

#include <malloc.h>
#include <mem.h>
#include "model.h"

void get_cube_transform(struct WORLD_CUBE_TRANSFORM* transform, struct WORLD_BLOCK* block, struct WORLD_SIZE_CONFIG* config){
    transform->px=config->min_distance*block->position.x;
    transform->py=config->min_distance*block->position.x;
    transform->pz=config->min_distance*block->position.x;
    transform->size=config->min_distance*(0x1<<block->level);
}

#define INDEX(x, y, z, level)

struct WORLD_BLOCK* find_child_block(struct WORLD_BLOCK* block, int x, int y, int z, int level){
    int next_level=block->level-1;
    struct WORLD_BLOCK* presult=block;
    while (next_level>=level){
        int index_mask=0x1<<next_level;
        int index= (x & index_mask) | (y & index_mask) << 1 | (z & index_mask) << 2;
        if(!presult->children[index]){
            break;
        }
        presult=presult->children[index];
        next_level--;
    }
    return presult;
}

//LEVELMASK(level)
//1s and (level+1) 0s

#define LEVELMASK(level) ~((0x1<<((level)+1))-1)

struct WORLD_BLOCK* find_nearby_block(struct WORLD_BLOCK* node, int x, int y, int z, int level){
    while(node->level<level){
        node=node->parent;
    }
    int level_mask=LEVELMASK(level);

    while ( ((node->position.x ^ x) & level_mask) |
            ((node->position.y ^ y) & level_mask)|
            ((node->position.z ^ z) & level_mask)){
        node=node->parent;
    }

}

void get_adj_cube(struct WORLD_BLOCK* node, int direction){

}

void get_adj_cubes(struct WORLD_BLOCK* node, struct WORLD_BLOCK** result){

}

void* calc_visible_faces(struct WORLD_BLOCK* node){

}

void insert_block(struct WORLD_BLOCK* tree, struct WORLD_BLOCK* node){
    if(tree->level<=node->level)
        return;

    if(node->position.x >=tree->position.x && node->position.x>>1 < tree->position.x &&
       node->position.y >=tree->position.y && node->position.y>>1 < tree->position.y &&
       node->position.z >=tree->position.z && node->position.z>>1 < tree->position.z){

    }
    else{
        return;
    }

    int current_level=tree->level;
    struct WORLD_BLOCK* current_node=tree;
    while (current_node->level - node->level>1){

    }

}

struct WORLD_BLOCK* create_block(int x, int y, int z, int level){
    struct WORLD_BLOCK* pblock=(struct WORLD_BLOCK*)malloc(sizeof(struct WORLD_BLOCK));
    memset(pblock, 0, sizeof(struct WORLD_BLOCK));

    pblock->position.x=x;
    pblock->position.y=y;
    pblock->position.z=z;
    pblock->level=level;

    return pblock;
}