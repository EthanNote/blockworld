#version 330

in float gray;

out vec4 FragColor;

void main(){	
	FragColor = vec4(abs(gray), 1.0, 0.0, 1.0);
}