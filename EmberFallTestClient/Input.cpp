#include "pch.h"
#include "Input.h"

void Input::Update() {
    auto result = ::GetKeyboardState(mKeys.data());
    CrashExp(result == false, "");

    for (BYTE key{ 0x00 }; auto & keyState : mKeys) {
        bool curState = keyState & 0x80;
        if (curState != mKeyStates[key]) {
            mKeyStates[key] = curState;
            mStateChangedKeys.emplace_back(key, curState);
        }
        ++key;
    }
}

std::list<Key>& Input::GetStateChangedKeys() {
    return mStateChangedKeys;
}

void Input::Clear() {
    mStateChangedKeys.clear();
}
