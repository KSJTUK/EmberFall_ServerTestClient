#pragma once

using KeyType = UINT16;
inline constexpr KeyType MAX_KEY = 256;

class Input {
public:
    static bool GetState(KeyType key);
    static void Update();

private:
    inline static std::array<BYTE, MAX_KEY> mKeys{ };
};

