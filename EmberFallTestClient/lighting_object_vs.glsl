#version 460 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_Texture;
layout (location = 2) in vec3 in_Normal;

uniform mat4 world;

uniform mat4 viewProj;

void main(void)
{
	vec3 fragPos = vec3(world * vec4(in_Position, 1.0f));

	gl_Position = viewProj * vec4(fragPos, 1.0f);
}