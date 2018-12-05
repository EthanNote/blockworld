#version 330

layout (location = 0) in vec3 Position;

uniform mat4 ModelView;
uniform mat4 Projection;

out float depth;

void main()
{
	vec4 pos = Projection * ModelView * vec4( Position, 1.0 );
	gl_Position = pos;
	depth=pos.z;
}