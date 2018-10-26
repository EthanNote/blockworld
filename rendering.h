//
// Created by snipe_000 on 2018/10/23.
//

#include "model.h"

#ifndef WORLD_RENDERING_H
#define WORLD_RENDERING_H

struct VERTEX{
    float x;
    float y;
    float z;
};

struct FACE{
    struct VERTEX[4] vertices;
};


struct FACEBUFFER{
    struct FACE* data;
    int capasity;
    int facecount;
};

struct FACEBUFFERWRITER{
    struct FACEBUFFER* facebuffer;
    int index;
};

struct LAYER{

};


void init_face_buffer(struct FACEBUFFER* facebuffer, int size);

void resize_face_buffer(struct FACEBUFFER* facebuffer, int size);

void init_face_buffer_writer(struct FACEBUFFERWRITER* facebufferwriter, struct FACEBUFFER* facebuffer   );

void fill_face_buffer(struct WORLD_BLOCK* node, struct FACEBUFFER* facebuffer);

void draw_buffer(struct FACEBUFFER* facebuffer);

#endif //WORLD_RENDERING_H
