#version 460 core

layout (location = 0) in vec3 in_Position;

uniform mat4 world;

uniform mat4 viewProj;

out vec3 fragPos;

void main(void)
{
	fragPos = vec3(world * vec4(in_Position, 1.0f));

	gl_Position = viewProj * vec4(fragPos, 1.0f);
}