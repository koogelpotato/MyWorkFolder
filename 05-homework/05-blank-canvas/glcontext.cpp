#include "glad/glad.h"
#include <SDL3/SDL.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cmath>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <vector>

int main(int argc, char** argv)
{
    using namespace std;
    const int width       = 512;
    const int height      = 512;
    const int init_result = SDL_Init(SDL_INIT_VIDEO);
    if (init_result != 0)
    {
        const char* err_message = SDL_GetError();
        cerr << "error: failed call SDL_Init: " << err_message << endl;
    }
    SDL_Window* window =
        SDL_CreateWindow("blank canvas", width, height, SDL_WINDOW_OPENGL);
    if (window == nullptr)
    {
        const char* err_message = SDL_GetError();
        cerr << "error: window uninitialized: " << err_message << endl;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    int              gl_major_ver       = 3;
    int              gl_minor_ver       = 2;
    int              gl_context_profile = SDL_GL_CONTEXT_PROFILE_ES;
    const char*      platform_from_sdl  = SDL_GetPlatform();
    std::string_view platform{ platform_from_sdl };
    using namespace std::string_view_literals;
    using namespace std;
    auto list = { "Windows"sv, "Mac OS X"sv };
    auto it   = find(begin(list), end(list), platform);
    if (it != end(list))
    {
        gl_major_ver       = 4;
        gl_minor_ver       = (platform == "Mac OS X") ? 1 : 3;
        gl_context_profile = SDL_GL_CONTEXT_PROFILE_CORE;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major_ver);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor_ver);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, gl_context_profile);

    SDL_GLContext mainContext = SDL_GL_CreateContext(window);
    int gl_funcitons = gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress);
    if (gl_funcitons != 0)
    {
        cerr << "error: failed to initialize glad" << endl;
    }

    SDL_Event event;
    int       running = 1;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = 0;
            }
        }
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window);
    }
    printf("%s\n", glGetString(GL_VERSION));

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
