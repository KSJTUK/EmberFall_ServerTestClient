#pragma once

class GraphicsBuffer {
public:
    GraphicsBuffer();
    ~GraphicsBuffer();

public:
    void Init();

    void ResetVerticies(const std::vector<Vertex>& verticies);

    void SetVerticies(const std::vector<Vertex>& verticies);
    void SetPatchParameters(int numOfPatches);
    void BindingTexture(const UINT32& textureID);

    void SetDrawMode(unsigned int mode);
    void Render();

private:
    UINT32 mDrawMode{ GL_TRIANGLES };
    UINT32 mVertexBuffer{ };
    UINT32 mVertexArray{ };

    size_t mVertexDataSize{ };
    size_t mIndexDataSize{ };
};