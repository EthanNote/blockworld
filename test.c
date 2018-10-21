//
// Created by snipe_000 on 2018/10/21.
//

#include <malloc.h>
#include <mem.h>
#include "model.h"
#include "loader.h"

struct WORLD_TREE* load_test_world(){
    struct WORLD_TREE* pworld=(struct WORLD_TREE*)malloc(sizeof(struct WORLD_TREE));
    memset(pworld, 0, sizeof(struct WORLD_TREE));
    pworld->root=create_block(0,0,0,2);

}

#define INDEX(x, y, z, level) (((x)>>(level))&0x01)|(((y)>>((level)-1))&0x02)|(((z)>>((level)-2))&0x04)

int main(){
    struct WORLD_TREE* world_tree = load_test_world();
    struct WORLD_BLOCK* testblock=create_block(0,1,2,0);
    insert_block(world_tree->root, testblock);
    return 0;
}

