#include "pch.h"
#include "Terrain.h"
#include "GraphicsBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"

HeightMap::HeightMap(std::string_view imageFilePath, size_t imageWidth, size_t imageHeight) {
	std::filesystem::path path{ imageFilePath };
	size_t fileSize = 0;
	if (0 == imageWidth or 0 == imageHeight) {
		fileSize = std::filesystem::file_size(imageFilePath);
		mWidth = mHeight = static_cast<size_t>(std::sqrt(fileSize));
	}
	else {
		mWidth = imageWidth;
		mHeight = imageHeight;
		fileSize = imageWidth * imageHeight;
	}

	std::ifstream imageFile{ path, std::ios::binary };
	if (not imageFile.is_open()) {
		return;
	}

	mPixels.resize(fileSize / sizeof(PixelType));
	imageFile.read(reinterpret_cast<char*>(mPixels.data()), fileSize);

	for (int z = 0; z < mHeight; ++z) {
		for (int x = 0; x < mWidth; ++x) {
			std::swap(mPixels[x + z * mWidth], mPixels[x + (mHeight- z - 1) * mWidth]); // flip
		}
	}

	std::cout << std::format("Height Map Load Success [File: {}] [Size: {}]\n", imageFilePath, fileSize);
	std::cout << std::format("Height Map Info [Width: {}] [Height: {}]\n", mWidth, mHeight);
}

HeightMap::~HeightMap() { }

HeightMap::HeightMap(const HeightMap& other) {
	mPixels.resize(other.PixelCount());
	std::copy(other.mPixels.begin(), other.mPixels.end(), mPixels.begin());
}

HeightMap::HeightMap(HeightMap&& other) noexcept {
	mPixels = std::move(other.mPixels);
}

HeightMap& HeightMap::operator=(const HeightMap& other) {
	mPixels.resize(other.PixelCount());
	std::copy(other.mPixels.begin(), other.mPixels.end(), mPixels.begin());
	return *this;
}

HeightMap& HeightMap::operator=(HeightMap&& other) noexcept {
	mPixels = std::move(other.mPixels);
	return *this;
}

size_t HeightMap::ImageWidth() const {
	return mWidth;
}

size_t HeightMap::ImageHeight() const {
	return mHeight;
}

const BYTE* HeightMap::GetPixelData() const {
	return mPixels.data();
}

float HeightMap::GetPixel(size_t idx) const {
	return static_cast<float>(mPixels[idx]);
}

float HeightMap::GetPixel(size_t u, size_t v) const {
	return static_cast<float>(mPixels[mWidth * v + u]);
}

float HeightMap::GetPixel(const float u, const float v) const {
	size_t iu{ static_cast<size_t>(u) };
	size_t iv{ static_cast<size_t>(v) };

	if ((iu >= mWidth - 1 or iv >= mHeight - 1) or (0.0f > u or 0.0f > v)) {
		return 0.0f;
	}

	float fu{ u - iu };
	float fv{ v - iv };

	float ltHeight{ GetPixel(iu, iv + 1) };
	float rtHeight{ GetPixel(iu + 1, iv + 1) };
	float lbHeight{ GetPixel(iu, iv) };
	float rbHeight{ GetPixel(iu + 1, iv) };

	float topHeight = std::lerp(ltHeight, rtHeight, fu);
	float bottomHeight = std::lerp(ltHeight, rtHeight, fu);

	return std::lerp(topHeight, bottomHeight, fv);
}

float HeightMap::GetPixel(const SimpleMath::Vector2& uv) const {
	return GetPixel(uv.x, uv.y);
}

size_t HeightMap::Size() const {
	return mPixels.size() * sizeof(PixelType);
}

size_t HeightMap::PixelCount() const {
	return mPixels.size();
}

Terrain::Terrain(const glm::vec2& mapSize) 
	: mTerrainMapSize{ mapSize }, mHeightMap{ std::make_shared<HeightMap>("./textures/HeightMap.raw") } {
	mTerrainShader = std::make_shared<Shader>(terrain_shader);

	mTerrainShader->UseProgram();

	mTextureComponent = std::make_unique<Texture>();
	mTextureComponent->CreateTextureFromRaw(mHeightMap->GetPixelData(), mHeightMap->ImageWidth(), mHeightMap->ImageHeight());

	auto info = mTextureComponent->GetTextureInfo(HEIGHT_MAP);
	mTerrainScale = { info.width, info.height };
	mTextureComponent->LoadTexture("./textures/terrain1.png", false);

	mVertexBuffer = std::make_unique<GraphicsBuffer>();
	mVertexBuffer->Init();

	CreateTerrainMeshMap();
	mVertexBuffer->SetVerticies(m_verticies);

	Init();
	mTerrainShader->UnuseProgram();
}

Terrain::~Terrain() { }

void Terrain::SetMeterials() {
	mTerrainShader->SetUniformInt("heightMap", 0);
	mTerrainShader->SetUniformFloat("yScale", mYScale);
	mTerrainShader->SetUniformInt("meterials.heightMapTexture", 1);
	mTerrainShader->SetUniformVec3("meterials.specular", glm::vec3{ 0.f });
	mTerrainShader->SetUniformFloat("meterials.shininess", 128.f);
}

void Terrain::Init() {
	//TERRAINSHADER->SetUniformFloat("yScale", m_yScale);
	//TERRAINSHADER->SetUniformFloat("yShift", m_yShift);
}

void Terrain::Update(float deltaTime) {

}

void Terrain::Render(const std::shared_ptr<Camera>& camera) {
	mTerrainShader->UseProgram();
	SetMeterials();
	
	mTerrainShader->SetUniformInt("textured", static_cast<INT32>(mTextured));
	mTerrainShader->SetUniformVec3("terrainColor", ConvertVec3(mColor));
	mTerrainShader->SetUniformVec3("viewPosition", ConvertVec3(camera->GetPosition()));
	mTerrainShader->SetUniformMat4("projection", GL_FALSE, camera->GetProjMat());
	mTerrainShader->SetUniformMat4("view", GL_FALSE, camera->GetViewMat());

	mTextureComponent->BindingTexture(HEIGHT_MAP);
	mTextureComponent->BindingTexture(TERRAIN_TEX);

	mVertexBuffer->SetDrawMode(GL_TRIANGLES);
	mVertexBuffer->Render();
	mTerrainShader->UnuseProgram();
}

float Terrain::GetHeight(const float x, const float y, const float offset) const {
	float idxX = (x + mTerrainMapSize.x / 2.0f) / mTileSize.x;
	float idxZ = (y + mTerrainMapSize.y / 2.0f) / mTileSize.y;

	float pixel = mHeightMap->GetPixel(idxX, idxZ);
	return pixel * mYScale + offset;
}

void Terrain::SetTextured(bool textured) {
	mTextured = textured;
}

void Terrain::SetColor(const SimpleMath::Vector3& color) {
	mColor = color;
}

void Terrain::CreateTerrainMeshMap() {
	auto heightMapInfo = mTextureComponent->GetTextureInfo(HEIGHT_MAP);
	float tileWidth = static_cast<float>(mTerrainMapSize.x) / static_cast<float>(heightMapInfo.width);
	float tileHeight = static_cast<float>(mTerrainMapSize.y) / static_cast<float>(heightMapInfo.height);
	float left = (-mTerrainMapSize.x / 2.f);
	float bottom = (-mTerrainMapSize.y / 2.f);
	glm::vec3 terrainNorm{ 0.f, 1.f, 0.f };
	mTileSize = { tileWidth, tileHeight };

	// xz평면 상에 지형을 그려줄 삼각형 리스트 정점 생성
	for (unsigned int z = 0; z < heightMapInfo.height; ++z) {
		for (unsigned int x = 0; x < heightMapInfo.width; ++x) {
			// 네 개의 꼭짓점 정의
			Vertex p0{
				glm::vec3{ left + static_cast<float>(x * tileWidth), 0.f, bottom + static_cast<float>(z * tileHeight) },
				terrainNorm,
				glm::vec2{ static_cast<float>(x) / heightMapInfo.width, static_cast<float>(z) / heightMapInfo.height }
			};

			Vertex p1{
				glm::vec3{ left + static_cast<float>((x + 1) * tileWidth), 0.f, bottom + static_cast<float>(z * tileHeight) },
				terrainNorm,
				glm::vec2{ static_cast<float>(x + 1) / heightMapInfo.width, static_cast<float>(z) / heightMapInfo.height }
			};

			Vertex p2{
				glm::vec3{ left + static_cast<float>(x * tileWidth), 0.f, bottom + static_cast<float>((z + 1) * tileHeight) },
				terrainNorm,
				glm::vec2{ static_cast<float>(x) / heightMapInfo.width, static_cast<float>(z + 1) / heightMapInfo.height }
			};

			Vertex p3{
				glm::vec3{ left + static_cast<float>((x + 1) * tileWidth), 0.f, bottom + static_cast<float>((z + 1) * tileHeight) },
				terrainNorm,
				glm::vec2{ static_cast<float>(x + 1) / heightMapInfo.width, static_cast<float>(z + 1) / heightMapInfo.height }
			};

			p0.position.y = GetHeight(p0.position.x, p0.position.z, 0.0f);
			p1.position.y = GetHeight(p1.position.x, p1.position.z, 0.0f);
			p2.position.y = GetHeight(p2.position.x, p2.position.z, 0.0f);
			p3.position.y = GetHeight(p3.position.x, p3.position.z, 0.0f);

			// Triangle 1: p0 -> p2 -> p1
			m_verticies.push_back(p0);
			m_verticies.push_back(p2);
			m_verticies.push_back(p1);

			// Triangle 2: p1 -> p2 -> p3
			m_verticies.push_back(p1);
			m_verticies.push_back(p2);
			m_verticies.push_back(p3);
		}
	}

}
