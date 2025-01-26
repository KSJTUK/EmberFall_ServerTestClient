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
    // ��� ������ ǥ�� �ڷ����� ���� �޾ƿ� ��. 
    void SetUniformMat4(const std::string& valueName, GLenum transpose, const glm::mat4& val);
    void SetUniformMat3(const std::string& valueName, GLenum transpose, const glm::mat3& val);

    // MEMO:
    // ������ Mat2 �� �ʿ��ϴٰ� �Ǵܵ��� �ʾƼ� �߰� ����. �ʿ��Ѱ�� ���� �������� �߰��ϱ� �ٶ� 
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