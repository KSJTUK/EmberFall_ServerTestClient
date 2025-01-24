#pragma once
class Window {
public:
    Window();
    ~Window();

public:
    GLFWwindow* GetWindow();

private:
    GLFWwindow* mWindow;
};

