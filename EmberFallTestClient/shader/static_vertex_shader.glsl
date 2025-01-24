#version 460 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_Texture;
layout (location = 2) in vec3 in_Normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 M_matrix;
uniform mat4 TIM_matrix;

uniform mat4 VP;

// lighting
out vec3 normal;
out vec2 texCoords;
out vec3 fragPos;

void main(void)
{
	fragPos = vec3(M_matrix * vec4(in_Position, 1.0f));
	normal = normalize( mat3(TIM_matrix) * in_Normal);
	texCoords = in_Texture;

	gl_Position = VP * vec4(fragPos, 1.0f);
}