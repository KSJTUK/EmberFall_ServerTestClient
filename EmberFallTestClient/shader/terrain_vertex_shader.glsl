#version 460 core

layout (location = 0) in vec3 in_Position; //--- 위치 변수: attribute position 0
layout (location = 1) in vec2 in_Texture; //--- 텍스처 변수: attribute position 1
layout (location = 2) in vec3 in_Normal; //--- 정점 노멀 변수: attribute position 2

out vec2 texCoord;

uniform float yScale;
uniform mat4 view;
uniform mat4 projection;
uniform sampler2D heightMap;

vec3 up = vec3(0.0f, 1.0f, 0.0f);

void main(void)
{
	float height = texture(heightMap, in_Texture).r * 255.0f * yScale;
	vec3 result = in_Position + (up * height);
	gl_Position = projection * view * vec4(in_Position, 1.0f);
	texCoord = in_Texture;
}