//
// Created by snipe_000 on 2018/10/23.
//

#include <malloc.h>
#include "rendering.h"

void init_face_buffer(struct FACEBUFFER* facebuffer, int size){
    facebuffer->data=malloc(size*sizeof(struct FACE));
    facebuffer->capasity=size;
    facebuffer->facecount=0;
}

void resize_face_buffer(struct FACEBUFFER* facebuffer, int size){
    facebuffer->data realloc(facebuffer->data, size*sizeof(struct FACE));
    facebuffer->capasity=size;

}

void init_face_buffer_writer(struct FACEBUFFERWRITER* facebufferwriter, struct FACEBUFFER* facebuffer){
    facebufferwriter->facebuffer=facebuffer;
    facebufferwriter->index=0;
}

void fill_face(struct WORLD_BLOCK* node, int direction, struct FACE* face){

    int size=1<<(node->level);

    switch (direction){
        case 0: //x-
            face->vertices[0].x=node->position.x;
            face->vertices[0].y=node->position.y;
            face->vertices[0].z=node->position.z;

            face->vertices[1].x=node->position.x;
            face->vertices[1].y=node->position.y;
            face->vertices[1].z=node->position.z+size;

            face->vertices[2].x=node->position.x;
            face->vertices[2].y=node->position.y+size;
            face->vertices[2].z=node->position.z+size;

            face->vertices[3].x=node->position.x;
            face->vertices[3].y=node->position.y+size;
            face->vertices[3].z=node->position.z;

            break;

        case 1: //x+
            face->vertices[0].x=node->position.x+size;
            face->vertices[0].y=node->position.y;
            face->vertices[0].z=node->position.z;

            face->vertices[1].x=node->position.x+size;
            face->vertices[1].y=node->position.y+size;
            face->vertices[1].z=node->position.z;

            face->vertices[2].x=node->position.x+size;
            face->vertices[2].y=node->position.y+size;
            face->vertices[2].z=node->position.z+size;

            face->vertices[3].x=node->position.x+size;
            face->vertices[3].y=node->position.y;
            face->vertices[3].z=node->position.z+size;

            break;
        case 2: //y-
            face->vertices[0].x=node->position.x;
            face->vertices[0].y=node->position.y;
            face->vertices[0].z=node->position.z;

            face->vertices[1].x=node->position.x+size;
            face->vertices[1].y=node->position.y;
            face->vertices[1].z=node->position.z;

            face->vertices[2].x=node->position.x+size;
            face->vertices[2].y=node->position.y;
            face->vertices[2].z=node->position.z+size;

            face->vertices[3].x=node->position.x;
            face->vertices[3].y=node->position.y;
            face->vertices[3].z=node->position.z+size;

            break;

        case 3: //y+

            face->vertices[0].x=node->position.x;
            face->vertices[0].y=node->position.y+size;
            face->vertices[0].z=node->position.z;

            face->vertices[1].x=node->position.x;
            face->vertices[1].y=node->position.y+size;
            face->vertices[1].z=node->position.z+size;

            face->vertices[2].x=node->position.x+size;
            face->vertices[2].y=node->position.y+size;
            face->vertices[2].z=node->position.z+size;

            face->vertices[3].x=node->position.x+size;
            face->vertices[3].y=node->position.y+size;
            face->vertices[3].z=node->position.z;

            break;

        case 4: //z-

            face->vertices[0].x=node->position.x;
            face->vertices[0].y=node->position.y;
            face->vertices[0].z=node->position.z;

            face->vertices[1].x=node->position.x;
            face->vertices[1].y=node->position.y+size;
            face->vertices[1].z=node->position.z;

            face->vertices[2].x=node->position.x+size;
            face->vertices[2].y=node->position.y+size;
            face->vertices[2].z=node->position.z;

            face->vertices[3].x=node->position.x+size;
            face->vertices[3].y=node->position.y;
            face->vertices[3].z=node->position.z;
            break;

        case 5: //z+

            face->vertices[0].x=node->position.x;
            face->vertices[0].y=node->position.y;
            face->vertices[0].z=node->position.z+size;

            face->vertices[1].x=node->position.x+size;
            face->vertices[1].y=node->position.y;
            face->vertices[1].z=node->position.z+size;

            face->vertices[2].x=node->position.x+size;
            face->vertices[2].y=node->position.y+size;
            face->vertices[2].z=node->position.z+size;

            face->vertices[3].x=node->position.x;
            face->vertices[3].y=node->position.y+size;
            face->vertices[3].z=node->position.z+size;

            break;


        default:
            break;
    }
}



void fill_face_buffer(struct WORLD_BLOCK* node, int type, struct FACEBUFFERWRITER* facebufferwriter){
    if(!node){
        return;
    }


}

//void draw_buffer(struct FACEBUFFER* facebuffer);