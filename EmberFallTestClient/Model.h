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
	// VAO, VBO, EBO�� ������ ��ü
	std::unique_ptr<class GraphicsBuffer> m_graphicsBuffer{ };
	//std::unique_ptr<class TextureComponent> m_textureComponent{ };

	// ���� �Ӽ����� ������ vector
	std::vector<Vertex> m_verticies{ };

	// ���� ��ֵ��� ������ vector
	std::vector<glm::vec3> m_vertexNormals{ };
	std::vector<glm::vec3> m_noDuplicatedVertex{ };

	// ������ǥ�� �� �ִ� �ּ��� x, y, z ���� ������ ������
	glm::vec3 m_maxCoord{ };
	glm::vec3 m_minCoord{ };

	std::pair<glm::vec3, glm::vec3> m_boundingBox{ };
};