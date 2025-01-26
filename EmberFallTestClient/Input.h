#pragma once

using KeyType = UINT16;
inline constexpr KeyType MAX_KEY = 256;

class Input {
public:
    static void SetMainWindow(std::shared_ptr<class Window> window);
    static bool GetState(KeyType key);
    static glm::vec2 GetDeltaMouse();
    static void Update();
    static std::list<Key>& GetStateChangedKeys();
    static void Clear();

private:
    inline static std::shared_ptr<class Window> mWindow{ };
    inline static std::array<BYTE, MAX_KEY> mKeys{ };
    inline static std::array<bool, MAX_KEY> mKeyStates{ };
    inline static std::list<Key> mStateChangedKeys{ };
    inline static glm::vec2 mDeltaMouse{ };
    inline static glm::vec2 mPrevMouse{ };
};

