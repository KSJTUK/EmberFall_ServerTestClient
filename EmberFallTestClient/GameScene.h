#pragma once

class GameScene {
public:
    GameScene(std::shared_ptr<class Window> mainWindow);
    ~GameScene();

public:
    void RegisterPacketProcessFunctions();
    void ProcessPackets();
    void Update();
    void SendUpdateResult();
    void Render();

private:
    void ProcessNotifyId(PacketHeader* header);
    void ProcessPacketProtocolVersion(PacketHeader* header);
    void ProcessPlayerPacket(PacketHeader* header);
    void ProcessObjectPacket(PacketHeader* header);
    void ProcessObjectDead(PacketHeader* header);
    void ProcessObjectAppeared(PacketHeader* header);
    void ProcessObjectDisappeared(PacketHeader* header);
    void ProcessPlayerExit(PacketHeader* header);
    void ProcessAcquiredItem(PacketHeader* header);
    void ProcessObjectAttacked(PacketHeader* header);
    void ProcessUseItem(PacketHeader* header);
    void ProcessRestoreHP(PacketHeader* header);

private:
    ClientPacketProcessor mPacketProcessor{ };

    std::array<std::shared_ptr<class GameObject>, 200> mObjects{ };

    std::shared_ptr<class Window> mMainWindow{ };

    std::unique_ptr<class Terrain> mTerrain{ };
    std::shared_ptr<class Camera> mCamera{ };
    std::unordered_map<std::string, std::shared_ptr<class Shader>> mShaders{ };
    std::unordered_map<SessionIdType, std::shared_ptr<class GameObject>> mPlayers{ };
    //std::vector<std::shared_ptr<class GameObject>> mObjects{ };
    std::vector<std::shared_ptr<class Light>> mLights{ };;
    std::unique_ptr<class GameTimer> mMainTimer{ };
};
