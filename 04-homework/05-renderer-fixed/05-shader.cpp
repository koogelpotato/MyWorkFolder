#include "04-interpolated-triangle-render.hpp"

#include <SDL3/SDL.h>

#include <iostream>
#include <random>

int main(int, char**)
{
    using namespace std;

    constexpr size_t width  = 1000;
    constexpr size_t height = 1000;
    const color black = {0, 0, 0};

    struct program : gfx_program
    {
        double mouse_x{};
        double mouse_y{};
        double radius{};
        double game_time{};
        

        vertex vertex_shader(const vertex& v_in) override
        {
            vertex out = v_in;
            float speed = 0.1f;
            out.x += game_time * speed * game_time;    
            return out;
        }

        color fragment_shader(
            const vertex& v_in) override 
        {
            color out;
            color start_color = { 0, 0, 0 }; 
            color end_color   = {100, 100, 0}; 
            double gradient_position = (v_in.x - width / 2) / width + 0.5;
            out.r =
                static_cast<uint8_t>(start_color.r * (1 - gradient_position) +
                                     end_color.r * gradient_position);
            out.g =
                static_cast<uint8_t>(start_color.g * (1 - gradient_position) +
                                     end_color.g * gradient_position);
            out.b =
                static_cast<uint8_t>(start_color.b * (1 - gradient_position) +
                                     end_color.b * gradient_position);
            return out;
        }

        void set_uniforms(const uniforms& u) override
        {
            mouse_x = u.f0;
            mouse_y = u.f1;
            radius  = u.f2;
            game_time = u.f3;
        }
    } shader_program;

    const int init = SDL_Init(SDL_INIT_VIDEO);
    if (init != 0)
    {
        cerr << "error SDL_Init:" << SDL_GetError() << endl;
        return EXIT_FAILURE;
    }

    SDL_Window* const window =
        SDL_CreateWindow("program", width, height, SDL_WINDOW_OPENGL);
    if (window == nullptr)
    {
        cerr << "error SDL_CreateWindow:" << SDL_GetError() << endl;
        return EXIT_FAILURE;
    }

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, nullptr, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        cerr << "error SDL_CreateRenderer:" << SDL_GetError() << endl;
        return EXIT_FAILURE;
    }

    canvas image(width, height);

    triangle_interpolated shaders_render(image, width, height);

constexpr size_t max_x  = 10;
constexpr size_t max_y  = 10;
constexpr double step_x = (width - 1.0) / static_cast<double>(max_x - 1);
constexpr double step_y = (height - 1.0) / static_cast<double>(max_y - 1);

vector<vertex> triangle_vertices;

for (size_t j = 0; j < max_y; ++j)
{
    for (size_t i = 0; i < max_x; ++i)
        {
            double x = i * step_x;
            double y = j * step_y;
            triangle_vertices.push_back({ x, y });
        }
}

vector<uint16_t> indeces;

for (size_t j = 0; j < max_y - 1; ++j)
{
    for (size_t i = 0; i < max_x - 1; ++i)
    {
        uint16_t v0 = j * max_x + i;
        uint16_t v1 = v0 + 1;
        uint16_t v2 = v0 + max_x;
        uint16_t v3 = v2 + 1;

        if ((i + j) % 2 == 0) {
            indeces.insert(end(indeces), { v0, v1, v2 });
            indeces.insert(end(indeces), { v2, v1, v0 });
        } else {
            indeces.insert(end(indeces), { v1, v2, v3 });
            indeces.insert(end(indeces), { v3, v2, v1 });
        }
    }
}

    const int pitch  = width * sizeof(color);
    void*     pixels = image.get_pixels().data();

    shaders_render.set_gfx_program(shader_program);

    double game_time{0};
    double mouse_x{ width / 2 };    
    double mouse_y{ height / 2 };
    double radius{ 10 };
    
    bool running = true;

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
                break;
            }
            if (event.type == SDL_EVENT_MOUSE_MOTION)
            {
                
                mouse_x = event.motion.x;
                mouse_y = event.motion.y;
            }
            if(event.type == SDL_EVENT_MOUSE_WHEEL)
            {
                radius += event.wheel.y * 5;
            }
        }

        shaders_render.clear(black);
        game_time = SDL_GetTicks() / 500.0;
        shader_program.set_uniforms(uniforms{ mouse_x, mouse_y, radius,game_time});

        shaders_render.draw_triangles(triangle_vertices, indeces);

        SDL_Surface* surface = SDL_CreateSurfaceFrom(
            pixels, width, height, pitch, SDL_PIXELFORMAT_RGB24);
        if (surface == nullptr)
        {
            cerr << "SDL_Surface Error: " << SDL_GetError() << endl;
            return EXIT_FAILURE;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == nullptr)
        {
            cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError()
                 << endl;
            return EXIT_FAILURE;
        }
        SDL_DestroySurface(surface);

        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);

        SDL_DestroyTexture(texture);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return EXIT_SUCCESS;
}