#version 460 core

in vec2 texCoord;

out vec4 fragColor;

// 오브젝트의 meterial속성을 구조체로 묶음
struct Meterials {
	sampler2D heightMapTexture;
	vec3 specular;
	float shininess;
};

uniform Meterials meterials;

uniform int textured;
uniform vec3 terrainColor;

void main(void)
{
	if (1 == textured) {
		fragColor = vec4(vec3(texture(meterials.heightMapTexture, texCoord)), 1.0f);
	} else {
		fragColor = vec4(terrainColor, 1.0f);
	}
//	fragColor = vec4(tes_out_normal, 1.0f);
}