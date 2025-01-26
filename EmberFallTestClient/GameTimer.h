#pragma once

class GameTimer {
public:
    using Clock = std::chrono::high_resolution_clock;
    using TimePeriod = std::milli;
    using Duration = std::chrono::duration<float, TimePeriod>;
    using TimePoint = std::chrono::high_resolution_clock::time_point;
    
public:
    GameTimer();
    ~GameTimer(); 

public:
    UINT32 GetFPS() const;
    float GetDeltaTime(float timeScale=1.0f) const;

    void Update();
    
private:
    TimePoint mPrevPoint{ };
    UINT32 mFps{ };
    float mDeltaTime{ };
    float mFpsCounter{ 0.0f };
};