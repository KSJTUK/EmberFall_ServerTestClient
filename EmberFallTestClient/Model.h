#pragma once

class Model {
public:
	Model(const std::string& objectFilePath);
	Model(const std::string& objectFilePath, const std::string& textureFilePath);
	~Model();

	Model(const Model& other) = delete;
	Model& operator=(const Model& other) = delete;

private:
	void CalcMinMaxVertexElem();
	void MakeBoundingBox();

private:
	void ReadFace(std::stringstream& contents, std::vector<unsigned int>* indiciesVec);
	void ReadVertex(std::stringstream& contents, std::vector<glm::vec3>& positions);
	void ReadVertexTexture(std::stringstream& contents, std::vector<glm::vec2>& textureCoords);
	void ReadVertexNormal(std::stringstream& contents, std::vector<glm::vec3>& normals);

	void ReadObject(const char* filePath);

public:
	// setter
	void SetPatchParameters(int numOfPatches);
	void SetDrawMode(int drawMode);
	//bool ExistTexture() const;

	std::pair<glm::vec3, glm::vec3> GetBoundingBox() const { return m_boundingBox; }

	//void BindingTexture(int textureIndex);

public:
	void Init();
	void Update();
	void Render();

private:
	// VAO, VBO, EBO를 가지는 객체
	std::unique_ptr<class GraphicsBuffer> m_graphicsBuffer{ };
	//std::unique_ptr<class TextureComponent> m_textureComponent{ };

	// 정점 속성들을 저장할 vector
	std::vector<Vertex> m_verticies{ };

	// 정점 노멀들을 저장할 vector
	std::vector<glm::vec3> m_vertexNormals{ };
	std::vector<glm::vec3> m_noDuplicatedVertex{ };

	// 정점좌표들 중 최대 최소인 x, y, z 값을 저장할 변수들
	glm::vec3 m_maxCoord{ };
	glm::vec3 m_minCoord{ };

	std::pair<glm::vec3, glm::vec3> m_boundingBox{ };
};