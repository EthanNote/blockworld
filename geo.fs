#version 330

uniform vec3 normal;
uniform vec3 color;

layout (location = 0) out vec3 tex_color;
layout (location = 1) out vec3 tex_position;
layout (location = 2) out vec3 tex_normal;

in vec3 view_position;

void main(){
	tex_color = color;
	tex_position = view_position;
	tex_normal = normal;
}
