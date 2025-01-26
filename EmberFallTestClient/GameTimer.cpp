#include "pch.h"
#include "GameTimer.h"

GameTimer::GameTimer() 
    : mPrevPoint{ Clock::now() } { }

GameTimer::~GameTimer() { }

UINT32 GameTimer::GetFPS() const {
    return mFps;
}

float GameTimer::GetDeltaTime(float timeScale) const {
    return mDeltaTime * timeScale;
}

void GameTimer::Update() {
    static UINT32 fpsCount{ };
    ++fpsCount;

    auto curPoint = Clock::now();
    
    mDeltaTime = std::chrono::duration_cast<Duration>(curPoint - mPrevPoint).count() / 1000.0f;
    mFpsCounter += mDeltaTime;
    if (1.0f <= mFpsCounter) {
        mFps = fpsCount;
        mFpsCounter = 0.0f;
    }

    mPrevPoint = curPoint;
}
