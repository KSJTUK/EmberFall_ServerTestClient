#pragma once

using KeyType = UINT16;
inline constexpr KeyType MAX_KEY = 256;

class Input {
public:
    static void Update();
    static std::list<Key>& GetStateChangedKeys();
    static void Clear();

private:
    inline static std::array<BYTE, MAX_KEY> mKeys{ };
    inline static std::array<bool, MAX_KEY> mKeyStates{ };
    inline static std::list<Key> mStateChangedKeys{ };
};

