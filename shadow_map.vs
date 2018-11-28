#version 330

layout (location = 0) in vec3 Position;

uniform mat4 gWVP;

out vec2 TexCoordOut;

void main()
{
	gl_Position = gWVP * vec4( Position, 1.0 );
	TexCoordOut = ((gWVP * vec4( Position, 1.0 )).xy + vec2(1.0)) * 0.5;
}