//
// Created by snipe_000 on 2018/10/23.
//

#include <malloc.h>
#include "rendering.h"
#include "glfw-3.2.1/deps/glad/glad.h"
#include <GLFW/glfw3.h>

struct FACEBUFFER_GL_CONTEXT{
    struct FACEBUFFER* facebuffer;
    GLuint vbo;

};

void init_face_buffer(struct FACEBUFFER* facebuffer, int size){
    facebuffer->data=malloc(size*sizeof(struct FACE));
    facebuffer->capasity=size;
    facebuffer->facecount=0;

    struct FACEBUFFER_GL_CONTEXT* gl_context=malloc(sizeof(struct FACEBUFFER_GL_CONTEXT));
    gl_context->facebuffer=facebuffer;
    glGenBuffers(1, &gl_context->vbo);
    facebuffer->low_level_context=gl_context;

    pthread_rwlock_init(&facebuffer->buffer_lock, NULL);
}

void resize_face_buffer(struct FACEBUFFER* facebuffer, int size){
    facebuffer->data=realloc(facebuffer->data, size*sizeof(struct FACE));
    facebuffer->capasity=size;

}

//void init_face_buffer_writer(struct FACEBUFFERWRITER* facebufferwriter, struct FACEBUFFER* facebuffer){
//    facebufferwriter->facebuffer=facebuffer;
//    facebufferwriter->index=0;
//}

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
    face->vertices[4]=face->vertices[0];
    face->vertices[5]=face->vertices[2];
}

//void face_buffer_write(struct FACEBUFFER* facebuffer, struct FACE* face){
//    memcpy(&facebuffer->data[facebuffer->facecount], face, sizeof(struct FACE));
//    facebuffer->facecount++;
//}

void fill_face_buffer(struct WORLD_BLOCK* node, struct FACEBUFFER* facebuffer){
    if(!node){
        return;
    }

    if(node->visual_effect.is_visible){
        for(int i=0;i<6;i++){
            if(node->visual_effect.blocked_faces ^ (0x01<<i)){
                pthread_rwlock_wrlock(&facebuffer->buffer_lock);

                fill_face(node,i,facebuffer->data+facebuffer->facecount);
                facebuffer->facecount++;

                pthread_rwlock_unlock(&facebuffer->buffer_lock);
            }

        }
    }

}

void feed_buffer(struct FACEBUFFER* facebuffer){
    glBindBuffer(GL_ARRAY_BUFFER, ((struct FACEBUFFER_GL_CONTEXT*)facebuffer->low_level_context)->vbo);
    glBufferData(GL_ARRAY_BUFFER, facebuffer->facecount* sizeof(struct FACE), facebuffer->data, GL_STATIC_DRAW);
}

void draw_buffer(struct FACEBUFFER* facebuffer) {

    glBindBuffer(GL_ARRAY_BUFFER, ((struct FACEBUFFER_GL_CONTEXT*)facebuffer->low_level_context)->vbo);
    glDrawArrays(GL_TRIANGLES, 0, facebuffer->facecount*2);
}