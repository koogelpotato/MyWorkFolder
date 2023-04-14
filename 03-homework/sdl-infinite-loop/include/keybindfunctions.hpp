#include <stdio.h>
#include <unordered_map>
#include <string>

#include <SDL3/SDL.h>
#include "keybind.hpp"

#ifndef KEYBINDFUNCTIONS_H
#define KEYBINDFUNCTIONS_H

bool init();
bool process_key(SDL_Event& event, std::unordered_map<SDL_Keycode, keybinds>& keymap);
void close();
void remap_key(SDL_Keycode old_key, SDL_Keycode new_key, std::unordered_map<SDL_Keycode, keybinds>& keymap);
void print_all_keys(std::unordered_map<SDL_Keycode, keybinds>& keymap);

#endif