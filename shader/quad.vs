#version 330

layout (location = 0) in vec2 Position;
layout (location = 1) in vec2 TexCoord;

out vec2 pos;

void main()
{	
	gl_Position = vec4(Position, 0.0, 1.0);
	pos = TexCoord;
}