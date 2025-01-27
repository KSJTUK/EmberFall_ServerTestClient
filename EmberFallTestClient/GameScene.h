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
    std::shared_ptr<class Window> mMainWindow{ };

    std::shared_ptr<class Camera> mCamera{ };
    std::unordered_map<std::string, std::shared_ptr<class Shader>> mShaders{ };
    std::vector<std::shared_ptr<class GameObject>> mObjects{ };
    std::vector<std::shared_ptr<class Light>> mLights{ };
    std::unique_ptr<class GameTimer> mMainTimer{ };
};
