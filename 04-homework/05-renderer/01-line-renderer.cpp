#include "01-line-renderer.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>

line_render::line_render(canvas &buffer, size_t width, size_t height)
    : buffer(buffer), w(width), h(height) {}

void line_render::clear(color c) { std::fill(buffer.begin(), buffer.end(), c); }

void line_render::set_pixel(position p, color c) {
  buffer.set_pixel(p.x, p.y, c);
}

pixels line_render::pixels_positions(position start, position end) {
  using namespace std;
  pixels result;
  int x0 = start.x;
  int y0 = start.y;
  int x1 = end.x;
  int y1 = end.y;

  if (x1 <= x0) {
    std::swap(x0, x1);
    std::swap(y0, y1);
  }

  int dx = x1 - x0;
  int dy = y1 - y0;

  int error = 0;
  int derror = std::abs(dy) * 2;
  int y = y0;

  if (dx == 0) { // vertical line
    int ystep = (y1 > y0) ? 1 : -1;
    for (int y = y0; y != y1; y += ystep) {
      result.push_back(position{x0, y});
    }
    result.push_back(position{x0, y1});
  } else {
    for (int x = x0; x <= x1; ++x) {
      result.push_back(position{x, y});

      error += derror;
      if (error > dx) {
        y += (dy > 0) ? 1 : -1;
        error -= dx * 2;
      }
    }
  }
  return result;
}

void line_render::draw_line(position start, position end, color c) {
  pixels l = pixels_positions(start, end);
  std::for_each(begin(l), std::end(l), [&](auto &pos) { set_pixel(pos, c); });
}