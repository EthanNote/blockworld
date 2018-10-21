//
// Created by snipe_000 on 2018/10/21.
//

#include "loader.h"
#include <stdlib.h>
#include <mem.h>




struct WORLD_TREE* load_test_world(){
    struct WORLD_TREE* pworld=(struct WORLD_TREE*)malloc(sizeof(struct WORLD_TREE));
    memset(pworld, 0, sizeof(struct WORLD_TREE));
    pworld->root=create_block(0,0,0,0);

}