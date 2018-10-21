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

//#define INDEX(x, y, z, level) (((x)>>(level))&0x01)|(((y)>>((level)-1))&0x02)|(((z)>>((level)-2))&0x04)

int get_index(int x, int y, int z, int level){
//    return (((x)>>(level))&0x01)|(((y)>>((level)-1))&0x02)|(((z)>>((level)-2))&0x04);
    int bitmask=1<<level;
    int bx=(x & bitmask)>>level;
    int by=(y & bitmask)>>level;
    int bz=(z & bitmask)>>level;
    int result=bx|by<<1|bz<<2;
    return result;
}

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

    if(node->position.x >=tree->position.x && node->position.x < tree->position.x | (0x01<<(tree->level-1)) &&
       node->position.y >=tree->position.y && node->position.y < tree->position.y | (0x01<<(tree->level-1)) &&
       node->position.z >=tree->position.z && node->position.z < tree->position.z | (0x01<<(tree->level-1))){

    }
    else{

        return;
    }

    struct WORLD_BLOCK* current_node=tree;
    while (current_node->level - node->level>1){
        int index=get_index(node->position.x, node->position.y, node->position.z, current_node->level-1);

        if(!current_node->children[index]){
            int level_mask=LEVELMASK(current_node->level-1);
            current_node->children[index]=create_block(node->position.x & level_mask, node->position.y & level_mask, node->position.z & level_mask, current_node->level-1);

        }
        current_node=current_node->children[index];
    }
    int index=get_index(node->position.x, node->position.y, node->position.z, current_node->level-1);
    if(!current_node->children[index]){
        current_node->children[index]=node;
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