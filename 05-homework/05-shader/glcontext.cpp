#include "glcontext.hpp"
#include "glad/glad.h"
#include <SDL3/SDL.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cmath>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace kgl_engine {
static void gl_check_errors() {
    using namespace std;
    const int err = static_cast<int>(glGetError());
    if (err != GL_NO_ERROR) {
        switch (err) {
        case GL_INVALID_ENUM:
            cerr << GL_INVALID_ENUM << endl;
            break;
        case GL_INVALID_VALUE:
            cerr << GL_INVALID_VALUE << endl;
            break;
        case GL_INVALID_OPERATION:
            cerr << GL_INVALID_OPERATION << endl;
            break;
        case GL_OUT_OF_MEMORY:
            cerr << GL_OUT_OF_MEMORY << endl;
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            cerr << GL_INVALID_FRAMEBUFFER_OPERATION << endl;
            break;
        }
        assert(false);
    }
}
static void APIENTRY callback_opengl_debug(GLenum source, GLenum type,
                                           GLenum id, GLenum severity,
                                           GLsizei length,
                                           const GLchar *message,
                                           const void *userParam); 
std::istream&        operator>>(std::istream& is, vertex& v)
{
    is >> v.x;
    is >> v.y;
    return is;
}

std::istream& operator>>(std::istream& is, triangle& t)
{
    is >> t.vert[0];
    is >> t.vert[1];
    is >> t.vert[2];
    return is;
}
class gl_engine : public engine
{
    
};

} // namespace kgl_engine
