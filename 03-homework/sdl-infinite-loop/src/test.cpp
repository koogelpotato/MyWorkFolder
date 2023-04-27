#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char *argv[]) {
  SDL_Window *window;
  SDL_Rect display_bounds;
  SDL_GetDisplayBounds(0, &display_bounds);

  const int max_width = display_bounds.w;
  const int max_height = display_bounds.h;

  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow("An SDL2 window", 640, 480,
                            640, // Use half the screen width
                            480, // Use half the screen height
                            SDL_WINDOW_OPENGL);

  if (window == NULL) {
    printf("Could not create window: %s\n", SDL_GetError());
    return 1;
  }

  bool is_running = true;
  SDL_Event event;
  while (is_running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        is_running = false;
      }
    }
    SDL_Delay(16);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}