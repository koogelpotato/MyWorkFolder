#include "keybind.hpp"
std::unordered_map<SDL_Keycode, keybinds> keymap = {
     {SDLK_UP, KEY_PRESS_UP},
     {SDLK_DOWN, KEY_PRESS_DOWN},
     {SDLK_LEFT, KEY_PRESS_LEFT},
     {SDLK_RIGHT, KEY_PRESS_RIGHT},
     {SDLK_q, KEY_PRESS_ACTION1},
     {SDLK_e, KEY_PRESS_ACTION2},
     {SDLK_ESCAPE, KEY_PRESS_QUIT},
     {SDLK_LCTRL, KEY_PRESS_REMAP},
     {SDLK_TAB, KEY_PRESS_PRINTALL}
};
