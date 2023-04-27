#include "03-indexed-triangle-render.hpp"
#include "00-blank-canvas.hpp"

triangle_indexed_render::triangle_indexed_render(canvas& buffer, size_t width, size_t height)
    : triangle_render(buffer, width, height)
{
}

void triangle_indexed_render::draw_triangles(std::vector<position>& vertexes, std::vector<uint8_t>& indices, color c)
{
    pixels triangles_edge_pixels;

    for (size_t i = 0; i < indices.size(); i += 3)
    {
        uint8_t index0 = indices[i];
        uint8_t index1 = indices[i + 1];
        uint8_t index2 = indices[i + 2];

        position v0 = vertexes[index0];
        position v1 = vertexes[index1];
        position v2 = vertexes[index2];

        pixels lines = pixels_positions_triangle(v0, v1, v2);

        triangles_edge_pixels.insert(triangles_edge_pixels.end(), lines.begin(), lines.end());
    }

    for (auto pos : triangles_edge_pixels)
    {
        set_pixel(pos, c);
    }
}