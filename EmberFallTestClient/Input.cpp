#include "pch.h"
#include "Input.h"

bool Input::GetState(KeyType key) {
    return mKeys[key] & 0x80;
}

void Input::Update() {
    if (not ::GetKeyboardState(mKeys.data())) {
        return;
    }
}
