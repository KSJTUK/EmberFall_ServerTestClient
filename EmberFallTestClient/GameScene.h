#pragma once

class GameScene {
public:
    GameScene(std::shared_ptr<class Window> mainWindow);
    ~GameScene();

public:
    void ProcessPackets(const std::shared_ptr<ClientCore>& core);
    void Update();
    void SendUpdateResult(const std::shared_ptr<ClientCore>& core);
    void Render();

private:
    std::array<std::shared_ptr<class GameObject>, 3> mObjects{ };

    std::shared_ptr<class Window> mMainWindow{ };

    std::unique_ptr<class Terrain> mTerrain{ };
    std::shared_ptr<class Camera> mCamera{ };
    std::unordered_map<std::string, std::shared_ptr<class Shader>> mShaders{ };
    std::unordered_map<SessionIdType, std::shared_ptr<class GameObject>> mPlayers{ };
    //std::vector<std::shared_ptr<class GameObject>> mObjects{ };
    std::vector<std::shared_ptr<class Light>> mLights{ };;
    std::unique_ptr<class GameTimer> mMainTimer{ };
};
