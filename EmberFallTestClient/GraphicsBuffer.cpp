#include "pch.h"
#include "GraphicsBuffer.h"

GraphicsBuffer::GraphicsBuffer() { }

GraphicsBuffer::~GraphicsBuffer() {
	glDeleteVertexArrays(1, &mVertexArray);
	glDeleteBuffers(1, &mVertexBuffer);
}

void GraphicsBuffer::Init() {
	// VAO ��ü ���� �� ���ε�
	// VBO ��ü ���� �� ���ε�
	glGenVertexArrays(1, &mVertexArray);
	glGenBuffers(1, &mVertexBuffer);

	glBindVertexArray(mVertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
}

void GraphicsBuffer::ResetVerticies(const std::vector<Vertex>& verticies) {
	mVertexDataSize = verticies.size();

	glDeleteVertexArrays(1, &mVertexArray);

	glGenVertexArrays(1, &mVertexArray);
	glGenBuffers(1, &mVertexBuffer);

	glBindVertexArray(mVertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, mVertexDataSize * sizeof(Vertex), &verticies[0], GL_STATIC_DRAW);

	// location 0���� Vertex��ü�� position������ �Ѱ���
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	// location 1���� Vertex��ü�� texture������ �Ѱ���
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(1);

	// location 2���� Vertex��ü�� normal������ �Ѱ���
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
}

void GraphicsBuffer::SetVerticies(const std::vector<Vertex>& verticies) {
	mVertexDataSize = verticies.size();

	// Vertex��ü�� ������ VBO�� �Ѱ���
	glBindVertexArray(mVertexArray);
	glBufferData(GL_ARRAY_BUFFER, mVertexDataSize * sizeof(Vertex), &verticies[0], GL_STATIC_DRAW);

	// location 0���� Vertex��ü�� position������ �Ѱ���
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	// location 1���� Vertex��ü�� texture������ �Ѱ���
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(1);

	// location 2���� Vertex��ü�� normal������ �Ѱ���
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
}

void GraphicsBuffer::SetPatchParameters(int numOfPatches) {
	glPatchParameteri(GL_PATCH_VERTICES, numOfPatches);
}

void GraphicsBuffer::BindingTexture(const UINT32& textureID) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void GraphicsBuffer::SetDrawMode(unsigned int mode) {
	mDrawMode = mode;
}

void GraphicsBuffer::Render() {
	// shaderProgram ���� UseProgram�� Ȱ��ȭ �ߴٴ� �����Ͽ� ����
	glBindVertexArray(mVertexArray);
	glDrawArrays(mDrawMode, 0, UINT32(mVertexDataSize));
	glBindVertexArray(0); // Array ���ε� ����
	glBindTexture(GL_TEXTURE_2D, 0);
}
