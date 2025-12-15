#pragma once

#include "window.h"
#include "renderer.h"
#include "input.h"

#include "world.h"
#include "player.h"
#include "uimanager.h"

class Game
{
public:
    Game();
    ~Game();

    void run();

    inline void stop() { _isRunning = false; }
    void onResize(int width, int height);

    inline Window& getWindow() { return *_window; }
    inline Renderer& getRenderer() { return *_renderer; }
    inline Input& getInput() { return *_input; }

    inline World& getWorld() { return *_world; }
    inline Player& getPlayer() { return *_player; }

    inline static Game& getInstance() { return *_instance; }

private:
    Window *_window;
    Renderer *_renderer;
    Input *_input;
    UIManager *_uiManager;
    World *_world;
    Player *_player;

    bool _isRunning;

    static Game* _instance;
};