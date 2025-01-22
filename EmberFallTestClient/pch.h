#pragma once

#include <gl/glew.h>
#include <gl/GLFW/glfw3.h>
//#include <GL/GLM/glm.hpp>
//#include <GL/GLM/ext.hpp>
//#include <GL/GLM/gtc/matrix_transform.hpp>
//#include <GL/GLM/gtx/vector_angle.hpp>
//#include <GL/GLM/gtx/euler_angles.hpp>

#include "../External/Include/ServerLib/pch.h"
#include "../External/Include/ServerLib/NetworkCore.h"
#ifdef _DEBUG
#pragma comment(lib, "../External/Lib/ServerLib/debug/ServerLib.lib")
#else
#pragma comment(lib, "../External/Lib/ServerLib/Release/ServerLib.lib")
#endif