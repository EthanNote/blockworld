cmake_minimum_required(VERSION 3.8)
project(world C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(glfw-3.2.1)
include_directories(glfw-3.2.1/include)

add_executable(world main.c model.h model.c)

#target_link_libraries(world glfw ${GLFW_LIBRARIES})
target_link_libraries(world glfw opengl32 ${OPENGL_glu_LIBRARY} ${GLFW_LIBRARIES})