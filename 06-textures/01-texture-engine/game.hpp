#pragma once

#include "glad/glad.h"
#include <SDL3/SDL.h>

enum GameState{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game{
    public:
    GameState State;
    bool keys[1024];
    unsigned int width, height;
    Game(unsigned int width, unsigned int height);
    ~Game();
    void init_game();
    void proccess_input(float dt);
    void update(float dt);
    void render();
};