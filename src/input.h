#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdexcept>

enum CursorMode
{
    CURSOR_MODE_NORMAL = GLFW_CURSOR_NORMAL,
    CURSOR_MODE_HIDDEN = GLFW_CURSOR_HIDDEN,
    CURSOR_MODE_DISABLED = GLFW_CURSOR_DISABLED
};

enum Key
{
    KEY_W = GLFW_KEY_W,
    KEY_A = GLFW_KEY_A,
    KEY_S = GLFW_KEY_S,
    KEY_D = GLFW_KEY_D,
    KEY_SPACE = GLFW_KEY_SPACE,
    KEY_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT,
    KEY_ESCAPE = GLFW_KEY_ESCAPE,
    KEY_F11 = GLFW_KEY_F11
};

enum MouseButton
{
    MOUSE_BUTTON_LEFT = GLFW_MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_RIGHT = GLFW_MOUSE_BUTTON_RIGHT
};

class Input
{
public:
    Input(GLFWwindow* window);
    ~Input();

    void update();

    inline bool isKeyPressed(int key) const
    {
        return _currentKeys[key];
    }

    inline bool isKeyJustPressed(int key) const
    {
        return _currentKeys[key] && !_previousKeys[key];
    }

    inline bool isMouseButtonPressed(int button) const
    {
        return _currentMouseButtons[button];
    }

    inline bool isMouseButtonJustPressed(int button) const
    {
        return _currentMouseButtons[button] && !_previousMouseButtons[button];
    }

    inline void getCursorPos(double& x, double& y) const
    {
        glfwGetCursorPos(_window, &x, &y);
    }

    void setCursorMode(CursorMode mode);
    inline CursorMode getCursorMode() const
    {
        return _cursorMode;
    }

    static Input* getInstance() { return _instance; }

private:
    GLFWwindow* _window;

    bool _currentKeys[GLFW_KEY_LAST];
    bool _previousKeys[GLFW_KEY_LAST];
    bool _currentMouseButtons[GLFW_MOUSE_BUTTON_LAST];
    bool _previousMouseButtons[GLFW_MOUSE_BUTTON_LAST];

    CursorMode _cursorMode;

    static Input* _instance;
};