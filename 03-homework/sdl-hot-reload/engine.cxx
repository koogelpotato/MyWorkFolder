#include "engine.hxx"
#include <SDL3/SDL.h>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <memory>
#include <thread>

class engine_impl final : public engine
{
public:
    std::string init(std::string_view) final
    {
        using namespace std;
        const int init_sdl = SDL_Init(SDL_INIT_VIDEO);
        if (init_sdl != 0)
        {
            cerr << "error with SDL init" << SDL_GetError() << endl;
            return "";
        }
        SDL_Window* const window =
            SDL_CreateWindow("window", 640, 480, SDL_WINDOW_OPENGL);
        if (window == nullptr)
        {
            cerr << "error create window" << SDL_GetError() << endl;
            return "";
        }
        return "";
    }
    bool input() final
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                std::cout << "key pressed: "
                          << SDL_GetKeyName(event.key.keysym.sym) << std::endl;
            }
            else if (event.type == SDL_EVENT_KEY_UP)
            {
                std::cout << "key released: "
                          << SDL_GetKeyName(event.key.keysym.sym) << std::endl;
            }
            else if (event.type == SDL_EVENT_QUIT)
            {
                return false;
            }
        }
        return true;
    }

    void uninitialize() final{};
};
static bool engine_exist = false;
engine*     create_engine()
{
    if (engine_exist)
    {
        throw std::runtime_error("engine exist");
    }
    engine_exist = true;
    return new engine_impl();
}
void destroy_engine(engine* e)
{
    if (!engine_exist)
    {
        throw std::runtime_error("engine not created");
    }
    if (nullptr == e)
    {
        throw std::runtime_error("e is nullptr");
    }
    delete e;
}
engine::~engine() = default;

game* reload_game(game*   old,
                       const char*  library_name,
                       const char*  temp_lib,
                       engine& engine,
                       void*&       old_handle);

int main(int /*argc*/, char* /*argv*/[])
{
    using namespace std;

    unique_ptr<engine, void (*)(engine*)> engine(create_engine(),
                                                           destroy_engine);

    string init_err = engine->init("");

    if (!init_err.empty())
    {
        cerr << init_err << endl;
        return EXIT_FAILURE;
    }

    const char* library_name = "./libgame.so";
    const char* temp_lib     = "./tmp.so";
    void*       game_library_handle{};
    game*  game = reload_game(
        nullptr, library_name, temp_lib, *engine, game_library_handle);

    auto time_during_loading = filesystem::last_write_time(library_name);

    game->init();

    bool running = true;
    while (running)
    {
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
            game = reload_game(
                game, library_name, temp_lib, *engine, game_library_handle);

            if (game == nullptr)
            {
                std::cerr << "next attempt to reload game..." << std::endl;
                continue;
            }

            time_during_loading = next_write_time;
        }

        while (!engine->input())
        {
            game->on_event();
        }

        game->update();
        game->render();
    }

    engine->uninitialize();
    return EXIT_SUCCESS;
}

game* reload_game(game*   old,
                       const char*  library_name,
                       const char*  temp_lib,
                       engine& engine,
                       void*&       old_handle)
{

    if (old)
    {
        SDL_UnloadObject(old_handle);
        old_handle = nullptr;
    }

    try
    {
        remove(temp_lib);     
    }
    catch (const std::exception& ex)
    {
        std::cerr << "error: can't remove [" << temp_lib << "]: " << ex.what()
                  << std::endl;
        return nullptr;
    }

    try
    {
        std::filesystem::copy(library_name, temp_lib); 
    }
    catch (const std::exception& ex)
    {
        std::cerr << "error: can't copy [" << library_name << "]to[" << temp_lib
                  << "] : " << ex.what() << std::endl;
        return nullptr;
    }
    void* game_library_handle = SDL_LoadObject(temp_lib);
    if (game_library_handle == nullptr)
    {
        std::cerr << "error: can't load library [" << temp_lib
                  << "]: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    auto create_game = reinterpret_cast<game* (*)()>(SDL_LoadFunction(
        game_library_handle, "create_game")); 
    if (create_game == nullptr)
    {
        std::cerr << "error: can't find symbol 'create_game' in library ["
                  << temp_lib << "]: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    auto game = create_game();
    if (game == nullptr)
    {
        std::cerr << "error: can't create game object from library ["
                  << temp_lib << "]" << std::endl;
        return nullptr;
    }

    if (old)
    {
        SDL_UnloadObject(old_handle);
        old_handle = nullptr;
    }

    old_handle = game_library_handle;

    return game;
}
