#include "04-interpolated-triangle.hpp"

#include <algorithm>
#include <iostream>

double interpolate(const double f0, const double f1, const double t) {
  assert(t >= 0.0);
  assert(t <= 1.0);
  return f0 + (f1 - f0) * t;
}

vertex interpolate(const vertex &v0, const vertex &v1, const double t) {
  return {interpolate(v0.x, v1.x, t),   interpolate(v0.y, v1.y, t),
          interpolate(v0.z, v1.z, t),   interpolate(v0.f3, v1.f3, t),
          interpolate(v0.f4, v1.f4, t), interpolate(v0.f5, v1.f5, t),
          interpolate(v0.f6, v1.f6, t), interpolate(v0.f7, v1.f7, t)};
}

position interpolate(const position &p0, const position &p1, const double t) {
  return {static_cast<int>(interpolate(p0.x, p1.x, t)), static_cast<int>(interpolate(p0.y, p1.y, t))};
};

triangle_interpolated::triangle_interpolated(canvas &buffer, size_t width,
                                             size_t height)
    : triangle_indexed_render(buffer, width, height) {}

void triangle_interpolated::raster_one_horizontal_line(
    const vertex &left_vertex, const vertex &right_vertex,
    std::vector<vertex> &out) {
  size_t num_of_pixels_in_line =
      static_cast<size_t>(std::round(std::abs(left_vertex.x - right_vertex.x)));
  if (num_of_pixels_in_line > 0) {
    for (size_t p = 0; p <= num_of_pixels_in_line + 1; ++p) {
      double t_pixel = static_cast<double>(p) / (num_of_pixels_in_line + 1);
      vertex pixel = interpolate(left_vertex, right_vertex, t_pixel);

      out.push_back(pixel);
    }
  } else {
    out.push_back(left_vertex);
  }
}

std::vector<vertex> triangle_interpolated::raster_horizontal_triangle(
    const vertex &single, const vertex &left, const vertex &right) {
  std::vector<vertex> out;

  size_t num_of_hlines =
      static_cast<size_t>(std::round(std::abs(single.y - left.y)));

  if (num_of_hlines > 0) {
    for (size_t i = 0; i <= num_of_hlines; ++i) {
      double t_vertical = static_cast<double>(i) / num_of_hlines;
      vertex left_vertex = interpolate(left, single, t_vertical);
      vertex right_vertex = interpolate(right, single, t_vertical);

      raster_one_horizontal_line(left_vertex, right_vertex, out);
    }
  } else {
    raster_one_horizontal_line(left, right, out);
  }

  return out;
}

std::vector<vertex>
triangle_interpolated::rasterize_triangle(const vertex &v0, const vertex &v1,
                                          const vertex &v2) {
  std::vector<vertex> out;

    
    std::array<const vertex*, 3> in_vertexes{ &v0, &v1, &v2 };
    std::sort(begin(in_vertexes),
              end(in_vertexes),
              [](const vertex* left, const vertex* right)
              { return left->y < right->y; });

    const vertex& top    = *in_vertexes.at(0);
    const vertex& middle = *in_vertexes.at(1);
    const vertex& bottom = *in_vertexes.at(2);
    position start{ static_cast<int32_t>(std::round(top.x)),
                    static_cast<int32_t>(std::round(top.y)) };
    position end{ static_cast<int32_t>(std::round(bottom.x)),
                  static_cast<int32_t>(std::round(bottom.y)) };
    position middle_pos{ static_cast<int32_t>(std::round(middle.x)),
                         static_cast<int32_t>(std::round(middle.y)) };

    if (start == end)
    {

        position delta        = start - middle_pos;
        size_t   count_pixels = 4 * (std::abs(delta.x) + std::abs(delta.y) + 1);
        for (size_t i = 0; i < count_pixels; ++i)
        {
            double t      = static_cast<double>(i) / count_pixels;
            vertex vertex = interpolate(top, middle, t);
            out.push_back(vertex);
        }

        return out;
    }

    if (start == middle_pos)
    {

        position delta        = start - end;
        size_t   count_pixels = 4 * (std::abs(delta.x) + std::abs(delta.y) + 1);
        for (size_t i = 0; i < count_pixels; ++i)
        {
            double t      = static_cast<double>(i) / count_pixels;
            vertex vertex = interpolate(top, bottom, t);
            out.push_back(vertex);
        }

        return out;
    }

    if (end == middle_pos)
    {

        position delta        = start - middle_pos;
        size_t   count_pixels = 4 * (std::abs(delta.x) + std::abs(delta.y) + 1);
        for (size_t i = 0; i < count_pixels; ++i)
        {
            double t      = static_cast<double>(i) / count_pixels;
            vertex vertex = interpolate(top, middle, t);
            out.push_back(vertex);
        }

        return out;
    }
    return out;
}
void triangle_interpolated::draw_triangles(
    const std::vector<vertex> &vertexes, const std::vector<uint16_t> &indexes) {
  for (size_t index = 0; index < indexes.size(); index += 3) {
    const uint16_t index0 = indexes.at(index + 0);
    const uint16_t index1 = indexes.at(index + 1);
    const uint16_t index2 = indexes.at(index + 2);

    const vertex &v0 = vertexes.at(index0);
    const vertex &v1 = vertexes.at(index1);
    const vertex &v2 = vertexes.at(index2);

    const vertex v0_ = program_->vertex_shader(v0);
    const vertex v1_ = program_->vertex_shader(v1);
    const vertex v2_ = program_->vertex_shader(v2);

    const std::vector<vertex> interpolated = rasterize_triangle(v0_, v1_, v2_);
    for (const vertex &interpolated_vertex : interpolated) {
      const color c = program_->fragment_shader(interpolated_vertex);
      const position pos{
          static_cast<int32_t>(std::round(interpolated_vertex.x)),
          static_cast<int32_t>(std::round(interpolated_vertex.y))};
      set_pixel(pos, c);
    }
  }
}