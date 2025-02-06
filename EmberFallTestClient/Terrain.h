#pragma once

const std::vector<std::string> terrain_shader{
	"terrain_vertex_shader.glsl",
	EMPTYSTRING,
	EMPTYSTRING,
	EMPTYSTRING,
	"terrain_fragment_shader.glsl"
};

enum TerrainTex {
	HEIGHT_MAP = 0,
	TERRAIN_TEX
};

enum PatchParam {
	TRIANGLE_PATCH = 3,
	QUAD_PATCH
};

using PixelType = BYTE;

class HeightMap {
public:
	HeightMap() = delete;

	HeightMap(std::string_view imageFile, size_t imageWidth = 0, size_t imageHeight = 0);
	~HeightMap();

	HeightMap(const HeightMap& other);
	HeightMap(HeightMap&& other) noexcept;
	HeightMap& operator=(const HeightMap& other);
	HeightMap& operator=(HeightMap&& other) noexcept;

public:
	size_t ImageWidth() const;
	size_t ImageHeight() const;

	const BYTE* GetPixelData() const;

	float GetPixel(size_t idx) const;
	float GetPixel(size_t x, size_t y) const;
	float GetPixel(const float x, const float y) const;
	float GetPixel(const SimpleMath::Vector2& uv) const;
	size_t Size() const;
	size_t PixelCount() const;

private:
	size_t mWidth{ };
	size_t mHeight{ };
	std::vector<PixelType> mPixels{ };
};

class Terrain {
public:
	Terrain(const glm::vec2& mapSize);
	~Terrain();

public:
	void SetMeterials();

public:
	void Init();
	void Update(float deltaTime);
	void Render(const std::shared_ptr<class Camera>& camera);

private:
	void CreateTerrainMeshMap();

private:
	glm::vec2 mTerrainMapSize{ };
	std::vector<Vertex> m_verticies{ };

	std::unique_ptr<class Texture> mTextureComponent{ };

	std::unique_ptr<class GraphicsBuffer> mVertexBuffer{ };

	glm::uvec2 mTerrainScale{ };

	std::shared_ptr<class Shader> mTerrainShader{ };

	std::shared_ptr<HeightMap> mHeightMap{ };
		
	const float mYScale{ 256.f };
	const float mYShift{ 64.f };
};