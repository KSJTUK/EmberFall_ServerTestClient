#pragma once

#include <gl/glew.h>
#include <gl/GLFW/glfw3.h>
//#include <GL/GLM/glm.hpp>
//#include <GL/GLM/ext.hpp>
//#include <GL/GLM/gtc/matrix_transform.hpp>
//#include <GL/GLM/gtx/vector_angle.hpp>
//#include <GL/GLM/gtx/euler_angles.hpp>

#include "ServerLib/include/pch.h"
#include "ServerLib/include/NetworkCore.h"
#ifdef _DEBUG
#pragma comment(lib, "ServerLib/lib/debug/ServerLib.lib")
#else
#pragma comment(lib, "ServerLib/lib/Release/ServerLib.lib")
#endif