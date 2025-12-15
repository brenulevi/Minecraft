#include "window.h"

Window::Window(int width, int height, const char *title)
    : _width(width), _height(height), _title(title), _isFullscreen(false), _vsyncEnabled(true)
{
    if(!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _id = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if(!_id)
        throw std::runtime_error("Failed to create GLFW window");

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(_id, (mode->width - width) / 2, (mode->height - height) / 2);

    glfwMakeContextCurrent(_id);
    glfwSetWindowUserPointer(_id, this);
}

Window::~Window()
{
    glfwDestroyWindow(_id);
    glfwTerminate();
}

void Window::pollEvents()
{
    glfwPollEvents();
}

void Window::swapBuffers()
{
    glfwSwapBuffers(_id);
}

void Window::setVSync(bool enabled)
{
    _vsyncEnabled = enabled;
    glfwSwapInterval(enabled ? 1 : 0);
}

void Window::toggleFullscreen()
{
    _isFullscreen = !_isFullscreen;

    GLFWmonitor* monitor = _isFullscreen ? glfwGetPrimaryMonitor() : nullptr;
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    if(_isFullscreen)
    {
        glfwSetWindowMonitor(_id, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    else
    {
        int windowedWidth = 800;
        int windowedHeight = 600;
        glfwSetWindowMonitor(_id, nullptr, (mode->width - windowedWidth) / 2, (mode->height - windowedHeight) / 2, windowedWidth, windowedHeight, 0);
    }
}

void Window::setCloseCallback(const std::function<void()> &callback)
{
    _closeCallback = callback;

    glfwSetWindowCloseCallback(_id, [](GLFWwindow* window)
    {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if(win && win->_closeCallback)
            win->_closeCallback();
    });
}

void Window::setResizeCallback(const std::function<void(int, int)> &callback)
{
    _resizeCallback = callback;

    glfwSetWindowSizeCallback(_id, [](GLFWwindow* window, int width, int height)
    {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

        win->_width = width;
        win->_height = height;

        if(win && win->_resizeCallback)
            win->_resizeCallback(width, height);
    });
}
