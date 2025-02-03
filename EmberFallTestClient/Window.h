#pragma once
class Window {
public:
    Window();
    ~Window();

public:
    GLFWwindow* GetWindow();
    
    SimpleMath::Vector2 OnResize();
    void ResizeWindow(INT32 width, INT32 height);

private:
    GLFWwindow* mWindow;
};

