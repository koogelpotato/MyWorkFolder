#include "keybind.hpp"
#include <iosfwd>
#include <string>
#include <string_view>
#include <SDL3/SDL.h>
class engine;

engine* create_engine();
void destroy_engine(engine* e);

class engine
{
    public:
        virtual ~engine();
        virtual bool initialize(std::string_view config) = 0;
        virtual bool read_input(SDL_Event& event,std::unordered_map<SDL_Keycode, keybinds>& keymap) = 0;
        virtual void uninitialize()       = 0;
};