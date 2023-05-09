#ifndef KEYBIND_H
#define KEYBIND_H

#include <SDL3/SDL.h>
#include <stdio.h>
#include <string>
#include <unordered_map>

enum keybinds {
    KEY_PRESS_UP,
    KEY_PRESS_DOWN,
    KEY_PRESS_LEFT,
    KEY_PRESS_RIGHT,
    KEY_PRESS_ACTION1,
    KEY_PRESS_ACTION2,
    KEY_PRESS_REMAP,
    KEY_PRESS_PRINTALL,
    KEY_PRESS_QUIT
};

extern std::unordered_map<SDL_Keycode, keybinds> keymap;
#endif
