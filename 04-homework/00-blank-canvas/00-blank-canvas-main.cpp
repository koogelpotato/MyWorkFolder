#include "00-blank-canvas.hpp"

#include <iostream>

int main(int, char **) {
    const color green = {0, 255, 0};

    size_t width = 320;
    size_t height = 240;

    canvas image(width, height);

    std::fill(image.begin(), image.end(), green);

    const char *file_name = "00_green_image.ppm";

    image.save_image(file_name);

    canvas image_loaded(0, 0);
    image_loaded.load_image(file_name);

    if (image != image_loaded) {
        std::cerr << "image != image_loaded\n";
        return 1;
    } else {
        std::cout << "image == image_loaded\n";
    }
    color new_color =
        green.invert_color(); // invert the green color which is purple/magenta

    canvas inverted_image(
        width,
        height); // TODO ask about code refactoring, problem: code doubling

    std::fill(inverted_image.begin(), inverted_image.end(),
              new_color); // fill the canvas with the new color

    const char *inverted_file_name = "00_inverted_image.ppm";

    inverted_image.save_image(inverted_file_name);

    canvas inverted_image_loaded(0, 0);
    inverted_image_loaded.load_image(inverted_file_name);

    if (inverted_image != inverted_image_loaded) {
        std::cerr << "inverted_image != inverted_image_loaded\n";
        return 1;
    } else {
        std::cout << "inverted_image == inverted_image_loaded\n";
    }

    return 0;
}