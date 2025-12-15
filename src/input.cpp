#include "input.h"

Input* Input::_instance = nullptr;

Input::Input(GLFWwindow* window)
    : _window(window)
{
    if(_instance)
        throw std::runtime_error("Input instance already exists!");

    _instance = this;

    for(int i = 0; i < GLFW_KEY_LAST; ++i)
    {
        _currentKeys[i] = false;
        _previousKeys[i] = false;
    }
}

Input::~Input()
{
}

void Input::update()
{
    for(int i = 0; i < GLFW_KEY_LAST; ++i)
    {
        _previousKeys[i] = _currentKeys[i];
        _currentKeys[i] = (glfwGetKey(_window, i) == GLFW_PRESS);
    }

    for(int i = 0; i < GLFW_MOUSE_BUTTON_LAST; ++i)
    {
        _previousMouseButtons[i] = _currentMouseButtons[i];
        _currentMouseButtons[i] = (glfwGetMouseButton(_window, i) == GLFW_PRESS);
    }
}

void Input::setCursorMode(CursorMode mode)
{
    glfwSetInputMode(_window, GLFW_CURSOR, mode);
    _cursorMode = mode;
}
