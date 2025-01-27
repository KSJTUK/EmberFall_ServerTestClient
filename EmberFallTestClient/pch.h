#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <gl/glew.h>
#include <gl/GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <queue>
#include <filesystem>
#include "stb_image.h"

#include "../External/Include/ServerLib/pch.h"
#include "../External/Include/ServerLib/NetworkCore.h"
#ifdef _DEBUG
#pragma comment(lib, "../External/Lib/ServerLib/debug/ServerLib.lib")
#else
#pragma comment(lib, "../External/Lib/ServerLib/Release/ServerLib.lib")
#endif

//#define STAND_ALONE

struct Meterials {
    glm::vec3 specular{ };
    float shininess{ };
};

struct TextureInfo {
    UINT32 id{ };
    INT32 width{ };
    INT32 height{ };
    INT32 nrChannel{ };
};

struct CubeMapInfo {
    enum { SIZE = 6 };
    UINT32 id{ };
    INT32 width[SIZE]{ };
    INT32 height[SIZE]{ };
    INT32 nrChannel[SIZE]{ };
};

inline constexpr auto MOUSE_SENSITIVE = 0.03f;
inline constexpr UINT32 INVALID_PROGRAM_ID = std::numeric_limits<UINT32>::max();
const std::string EMPTYSTRING{ };
const std::string SHADER_DIR{ "./Shader/" };

inline glm::mat4 identity{  1.0f, 0.0f, 0.0f, 0.0f,
                            0.0f, 1.0f, 0.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f, };

const std::vector<std::string> static_shader{
    "static_vertex_shader.glsl",
    EMPTYSTRING,
    EMPTYSTRING,
    EMPTYSTRING,
    "static_fragment_shader.glsl"
};

const std::vector<std::string> light_object_shader{
    "lighting_object_vs.glsl",
    EMPTYSTRING,
    EMPTYSTRING,
    EMPTYSTRING,
    "lighting_object_fs.glsl"
};

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

inline glm::mat4x4 ConvertDXMatToGLMat(SimpleMath::Matrix& mat)
{
    glm::mat4x4 glMat{ };
    SimpleMath::Matrix transposed = mat.Transpose();
    ::memcpy(&glMat, &transposed, sizeof(SimpleMath::Matrix));
    return glMat;
}