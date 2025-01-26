#include "pch.h"
#include "Input.h"
#include "Window.h"

void Input::SetMainWindow(std::shared_ptr<Window> window) {
    mWindow = window;
}

bool Input::GetState(KeyType key) {
    return mKeyStates[key];
}

glm::vec2 Input::GetDeltaMouse() {
    return mDeltaMouse;
}

void Input::Update() {
    // Update Keyboard
    auto result = ::GetKeyboardState(mKeys.data());
    CrashExp(false == result, "");

    for (BYTE key{ 0x00 }; auto & keyState : mKeys) {
        bool curState = keyState & 0x80;
        if (curState != mKeyStates[key]) {
            mStateChangedKeys.emplace_back(key, curState);
        }
        mKeyStates[key] = curState;
        ++key;
    }

    // Update Mouse
    double tx, ty;
    glfwGetCursorPos(mWindow->GetWindow(), &tx, &ty);

    mDeltaMouse.x = static_cast<float>(tx) - mPrevMouse.x;
    mDeltaMouse.y = static_cast<float>(ty) - mPrevMouse.y;

    mPrevMouse.x = static_cast<float>(tx);
    mPrevMouse.y = static_cast<float>(ty);
}

std::list<Key>& Input::GetStateChangedKeys() {
    return mStateChangedKeys;
}

void Input::Clear() {
    mStateChangedKeys.clear();
}
