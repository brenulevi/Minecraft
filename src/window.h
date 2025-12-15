#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <functional>

class Window
{
public:
    Window(int width, int height, const char* title);
    ~Window();

    void pollEvents();
    void swapBuffers();

    void setVSync(bool enabled);
    void toggleFullscreen();

    void setCloseCallback(const std::function<void()>& callback);
    void setResizeCallback(const std::function<void(int, int)>& callback);

    inline GLFWwindow* getGLFWwindow() const { return _id; }
    inline int getWidth() const { return _width; }
    inline int getHeight() const { return _height; }
    inline const char* getTitle() const { return _title; }
    inline bool isVSyncEnabled() const { return _vsyncEnabled; }
    inline bool isFullscreen() const { return _isFullscreen; }

private:
    GLFWwindow* _id;
    int _width;
    int _height;
    const char* _title;
    bool _vsyncEnabled;
    bool _isFullscreen;
    std::function<void()> _closeCallback;
    std::function<void(int, int)> _resizeCallback;
};