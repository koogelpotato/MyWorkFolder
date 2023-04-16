#include <engine.hpp>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string_view>

int main(int argc, char *argv[])
{
    using namespace std;
    std::unique_ptr<engine, void (*)(engine*)> engine(
        create_engine(), destroy_engine);

    bool continue_loop = true;
    while (continue_loop)
    {
        SDL_Event event;

        while (engine->read_input(event, keymap))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                continue_loop = false;
                break;
            }
        }
    }

    engine->uninitialize();

    return EXIT_SUCCESS;
}

