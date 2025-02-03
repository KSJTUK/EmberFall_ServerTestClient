#version 460 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_Texture;
layout (location = 2) in vec3 in_Normal;

uniform mat4 world;

uniform mat4 viewProj;

// lighting
out vec3 normal;
out vec2 texCoords;
out vec3 fragPos;

void main(void)
{
	fragPos = vec3(world * vec4(in_Position, 1.0f));
	normal = mat3(world) * in_Normal;
	texCoords = in_Texture;

	gl_Position = viewProj * vec4(fragPos, 1.0f);
}