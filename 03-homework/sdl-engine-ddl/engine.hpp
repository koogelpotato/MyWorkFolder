#ifndef ENGINE_HPP
#define ENGINE_HPP
#include "keybind.hpp"
#include <SDL3/SDL.h>
#include <iosfwd>
#include <string>
#include <string_view>
class engine;

engine* create_engine();
void    destroy_engine(engine* e);

class engine
{
public:
    virtual ~engine();
    virtual std::string initialize(std::string_view config) = 0;
    virtual bool        read_input(
               SDL_Event&                                 event,
               std::unordered_map<SDL_Keycode, keybinds>& keymap) = 0;
    virtual void uninitialize()                                   = 0;
    virtual void print_all_keys(
        std::unordered_map<SDL_Keycode, keybinds>& keymap) = 0;
    virtual void remap_key(
        SDL_Keycode                                old_key,
        SDL_Keycode                                new_key,
        std::unordered_map<SDL_Keycode, keybinds>& keymap) = 0;
};
#endif
