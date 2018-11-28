//
// Created by snipe_000 on 2018/10/21.
//

#include <malloc.h>
//#include <mem.h>
#include<memory.h>
#include "model.h"
#include "loader.h"
#include "rendering.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct WORLD_TREE* load_test_world(){
    struct WORLD_TREE* pworld=(struct WORLD_TREE*)malloc(sizeof(struct WORLD_TREE));
    memset(pworld, 0, sizeof(struct WORLD_TREE));
    pworld->root=create_block(0,0,0,2);
	return pworld;
}

#define INDEX(x, y, z, level) (((x)>>(level))&0x01)|(((y)>>((level)-1))&0x02)|(((z)>>((level)-2))&0x04)

//int main(){
//
//    GLFWwindow* window;
//
//    /* Initialize the library */
//    if (!glfwInit())
//        return -1;
//
//    /* Create a windowed mode window and its OpenGL context */
//    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
//    if (!window)
//    {
//        glfwTerminate();
//        return -1;
//    }
//
//    /* Make the window's context current */
//    glfwMakeContextCurrent(window);
//
//
//    struct WORLD_TREE tree;
//    load_world(&tree, "world.json");
//    calc_positions(&tree);
//    dump_world(&tree, "out.json");
//
//    struct FACEBUFFER buffer;
//    init_face_buffer(&buffer, 1000);
//    fill_face_buffer(tree.root, &buffer);
//
//
//    return 0;
//}

