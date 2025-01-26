#pragma once

inline constexpr UINT32 INVALID_SHADER_ID = std::numeric_limits<UINT32>::max();

class Shader : public std::enable_shared_from_this<Shader> {
public:
    Shader();
    ~Shader();

    Shader(const Shader&) = delete;
    Shader(Shader&&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader& operator=(Shader&&) = delete;
    
public:
    void CreateShaders(const std::vector<std::string>& files);

    // Uniform
    // 모든 변수는 표준 자료형을 통해 받아올 것. 
    void SetUniformMat4(const std::string& valueName, GLenum transpose, const glm::mat4& val);
    void SetUniformMat3(const std::string& valueName, GLenum transpose, const glm::mat3& val);

    // MEMO:
    // 아직은 Mat2 가 필요하다고 판단되지 않아서 추가 안함. 필요한경우 같은 포맷으로 추가하길 바람 
    void SetUniformVec4(const std::string& valueName, const glm::vec4& val);
    void SetUniformVec3(const std::string& valueName, const glm::vec3& val);
    void SetUniformVec2(const std::string& valueName, const glm::vec2& val);


    void SetUniformFloat(const std::string& valueName, const float value);
    void SetUniformInt(const std::string& valueName, const int value);

    void SetCamera(std::shared_ptr<class Camera> camera);
    void RegisterRenderingObject(std::shared_ptr<class GameObject> gameObj);
    void Render();

private:
    UINT32 CreateShader(const std::string& shaderPath, UINT32 shaderType);
    INT32 GetUniformLocation(const std::string& locationName);

private:
    UINT32 mId{ };
    std::shared_ptr<class Camera> mCamera{ };
    std::list<std::shared_ptr<class GameObject>> mRenderingList{ };
    std::unordered_map<std::string, INT32> mUniformLocationMap{ };
};