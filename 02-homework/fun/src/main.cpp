#include <SDL3/SDL.h>

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
    return 1;
  }

  SDL_Window *window =
      SDL_CreateWindow("Hello, SDL3!", 640, 480, SDL_WINDOW_FULLSCREEN);
  if (!window) {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return 1;
  }
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}