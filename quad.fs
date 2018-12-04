#version 330

uniform sampler2D quad_texture;
in vec2 pos;

out vec4 FragColor;

void main(){	
	//FragColor = vec4(pos.x/2+0.5, 1.0, pos.y/2+0.5, 1.0);
	FragColor = texture(quad_texture, pos);
}