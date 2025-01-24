#include "pch.h"
#include "Shader.h"
#include "GameObject.h"

Shader::Shader() { }

Shader::~Shader() {
    mRenderingList.clear();
}

void Shader::RegisterRenderingObject(std::shared_ptr<GameObject> gameObj) {
    mRenderingList.push_back(gameObj);
	gameObj->ResetShader(shared_from_this());
}

void Shader::Render() {
	glUseProgram(mId);
	for (auto& obj : mRenderingList) {
		obj->Render();
	}
	glUseProgram(0);
}

UINT32 Shader::CreateShader(const std::string& shaderPath, UINT32 shaderType) {
	UINT id{};
	GLchar errorlog[512]{};

	std::filesystem::path path{ SHADER_DIR + shaderPath };
	std::ifstream in{ path, std::ios::in };
	if (not in) {
		std::cerr << "ERROR : FAILED TO OPEN FILE " << std::endl;
		exit(EXIT_FAILURE);
	}

	std::stringstream ss{ };
	std::string source{};

    ss << in.rdbuf();
    source = ss.str();

	const char* source_cstr = source.c_str();

	id = glCreateShader(shaderType);
	glShaderSource(id, 1, &source_cstr, NULL);
	glCompileShader(id);

	GLint result{};
	::memset(errorlog, 0, 512);

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (0 == result) {
		glGetShaderInfoLog(id, 512, NULL, errorlog);
		std::cerr << "ERROR : SHADER COMPILE ERROR" << std::endl;
		std::cerr << errorlog << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		std::cerr << "SHADER SUCESSFULLY COMPILED!" << std::endl;
	}

	return id;
}

INT32 Shader::GetUniformLocation(const std::string& locationName) {
	auto result = mUniformLocationMap.find(locationName);
	int location{};

	if (result != mUniformLocationMap.end()) {
		return result->second;
	}

	location = glGetUniformLocation(mId, locationName.c_str());
	mUniformLocationMap.insert(std::make_pair(locationName, location));
#ifdef _DEBUG
	//assert(location != -1);
#else // Release
	if (location == -1) {
		std::cerr << "Error in\nFile : " << __FILE__ << "\n" << "Line : " << __LINE__ << std::endl;
		exit(EXIT_FAILURE);
}
#endif
	return location;
}

void Shader::CreateShaders(const std::vector<std::string>& files) {
	std::array<UINT, 5> programIds{ INVALID_PROGRAM_ID };

	mId = glCreateProgram();

	std::cout << "=======================================================================================" << std::endl;

	for (auto i = 0; i < files.size(); ++i) {
		if (EMPTYSTRING == files[i]) {
			continue;
		}

		switch (i) {
		case 0:
			programIds[i] = CreateShader(files[i], GL_VERTEX_SHADER);
			glAttachShader(mId, programIds[i]);
			break;
		case 1:
			programIds[i] = CreateShader(files[i], GL_GEOMETRY_SHADER);
			glAttachShader(mId, programIds[i]);
			break;
		case 2:
			programIds[i] = CreateShader(files[i], GL_TESS_CONTROL_SHADER);
			glAttachShader(mId, programIds[i]);
			break;
		case 3:
			programIds[i] = CreateShader(files[i], GL_TESS_EVALUATION_SHADER);
			glAttachShader(mId, programIds[i]);
			break;
		case 4:
			programIds[i] = CreateShader(files[i], GL_FRAGMENT_SHADER);
			glAttachShader(mId, programIds[i]);
			break;
		default:
			break;
		}
	}

	std::cout << "=======================================================================================" << std::endl;

	glLinkProgram(mId);

	for (auto& id : programIds) {
		if (INVALID_PROGRAM_ID != id) {
			glDeleteProgram(id);
		}
	}

	GLchar errorlog[512]{};

	ZeroMemory(errorlog, 512);
	GLint result{};
	glGetProgramiv(mId, GL_LINK_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(result, 512, NULL, errorlog);
		std::cerr << "ERROR : SHADER LINK FAILED" << std::endl;
		std::cerr << errorlog << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		std::cerr << "SHADERS SUCESSFULLY CREATED" << std::endl;
	}
}

void Shader::SetUniformMat4(const std::string& valueName, GLenum transpose, const glm::mat4& val) {
	auto location = GetUniformLocation(valueName);
	glUniformMatrix4fv(location, 1, transpose, &val[0][0]);
}

void Shader::SetUniformMat3(const std::string& valueName, GLenum transpose, const glm::mat3& val) {
	auto location = GetUniformLocation(valueName);
	glUniformMatrix3fv(location, 1, transpose, &val[0][0]);
}

void Shader::SetUniformVec4(const std::string& valueName, const glm::vec4& val) {
	auto location = GetUniformLocation(valueName);
	glUniform4fv(location, 1, &val.x);
}

void Shader::SetUniformVec3(const std::string& valueName, const glm::vec3& val) {
	auto location = GetUniformLocation(valueName);
	glUniform3fv(location, 1, &val.x);
}

void Shader::SetUniformVec2(const std::string& valueName, const glm::vec2& val) {
	auto location = GetUniformLocation(valueName);
	glUniform2fv(location, 1, &val.x);
}

void Shader::SetUniformFloat(const std::string& valueName, const float value) {
	auto location = GetUniformLocation(valueName);
	glUniform1f(location, value);
}

void Shader::SetUniformInt(const std::string& valueName, const int value) {
	auto location = GetUniformLocation(valueName);
	glUniform1i(location, value);
}
