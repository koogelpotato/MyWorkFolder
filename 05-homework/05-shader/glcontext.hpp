#include "glad/glad.h"
#include <SDL3/SDL.h>
#include <iosfwd>
#include <string>
#include <string_view>

namespace kgl_engine
{
struct vertex
{
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
};
struct triangle
{
    triangle()
    {
        vert[0] = vertex();
        vert[1] = vertex();
        vert[2] = vertex();
    }
    vertex vert[3];
};
std::istream& operator>>(std::istream& is, vertex&);
std::istream& operator>>(std::istream& is, triangle&);

class engine;

engine* create_engine();
void    destroy_engine(engine* e);

class engine
{

public:
    virtual ~engine();
    virtual std::string init(std::string_view config) = 0;
    virtual GLuint      create_shader_from_string(
             const char* vertex_shader_str, const char* fragment_shader_str) = 0;
    virtual GLuint create_shader_from_file(
        const char* vertex_shader_file, const char* fragment_shader_file) = 0;
    virtual void hot_reload_shader_file(GLuint*     shader,
                                        const char* vertex_shader_file,
                                        const char* fragment_shader_file) = 0;
    virtual bool input(bool& running)                                     = 0;
    virtual void triangle_render(const triangle&)                         = 0;
    virtual void uninitialize()                                           = 0;
    virtual void swap_buffer()                                            = 0;

private:
    SDL_Window*   window     = nullptr;
    SDL_GLContext gl_context = nullptr;
    GLuint        program    = 0;
};
} // namespace kgl_engine