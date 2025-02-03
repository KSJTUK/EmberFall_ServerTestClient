#pragma once

class Texture {
public:
	Texture();
	~Texture();

public:
	void SetTextureDefaultOption();
	std::vector<std::vector<float>> LoadHeightMap(const std::string& heightMap, float yScale, float yShift, bool flipOnLoad);
	void LoadTexture(const std::string& textureFilePath, bool flipImageOnLoad);

	void BindingTexture(int textureIndex);
	void BindingTextures();
	void UnBindingTextures();

	UINT32 GetTextureID(int textureIndex);
	const TextureInfo& GetTextureInfo(int textureIndex) const;

private:
	std::vector<TextureInfo> mTextures{ };
	std::vector<CubeMapInfo> mCubeMaps{ };
};

class CubeMapTexture {
public:
	CubeMapTexture();
	~CubeMapTexture();

public:
	void SetCubeMapTextureOption();

	void LoadCubeMapTexture(const std::string& posX, const std::string& negX, const std::string& posY,
		const std::string& negY, const std::string& posZ, const std::string& negZ, bool);

	void BindCubeMap(int cubeMapIndex);
	void UnBindCubeMap();

private:
	std::vector<CubeMapInfo> mCubeMaps{ };
};