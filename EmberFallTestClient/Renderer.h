#pragma once
class Renderer {
public:
    Renderer();
    ~Renderer();

public:
    GLFWwindow* GetWindow();

private:
    GLFWwindow* mWindow;
};

