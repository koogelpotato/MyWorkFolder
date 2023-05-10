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

#define MAX_SHADER_SZ 100000

namespace kgl_engine
{
static void gl_check_errors()
{
    using namespace std;
    const int err = static_cast<int>(glGetError());
    if (err != GL_NO_ERROR)
    {
        switch (err)
        {
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
static void APIENTRY callback_opengl_debug(GLenum        source,
                                           GLenum        type,
                                           GLenum        id,
                                           GLenum        severity,
                                           GLsizei       length,
                                           const GLchar* message,
                                           const void*   userParam);
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
    std::string init(std::string_view config) final
    {
        using namespace std;

        const int init_result = SDL_Init(SDL_INIT_VIDEO);
        if (init_result != 0)
        {
            const char* err_message = SDL_GetError();
            cerr << "error: failed call SDL_Init: " << err_message << endl;
        }

        SDL_Window* window =
            SDL_CreateWindow("blank canvas", 512, 512, SDL_WINDOW_OPENGL);
        if (window == nullptr)
        {
            const char* err_message = SDL_GetError();
            cerr << "error: window uninitialized: " << err_message << endl;
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

        int         gl_major_ver       = 3;
        int         gl_minor_ver       = 2;
        int         gl_context_profile = SDL_GL_CONTEXT_PROFILE_ES;
        const char* platform_from_sdl  = SDL_GetPlatform();

        std::string_view platform{ platform_from_sdl };

        using namespace std::string_view_literals;
        using namespace std;

        auto list = { "Windows"sv, "Mac OS X"sv };
        auto it   = find(begin(list), end(list), platform);
        if (it != end(list))
        {
            gl_major_ver       = 4;
            gl_minor_ver       = (platform == "Mac OS X") ? 1 : 3;
            gl_context_profile = SDL_GL_CONTEXT_PROFILE_CORE;
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major_ver);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor_ver);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, gl_context_profile);

        SDL_GLContext mainContext = SDL_GL_CreateContext(window);

        int gl_funcitons =
            gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress);
        if (gl_funcitons != 0)
        {
            cerr << "error: failed to initialize glad" << endl;
        }

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(callback_opengl_debug, nullptr);
        glDebugMessageControl(
            GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
    GLuint load_shader_from_string(const char* vertex_shader_str,
                                   const char* fragment_shader_str)
    {
        GLuint shader_program         = glCreateProgram();
        GLuint vertex_shader_handle   = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragment_shader_handle = glCreateShader(GL_FRAGMENT_SHADER);
        { // compile shader and check for errors
            glShaderSource(vertex_shader_handle, 1, &vertex_shader_str, NULL);
            glCompileShader(vertex_shader_handle);
            int params = -1;
            glGetShaderiv(vertex_shader_handle, GL_COMPILE_STATUS, &params);
            if (GL_TRUE != params)
            {
                fprintf(stderr,
                        "ERROR: vertex shader index %u did not compile\n",
                        vertex_shader_handle);
                const int max_length    = 2048;
                int       actual_length = 0;
                char      slog[2048];
                glGetShaderInfoLog(
                    vertex_shader_handle, max_length, &actual_length, slog);
                fprintf(stderr,
                        "shader info log for GL index %u:\n%s\n",
                        vertex_shader_handle,
                        slog);

                glDeleteShader(vertex_shader_handle);
                glDeleteShader(fragment_shader_handle);
                glDeleteProgram(shader_program);
                return 0;
            }
        }

        { // compile shader and check for errors
            glShaderSource(
                fragment_shader_handle, 1, &fragment_shader_str, NULL);
            glCompileShader(fragment_shader_handle);
            int params = -1;
            glGetShaderiv(fragment_shader_handle, GL_COMPILE_STATUS, &params);
            if (GL_TRUE != params)
            {
                fprintf(stderr,
                        "ERROR: fragment shader index %u did not compile\n",
                        fragment_shader_handle);
                const int max_length    = 2048;
                int       actual_length = 0;
                char      slog[2048];
                glGetShaderInfoLog(
                    fragment_shader_handle, max_length, &actual_length, slog);
                fprintf(stderr,
                        "shader info log for GL index %u:\n%s\n",
                        fragment_shader_handle,
                        slog);

                glDeleteShader(vertex_shader_handle);
                glDeleteShader(fragment_shader_handle);
                glDeleteProgram(shader_program);
                return 0;
            }
        }

        glAttachShader(shader_program, fragment_shader_handle);
        glAttachShader(shader_program, vertex_shader_handle);
        { // link program and check for errors
            glLinkProgram(shader_program);
            glDeleteShader(vertex_shader_handle);
            glDeleteShader(fragment_shader_handle);
            int params = -1;
            glGetProgramiv(shader_program, GL_LINK_STATUS, &params);
            if (GL_TRUE != params)
            {
                fprintf(stderr,
                        "ERROR: could not link shader program GL index %u\n",
                        shader_program);
                const int max_length    = 2048;
                int       actual_length = 0;
                char      plog[2048];
                glGetProgramInfoLog(
                    shader_program, max_length, &actual_length, plog);
                fprintf(stderr,
                        "program info log for GL index %u:\n%s",
                        shader_program,
                        plog);

                glDeleteProgram(shader_program);
                return 0;
            }
        }

        return shader_program;
    }
    GLuint load_shader_from_file(const char* vertex_shader_filename,
                                 const char* fragment_shader_filename)
    {
        assert(vertex_shader_filename && fragment_shader_filename);

        printf("loading shader from files `%s` and `%s`\n",
               vertex_shader_filename,
               fragment_shader_filename);
        char vs_shader_str[MAX_SHADER_SZ];
        char fs_shader_str[MAX_SHADER_SZ];
        vs_shader_str[0] = fs_shader_str[0] = '\0';
        { // read vertex shader file into a buffer
            FILE* fp = fopen(vertex_shader_filename, "r");
            if (!fp)
            {
                fprintf(stderr,
                        "ERROR: could not open vertex shader file `%s`\n",
                        vertex_shader_filename);
                return 0;
            }
            size_t count = fread(vs_shader_str, 1, MAX_SHADER_SZ - 1, fp);
            assert(count < MAX_SHADER_SZ - 1); // file was too long
            vs_shader_str[count] = '\0';
            fclose(fp);
        }
        { // read fragment shader file into a buffer
            FILE* fp = fopen(fragment_shader_filename, "r");
            if (!fp)
            {
                fprintf(stderr,
                        "ERROR: could not open fragment shader file `%s`\n",
                        fragment_shader_filename);
                return 0;
            }
            size_t count = fread(fs_shader_str, 1, MAX_SHADER_SZ - 1, fp);
            assert(count < MAX_SHADER_SZ - 1); // file was too long
            fs_shader_str[count] = '\0';
            fclose(fp);
        }

        return load_shader_from_string(vs_shader_str, fs_shader_str);
    }
};

} // namespace kgl_engine
