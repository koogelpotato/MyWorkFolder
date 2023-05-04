#include <SDL3/SDL.h>
#include <SDL_events.h>
#include <SDL_timer.h>

#include <cstdlib>
#include <iostream>

#include "04-interpolated-triangle.hpp"

int main(int, char**) {
  using namespace std;

  if (0 != SDL_Init(SDL_INIT_EVERYTHING)) {
    cerr << SDL_GetError() << endl;
    return EXIT_FAILURE;
  }

  const size_t width = 1000;
  const size_t height = 1000;

  SDL_Window* window =
      SDL_CreateWindow("runtime soft render", width, height, SDL_WINDOW_OPENGL);
  if (window == nullptr) {
    cerr << SDL_GetError() << endl;
    return EXIT_FAILURE;
  }

  SDL_Renderer* renderer =
      SDL_CreateRenderer(window, nullptr, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr) {
    cerr << SDL_GetError() << endl;
    return EXIT_FAILURE;
  }

  const color indigo = {75, 0, 130};
  const color void_color = {0, 0, 0};

  

  canvas image(width, height);

  triangle_interpolated interpolated_render(image, width, height);

  struct program : graphics_program {
    double mouse_x{};
    double mouse_y{};
    double radius{};
    

    void set_uniforms(const uniforms& a_uniforms) override {
      mouse_x = a_uniforms.f0;
      mouse_y = a_uniforms.f1;
      radius = a_uniforms.f2;
    }
    vertex vertex_shader(const vertex& v_in) override {
      vertex out = v_in;
    float pivot_x = 0.5f;
    float pivot_y = 0.5f;
    float angle = 3.14159f / 4.0f; // rotate 45 degrees

    
    out.x -= pivot_x;
    out.y -= pivot_y;

    
    float s = sin(angle);
    float c = cos(angle);
    float new_x = out.x * c - out.y * s;
    float new_y = out.x * s + out.y * c;
    out.x = new_x;
    out.y = new_y;

    
    out.x += pivot_x;
    out.y += pivot_y;

    return out;
    }
    color fragment_shader(const vertex& v_in) override {
    color out;
    out.r = static_cast<uint8_t>(v_in.f3 * 255);
    out.g = static_cast<uint8_t>(v_in.f4 * 255);
    out.b = static_cast<uint8_t>(v_in.f5 * 255);
    float noise = abs(sin(v_in.x * 10.0 + v_in.y * 10.0 + v_in.z * 10.0));
    out.r = v_in.f3 * noise;
    out.g = v_in.f4 * noise;
    out.b = v_in.f5 * noise;

    return out;
    }
  } program01;

  void* pixels = image.get_pixels().data();
  const int depth = sizeof(color) * 8;
  const int pitch = width * sizeof(color);
  const int rmask = 0x000000ff;
  const int gmask = 0x0000ff00;
  const int bmask = 0x00ff0000;
  const int amask = 0;

  interpolated_render.set_graphics_program(program01);
  double mouse_x{};
  double mouse_y{};
  double radius{20.0};

  bool continue_loop = true;

  while (continue_loop) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_EVENT_QUIT) {
        continue_loop = false;
        break;
      } else if (e.type == SDL_EVENT_MOUSE_MOTION) {
        mouse_x = e.motion.x;
        mouse_y = e.motion.y;
      } else if (e.type == SDL_EVENT_MOUSE_WHEEL) {
        radius += e.wheel.y;
      }
    }
    constexpr size_t max_x  = 15;
    constexpr size_t max_y  = 15;
    constexpr double step_x = (width - 1.0) / static_cast<double>(max_x - 1);
    constexpr double step_y = (height - 1.0) / static_cast<double>(max_y - 1);
    vector<vertex> vertices;
    for (size_t j = 0; j < max_y; ++j)
    {
        for (size_t i = 0; i < max_x; ++i)
        {
            double x = i * step_x;
            double y = j * step_y;
            vertices.push_back({ x, y });
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
            

            indeces.insert(end(indeces), { v0, v1, v2 });
            indeces.insert(end(indeces), { v2, v0, v1 });
            indeces.insert(end(indeces), { v1, v2, v0 });
            
        }
    }

    program01.set_uniforms(uniforms{mouse_x, mouse_y, radius});
    interpolated_render.clear(indigo);
    interpolated_render.draw_triangles(vertices, indeces);

    SDL_Surface* bitmapSurface = SDL_CreateSurfaceFrom(
        pixels, width, height, pitch, SDL_PIXELFORMAT_RGB24);
    if (bitmapSurface == nullptr) {
      cerr << SDL_GetError() << endl;
      return EXIT_FAILURE;
    }
    SDL_Texture* bitmapTex =
        SDL_CreateTextureFromSurface(renderer, bitmapSurface);
    if (bitmapTex == nullptr) {
      cerr << SDL_GetError() << endl;
      return EXIT_FAILURE;
    }
    SDL_DestroySurface(bitmapSurface);

    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, bitmapTex, nullptr, nullptr);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(bitmapTex);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return EXIT_SUCCESS;
}