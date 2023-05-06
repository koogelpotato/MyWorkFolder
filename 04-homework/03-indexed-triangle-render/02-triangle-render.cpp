#include "02-triangle-render.hpp"
#include "00-blank-canvas.hpp"

triangle_render::triangle_render(canvas &buffer, size_t width, size_t height)
    : line_render(buffer, width, height) {}

pixels triangle_render::pixels_positions_triangle(position v0, position v1,
                                                  position v2) {
    pixels pixels_pos;

    for (auto [start, end] :
         {std::pair{v0, v1}, std::pair{v1, v2}, std::pair{v2, v0}}) {
        pixels line = line_render::pixels_positions(start, end);
        pixels_pos.insert(pixels_pos.end(), line.begin(), line.end());
    }

    return pixels_pos;
}

void triangle_render::draw_triangles(std::vector<position> &vertexes,
                                     size_t num_vertexes, color c) {
    pixels triangles_edge_pixels;

    for (size_t i = 0; i < num_vertexes; i += 3) {
        position v0 = vertexes.at(i);
        position v1 = vertexes.at(i + 1);
        position v2 = vertexes.at(i + 2);

        auto pixels_pos_triangle = pixels_positions_triangle(v0, v1, v2);
        triangles_edge_pixels.insert(triangles_edge_pixels.end(),
                                     pixels_pos_triangle.begin(),
                                     pixels_pos_triangle.end());
    }

    for (auto &pos : triangles_edge_pixels) {
        set_pixel(pos, c);
    }
}