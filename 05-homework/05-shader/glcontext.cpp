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
#define OM_GL_CHECK()                                                          \
    {                                                                          \
        const GLenum err = glGetError();                                       \
        if (err != GL_NO_ERROR)                                                \
        {                                                                      \
            switch (err)                                                       \
            {                                                                  \
                case GL_INVALID_ENUM:                                          \
                    std::cerr << "GL_INVALID_ENUM" << std::endl;               \
                    break;                                                     \
                case GL_INVALID_VALUE:                                         \
                    std::cerr << "GL_INVALID_VALUE" << std::endl;              \
                    break;                                                     \
                case GL_INVALID_OPERATION:                                     \
                    std::cerr << "GL_INVALID_OPERATION" << std::endl;          \
                    break;                                                     \
                case GL_INVALID_FRAMEBUFFER_OPERATION:                         \
                    std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION"            \
                              << std::endl;                                    \
                    break;                                                     \
                case GL_OUT_OF_MEMORY:                                         \
                    std::cerr << "GL_OUT_OF_MEMORY" << std::endl;              \
                    break;                                                     \
            }                                                                  \
            std::cerr << __FILE__ << ':' << __LINE__ << '(' << __FUNCTION__    \
                      << ')' << std::endl;                                     \
            assert(false);                                                     \
        }                                                                      \
    }

namespace kgl_engine
{

std::istream& operator>>(std::istream& is, vertex& v)
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
class gl_engine final : public engine
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

        window = SDL_CreateWindow("blank canvas", 512, 512, SDL_WINDOW_OPENGL);
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

        SDL_GLContext context = SDL_GL_CreateContext(window);

        if (context == nullptr)
        {
            std::cerr << "context not initialized" << std::endl;
        }

        int gl_functions =
            gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress);

        if (gl_functions != 0)
        {
            cerr << "error: failed to initialize glad" << endl;
        }
        program = create_shader_from_file("myshader.frag", "myshader.vert");
        return "";
    }
    GLuint create_shader_from_string(const char* vertex_shader_str,
                                     const char* fragment_shader_str) final
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
                std::cerr << "ERROR: vertex shader index"
                          << vertex_shader_handle << "did not compile"
                          << std::endl;
                const int max_length    = 2048;
                int       actual_length = 0;
                char      slog[2048];
                glGetShaderInfoLog(
                    vertex_shader_handle, max_length, &actual_length, slog);
                std::cerr << "shader info log for GL index: "
                          << vertex_shader_handle << " " << slog << std::endl;

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
                std::cerr << "ERROR: vertex shader index"
                          << fragment_shader_handle << "did not compile"
                          << std::endl;
                const int max_length    = 2048;
                int       actual_length = 0;
                char      slog[2048];
                glGetShaderInfoLog(
                    fragment_shader_handle, max_length, &actual_length, slog);
                std::cerr << "shader info log for GL index: "
                          << fragment_shader_handle << " " << slog << std::endl;

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
                std::cerr << "ERROR: could not link shader program gl index "
                          << shader_program << std::endl;
                const int max_length    = 2048;
                int       actual_length = 0;
                char      plog[2048];
                glGetProgramInfoLog(
                    shader_program, max_length, &actual_length, plog);
                std::cerr << "program info log for GL index " << shader_program
                          << plog << std::endl;

                glDeleteProgram(shader_program);
                return 0;
            }
        }

        return shader_program;
    }
    GLuint create_shader_from_file(const char* vertex_shader_filename,
                                   const char* fragment_shader_filename) final
    {
        assert(vertex_shader_filename && fragment_shader_filename);

        std::cout << "loading shader from files: " << vertex_shader_filename
                  << " and " << fragment_shader_filename << std::endl;
        char vs_shader_str[MAX_SHADER_SZ];
        char fs_shader_str[MAX_SHADER_SZ];
        vs_shader_str[0] = fs_shader_str[0] = '\0';
        { // read vertex shader file into a buffer
            FILE* fp = fopen(vertex_shader_filename, "r");
            if (!fp)
            {
                std::cerr << "ERROR: could not open vertex shader file"
                          << vertex_shader_filename << std::endl;
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
                std::cerr << "ERROR: could not open fragment shader file"
                          << fragment_shader_filename << std::endl;
                return 0;
            }
            size_t count = fread(fs_shader_str, 1, MAX_SHADER_SZ - 1, fp);
            assert(count < MAX_SHADER_SZ - 1); // file was too long
            fs_shader_str[count] = '\0';
            fclose(fp);
        }

        return create_shader_from_string(vs_shader_str, fs_shader_str);
    }
    void hot_reload_shader_file(GLuint*     shader,
                                const char* vertex_shader_filename,
                                const char* fragment_shader_filename) final
    {
        assert(program && vertex_shader_filename && fragment_shader_filename);

        GLuint reloaded_program = create_shader_from_file(
            vertex_shader_filename, fragment_shader_filename);

        if (reloaded_program)
        {
            glDeleteProgram(*shader);
            *shader = reloaded_program;
        }
    }

    bool input(bool& running) final
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                std::cout << "key pressed: "
                          << SDL_GetKeyName(event.key.keysym.sym) << std::endl;
            }
            else if (event.type == SDL_EVENT_KEY_UP)
            {
                std::cout << "key released: "
                          << SDL_GetKeyName(event.key.keysym.sym) << std::endl;
            }
            else if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
                SDL_Quit();
                return EXIT_SUCCESS;
            }
        }
        return EXIT_FAILURE;
    }
    void triangle_render(const triangle& t) final
    {
        glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), &t.vert[0]);
        gl_check_errors();
        glEnableVertexAttribArray(0);
        gl_check_errors();
        glValidateProgram(program);
        gl_check_errors();
        GLint validate_status = 0;
        glGetProgramiv(program, GL_VALIDATE_STATUS, &validate_status);
        gl_check_errors();
        if (validate_status == GL_FALSE)
        {
            std::cerr << "Error linking program: " << std::endl;
            throw std::runtime_error("error");
        }
        glDrawArrays(GL_TRIANGLES, 0, 3);
        gl_check_errors();
    }

    void uninitialize() final
    {
        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    void swap_buffer() final
    {
        SDL_GL_SwapWindow(window);

        glClearColor(1.0f, 0.3f, 0.3f, 0.0f);
        gl_check_errors();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gl_check_errors();
    }
    void gl_check_errors()
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

private:
    SDL_Window*   window     = nullptr;
    GLuint        program    = 0;
    SDL_GLContext gl_context = nullptr;
};
static bool engine_exist = false;
engine*     create_engine()
{
    if (engine_exist)
    {
        throw std::runtime_error("engine exist");
    }
    engine_exist = true;
    return new gl_engine();
}
void destroy_engine(engine* e)
{
    if (!engine_exist)
    {
        throw std::runtime_error("engine not created");
    }
    if (nullptr == e)
    {
        throw std::runtime_error("e is nullptr");
    }
    delete e;
}
engine::~engine() {}

} // namespace kgl_engine
