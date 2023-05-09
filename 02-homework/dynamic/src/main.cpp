#include <SDL3/SDL_version.h>
#include <cstdlib>
#include <iostream>

int main(int argc, char *argv[]) {
    SDL_version compiled;
    SDL_version linked;

    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);

    std::cout << "Compiled against SDL version " << (int)compiled.major << "."
              << (int)compiled.minor << "." << (int)compiled.patch << std::endl;
    std::cout << "Linked against SDL version " << (int)linked.major << "."
              << (int)linked.minor << "." << (int)linked.patch << std::endl;

    bool is_good = std::cout.good();
    int result = is_good ? EXIT_SUCCESS : EXIT_FAILURE;
    return result;
}