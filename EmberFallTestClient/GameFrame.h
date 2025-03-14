#pragma once

class GameFrame {
public:
    GameFrame();
    ~GameFrame();

public:
    void ResizeWindow();
    void AdvanceFrame();

    void GameLoop();

private:
    std::shared_ptr<class Window> mMainWindow{ };
    std::unique_ptr<class GameScene> mScene{ };
};
