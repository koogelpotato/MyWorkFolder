#pragma once
#include "02-triangle-render.hpp"

struct triangle_indexed_render : triangle_render
{
    triangle_indexed_render(canvas& buffer, size_t width, size_t height);

    void draw_triangles(std::vector<position>& vertexes,
                        std::vector<uint8_t>&  indexes,
                        color                  col);
};