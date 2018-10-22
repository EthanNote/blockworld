//
// Created by snipe_000 on 2018/10/21.
//

#include "loader.h"
#include <stdlib.h>
#include <mem.h>
#include <stdio.h>

void load_world(struct WORLD_TREE* world_tree, const char* fname){

}


cJSON* dump_node(struct WORLD_BLOCK* node){
    cJSON* result=cJSON_CreateObject();
//    cJSON* children=cJSON_CreateArray();
    cJSON* children=cJSON_CreateObject();

    char name[2]={'0', '\0'};
    for(int i=0;i<8;i++){
//        if(!node->children[i]){
//            cJSON_AddItemToArray(children, cJSON_CreateNull());
//        } else{
//            cJSON_AddItemToArray(children, dump_node(node->children[i]));
//        }

        if(node->children[i]){
            cJSON_AddItemToObject(children, name, dump_node(node->children[i]));
        }
        name[0]++;
    }
    cJSON_AddItemToObject(result, "children", children);
    cJSON_AddItemToObject(result, "level", cJSON_CreateNumber(node->level));
    return result;
}


void dump_world(struct WORLD_TREE* world_tree, const char* fname){
    cJSON* world=cJSON_CreateObject();
    cJSON* root=dump_node(world_tree->root);
    cJSON_AddItemToObject(world, "root", root);
    char* datastr=cJSON_Print(world);
    FILE* fp=fopen(fname, "w");
    fputs(datastr, fp);
    fclose(fp);
    free(datastr);

}



