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
#include "../External/Include/ServerLib/PacketHandler.h"
#ifdef _DEBUG
#pragma comment(lib, "../External/Lib/ServerLib/debug/ServerLib.lib")
#else
#pragma comment(lib, "../External/Lib/ServerLib/Release/ServerLib.lib")
#endif

//#define STAND_ALONE

#define RAND_COLOR static_cast<float>(rand()) / static_cast<float>(RAND_MAX)

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

inline constexpr auto MOUSE_SENSITIVE = 0.09f;
inline constexpr auto EPSILON = std::numeric_limits<float>::epsilon();
inline constexpr auto EPSOLON_VEC3 = SimpleMath::Vector3{ EPSILON, EPSILON, EPSILON };
inline constexpr UINT32 INVALID_PROGRAM_ID = std::numeric_limits<UINT32>::max();
const std::string EMPTYSTRING{ };
const std::string SHADER_DIR{ "./Shader/" };

inline bool gMouse{ false };

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

inline glm::mat4 ConvertDXMatToGLMat(const SimpleMath::Matrix& mat)
{
    glm::mat4 glMat{ };
    glMat[0][0] = mat._11; glMat[0][1] = mat._12; glMat[0][2] = mat._13; glMat[0][3] = mat._14;
    glMat[1][0] = mat._21; glMat[1][1] = mat._22; glMat[1][2] = mat._23; glMat[1][3] = mat._24;
    glMat[2][0] = mat._31; glMat[2][1] = mat._32; glMat[2][2] = mat._33; glMat[2][3] = mat._34;
    glMat[3][0] = mat._41; glMat[3][1] = mat._42; glMat[3][2] = mat._43; glMat[3][3] = mat._44;
    return glMat;
}

inline SimpleMath::Vector3 ConvertVec3(const glm::vec3& v)
{
    return SimpleMath::Vector3{ v.x, v.y, v.z };
}

inline glm::vec3 ConvertVec3(const SimpleMath::Vector3& v)
{
    return glm::vec3{ v.x, v.y, v.z };
}

inline bool IsVector3Zero(const SimpleMath::Vector3& v)
{
    SimpleMath::Vector3 compV{ 
        std::fabs(v.x), std::fabsf(v.y), std::fabs(v.z) };
    return DirectX::XMVector3LessOrEqual(v, EPSOLON_VEC3);
}