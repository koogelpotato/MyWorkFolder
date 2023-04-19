#include "engine.hpp"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string_view>
#include <chrono>
#include <filesystem>
#include <thread>

int main(int argc, char *argv[])
{
    using namespace std;

    std::unique_ptr<engine, void (*)(engine*)> engine(
        create_engine(), destroy_engine);
    std::string err = engine->initialize("");
    if (err.empty())
    {
        std::cerr << err << std::endl;
        return EXIT_FAILURE;
    }
    const char* library_name = "./libmygame.so";
    const char* temp_lib     = "./temp.so";
    void*       game_library_handle{};
    engine -> hot_reload(nullptr, library_name, temp_lib, *engine, game_library_handle);
    game *game;
    game ->create_game();
    bool continue_loop = true;
    while (continue_loop)
    {
        SDL_Event event;
        auto lib_current_write_time = filesystem::last_write_time(library_name);

        if (lib_current_write_time != time_during_loading)
        {
            filesystem::file_time_type next_write_time;
            for (;;)
            {
                using namespace std::chrono;
                std::this_thread::sleep_for(milliseconds(1000));
                next_write_time = filesystem::last_write_time(library_name);
                if (next_write_time != lib_current_write_time)
                {
                    lib_current_write_time = next_write_time;
                }
                else
                {
                    break;
                }
            }

            std::cout << "reloading game" << std::endl;
            engine ->hot_reload(game, library_name, temp_lib, *engine, game_library_handle);
            if (game == nullptr)
            {
                std::cerr << "next attempt to reload game..." << std::endl;
                continue;
            }

            time_during_loading = next_write_time;
        }
        while (engine->read_input(event,keymap))
        {
            if (game == nullptr)
            {
                std::cerr << "Failed to load or create game instance" << std::endl;
            }

            if (!engine->read_input(event,keymap))
            {
                continue_loop = false;
                break;
            }
        }
        game->update();
        game->render();
    }
    
    engine->uninitialize();

    return EXIT_SUCCESS;
}

