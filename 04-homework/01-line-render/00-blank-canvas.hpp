#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <vector>

;

#pragma pack(push, 1)
struct color
{
    uint8_t     r = 0;
    uint8_t     g = 0;
    uint8_t     b = 0;
    friend bool operator==(const color& l, const color& r);
    color       invert_color() const // perform color inversion
    {
        color inv_color;
        inv_color.r = 255 - r;
        inv_color.g = 255 - g;
        inv_color.b = 255 - b;
        return inv_color;
    }
};
#pragma pack(pop)

constexpr color color_red{ 255, 0, 0 };
constexpr color color_green{ 0, 255, 0 };
constexpr color color_blue{ 0, 0, 255 };

class canvas
{
public:
    canvas(size_t w, size_t h)
        : width{ w }
        , height{ h }
    {
        pixels.resize(width * height);
    }
    void save_image(const std::string& file_name)
    {
        std::ofstream out_file;
        out_file.exceptions(std::ios_base::failbit);
        out_file.open(file_name, std::ios_base::binary);
        out_file << "P6\n" << width << ' ' << height << ' ' << 255 << '\n';
        std::streamsize buf_size =
            static_cast<std::streamsize>(sizeof(color) * pixels.size());
        out_file.write(reinterpret_cast<const char*>(pixels.data()), buf_size);
    }

    void load_image(const std::string& file_name)
    {
        std::ifstream in_file;
        in_file.exceptions(std::ios_base::failbit);
        in_file.open(file_name, std::ios_base::binary);
        std::string header;
        std::string color_format;
        char        last_next_line = 0;
        in_file >> header >> width >> height >> color_format >> std::noskipws >>
            last_next_line;

        if (!iswspace(last_next_line))
        {
            throw std::runtime_error("expected whitespace");
        }

        pixels.resize(width * height);

        if (pixels.size() != width * height)
        {
            throw std::runtime_error("image size not match");
        }
        std::streamsize buf_size =
            static_cast<std::streamsize>(sizeof(color) * pixels.size());
        in_file.read(reinterpret_cast<char*>(pixels.data()), buf_size);
    }

    void set_pixel(size_t x, size_t y, color col)
    {
        const size_t liner_index_in_buffer = width * y + x;
        color&       target_pixel          = pixels.at(liner_index_in_buffer);
        target_pixel                       = col;
    }

    color get_pixel(size_t x, size_t y) const
    {
        const size_t liner_index_in_buffer = width * y + x;
        return pixels.at(liner_index_in_buffer);
    }

    bool operator==(const canvas& other) const
    {
        return pixels == other.pixels;
    }

    bool operator!=(const canvas& other) const { return !(*this == other); }

    auto begin() { return pixels.begin(); }
    auto end() { return pixels.end(); }

    std::vector<color>& get_pixels() { return pixels; }

    size_t get_width() const { return width; }
    size_t get_height() const { return height; }

private:
    size_t             width  = 0;
    size_t             height = 0;
    std::vector<color> pixels;
};

struct position
{
    double          length() { return std::sqrt(x * x + y * y); }
    friend position operator-(const position& left, const position& right);
    friend bool     operator==(const position& left, const position& right);
    int32_t         x = 0;
    int32_t         y = 0;
    static position generate_random(int width, int height);
};

using pixels = std::vector<position>;

struct irender
{
    virtual void   clear(color)                                   = 0;
    virtual void   set_pixel(position, color)                     = 0;
    virtual pixels pixels_positions(position start, position end) = 0;

    virtual ~irender();
};
struct line
{
    position start;
    position end;
    color    line_color;
};