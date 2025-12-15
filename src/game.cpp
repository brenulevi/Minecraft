#include "game.h"

#include <iostream>

Game* Game::_instance = nullptr;

Game::Game()
{
    if(_instance)
        throw std::runtime_error("Game instance already exists!");

    _instance = this;

    _window = new Window(800, 600, "My Game");
    _window->setCloseCallback(std::bind(&Game::stop, this));
    _window->setResizeCallback(std::bind(&Game::onResize, this, std::placeholders::_1, std::placeholders::_2));
    _window->setVSync(false);

    _renderer = new Renderer(800, 600);
    _input = new Input(_window->getGLFWwindow());

    _uiManager = new UIManager();
    _uiManager->crosshair = new Crosshair(new Texture("assets/textures/crosshair.png"), 32.0f, 0.0f);

    _player = new Player(5.0f, 0.1f);
    _world = new World(12345, _player->getTransform().position);

    _isRunning = true;
}

Game::~Game()
{
    delete _player;
    delete _world;
    delete _uiManager;

    delete _input;
    delete _renderer;
    delete _window;
}

void Game::run()
{
    float totalTime = 0.0f;
    float lastFrameTime = static_cast<float>(glfwGetTime());
    while(_isRunning)
    {
        float currentFrameTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;
        totalTime += deltaTime;

        _window->pollEvents();
        _input->update();

        _player->update(deltaTime);
        _world->update();

        _renderer->clear();
        _renderer->setCamera(_player->getTransform(), _player->getCamera());
        _renderer->renderWorld(*_world);
        _renderer->renderOutline(_player->getLastRaycastHit(), glm::vec3(0.15f));
        _renderer->renderUI(*_uiManager);
        _window->swapBuffers();
    }
}

void Game::onResize(int width, int height)
{
    _renderer->onResize(width, height);
    _player->onResize(width, height);
    _world->onResize(width, height);
}
