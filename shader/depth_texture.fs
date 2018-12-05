#version 330
uniform sampler2D depth_texture;

in vec2 frag_texcoord;

out vec4 FragColor;

void main(){
	float depth=texture(depth_texture, frag_texcoord).x;
	depth = 1.0 - (1.0 -depth )*25.0;
	FragColor = vec4(depth);
}