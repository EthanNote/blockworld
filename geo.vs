#version 330
layout (location=0) in vec3 position;

uniform mat4 mat_MVP;
uniform mat4 mat_modelview;

out vec3 view_position;

void main(){
	gl_Position = mat_MVP * position;
	view_position = mat_modelview * position;
}