#include "00-blank-canvas.hpp"
#include "01-line-renderer.hpp"

int main(int, char**)
{
    const color black = { 0, 0, 0 };
    const color white = { 255, 255, 255 };
    const color green = { 0, 255, 0 };

    int32_t width  = 320;
    int32_t height = 240;

    canvas image(width, height);

    line_render render(image, width, height);

    render.clear(black);
    std::vector<line> lines(100);
    for (size_t i = 0; i < lines.size(); ++i)
    {
        lines[i].start      = position::generate_random(width, height);
        lines[i].end        = position::generate_random(width, height);
        lines[i].line_color = { static_cast<uint8_t>(rand() % 256),
                                static_cast<uint8_t>(rand() % 256),
                                static_cast<uint8_t>(rand() % 256) };
        render.draw_line(lines[i].start, lines[i].end, lines[i].line_color);
    }

    image.save_image("01_lines.ppm");

    canvas      inverted_image(width, height);
    line_render inverted_render(inverted_image, width, height);
    inverted_render.clear(black);
    for (const auto& line : lines)
    {
        inverted_render.draw_line(
            line.start, line.end, line.line_color.invert_color());
    }
    inverted_image.save_image("01_inverted_lines.ppm");
    return 0;
}