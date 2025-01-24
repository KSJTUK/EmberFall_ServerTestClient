#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "../External/Include/ServerLib/pch.h"
#include "../External/Include/ServerLib/NetworkCore.h"
#ifdef _DEBUG
#pragma comment(lib, "../External/Lib/ServerLib/debug/ServerLib.lib")
#else
#pragma comment(lib, "../External/Lib/ServerLib/Release/ServerLib.lib")
#endif

#include <gl/glew.h>
#include <gl/GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <queue>
#include <filesystem>

inline constexpr UINT32 INVALID_PROGRAM_ID = std::numeric_limits<UINT32>::max();
const std::string EMPTYSTRING{ };
const std::string SHADER_DIR{ "./Shader/" };

const std::vector<std::string> static_shader{
    "static_vertex_shader.glsl",
    EMPTYSTRING,
    EMPTYSTRING,
    EMPTYSTRING,
    "static_fragment_shader.glsl"
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