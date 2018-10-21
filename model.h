//
// Created by snipe_000 on 2018/10/16.
//

#ifndef WORLD_MODEL_H
#define WORLD_MODEL_H

#include <stdbool.h>


struct VECTOR3I{
    int x;
    int y;
    int z;
};

struct BLOCK_VISUAL_EFFECT{
    bool is_solid;
    unsigned char visible_faces;
    int material;
};

struct WORLD_BLOCK
{
    struct VECTOR3I position;
    struct BLOCK_VISUAL_EFFECT visual_effect;
    int level;
    int type;

    struct WORLD_BLOCK* parent;
    struct WORLD_BLOCK* children[8];
    int index;

};

struct WORLD_CUBE_TRANSFORM{
    float px;
    float py;
    float pz;
    float size;
};

struct WORLD_SIZE_CONFIG{
    float max_distance;
    float min_distance;
    int max_index;
    int max_level;
};



struct WORLD_TREE{
    struct WORLD_BLOCK* root;
};

struct WORLD_BLOCK_QUERY_RESULT{
    struct WORLD_BLOCK* result;
};

typedef struct {
    void* pvertices;
    void* pindices;
}BUFFERDATA;

///
/// \param tree
/// \param block
void insert_block(struct WORLD_BLOCK* tree, struct WORLD_BLOCK* node);


///
/// \param root
/// \return
BUFFERDATA* calc_buffer_data(struct WORLD_TREE* root);

///
/// \param node
/// \return
void* calc_visible_faces(struct WORLD_BLOCK* node);

///
/// \param tree
/// \param x
/// \param y
/// \param z
/// \param level
void query_block(struct WORLD_TREE* tree, int x, int y, int z, int level);

///
/// \param transform
/// \param block
/// \param config
void get_cube_transform(struct WORLD_CUBE_TRANSFORM* transform, struct WORLD_BLOCK* block, struct WORLD_SIZE_CONFIG* config);

int get_index(int x, int y, int z, int level);

struct WORLD_BLOCK* create_block(int x, int y, int z, int level);

#endif //WORLD_MODEL_H


