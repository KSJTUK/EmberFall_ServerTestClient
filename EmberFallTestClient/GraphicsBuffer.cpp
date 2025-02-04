#include "pch.h"
#include "GraphicsBuffer.h"

GraphicsBuffer::GraphicsBuffer() { }

GraphicsBuffer::~GraphicsBuffer() {
	glDeleteVertexArrays(1, &mVertexArray);
	glDeleteBuffers(1, &mVertexBuffer);
}

void GraphicsBuffer::Init() {
	// VAO 객체 생성 및 바인드
	// VBO 객체 생성 및 바인드
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

	// location 0번에 Vertex객체의 position정보를 넘겨줌
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	// location 1번에 Vertex객체의 texture정보를 넘겨줌
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(1);

	// location 2번에 Vertex객체의 normal정보를 넘겨줌
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
}

void GraphicsBuffer::SetVerticies(const std::vector<Vertex>& verticies) {
	mVertexDataSize = verticies.size();

	// Vertex객체의 정보를 VBO에 넘겨줌
	glBindVertexArray(mVertexArray);
	glBufferData(GL_ARRAY_BUFFER, mVertexDataSize * sizeof(Vertex), &verticies[0], GL_STATIC_DRAW);

	// location 0번에 Vertex객체의 position정보를 넘겨줌
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	// location 1번에 Vertex객체의 texture정보를 넘겨줌
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(1);

	// location 2번에 Vertex객체의 normal정보를 넘겨줌
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
	// shaderProgram 에서 UseProgram을 활성화 했다는 가정하에 수행
	glBindVertexArray(mVertexArray);
	glDrawArrays(mDrawMode, 0, UINT32(mVertexDataSize));
	glBindVertexArray(0); // Array 바인드 해제
	glBindTexture(GL_TEXTURE_2D, 0);
}
