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
#include <fstream>

#define MAX_SHADER_SZ 100000 //TO DO Fix issues, check uniforms

namespace kgl_engine
{
    void opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                               GLsizei length, const GLchar* message, const void* userParam)
{
    std::cerr << "OpenGL Error:" << std::endl;
    std::cerr << "  Source: " << source << std::endl;
    std::cerr << "  Type: " << type << std::endl;
    std::cerr << "  ID: " << id << std::endl;
    std::cerr << "  Severity: " << severity << std::endl;
    std::cerr << "  Message: " << message << std::endl;
    std::cerr << std::endl;
}

void listen_opengl_errors()
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glDebugMessageCallback(opengl_debug_callback, nullptr);

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
}

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

        if (gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress) == 0)
        {
            cerr << "error: failed to initialize glad" << endl;
        }
        
        program = create_shader_from_file("shader/filemyshader.vert", "shader/filemyshader.frag");
        return "";
    }
    GLuint create_shader_from_string(const char* vertex_shader_str,
                                     const char* fragment_shader_str) final
    {

        GLuint shader_program         = glCreateProgram();
        listen_opengl_errors();
        GLuint vertex_shader_handle   = glCreateShader(GL_VERTEX_SHADER);
        listen_opengl_errors();
        GLuint fragment_shader_handle = glCreateShader(GL_FRAGMENT_SHADER);
        listen_opengl_errors();
        { // compile shader and check for errors
            glShaderSource(vertex_shader_handle, 1, &vertex_shader_str, NULL);
            listen_opengl_errors();
            glCompileShader(vertex_shader_handle);
            listen_opengl_errors();
            int params = -1;
            glGetShaderiv(vertex_shader_handle, GL_COMPILE_STATUS, &params);
            listen_opengl_errors();
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
                    listen_opengl_errors();
                std::cerr << "shader info log for GL index: "
                          << vertex_shader_handle << " " << slog << std::endl;

                glDeleteShader(vertex_shader_handle);
                listen_opengl_errors();
                glDeleteShader(fragment_shader_handle);
                listen_opengl_errors();
                glDeleteProgram(shader_program);
                listen_opengl_errors();
                return 0;
            }
        }

        { // compile shader and check for errors
            glShaderSource(
                fragment_shader_handle, 1, &fragment_shader_str, NULL);
                listen_opengl_errors();
            glCompileShader(fragment_shader_handle);
            listen_opengl_errors();
            int params = -1;
            glGetShaderiv(fragment_shader_handle, GL_COMPILE_STATUS, &params);
            listen_opengl_errors();
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
                listen_opengl_errors();
                glDeleteShader(vertex_shader_handle);
                listen_opengl_errors();
                glDeleteShader(fragment_shader_handle);
                listen_opengl_errors();
                glDeleteProgram(shader_program);
                listen_opengl_errors();
                return 0;
            }
        }

        glAttachShader(shader_program, fragment_shader_handle);
        glAttachShader(shader_program, vertex_shader_handle);
        { // link program and check for errors
            glLinkProgram(shader_program);
            listen_opengl_errors();
            glDeleteShader(vertex_shader_handle);
            listen_opengl_errors();
            glDeleteShader(fragment_shader_handle);
            listen_opengl_errors();
            int params = -1;
            glGetProgramiv(shader_program, GL_LINK_STATUS, &params);
            listen_opengl_errors();
            if (GL_TRUE != params)
            {
                std::cerr << "ERROR: could not link shader program gl index "
                          << shader_program << std::endl;
                const int max_length    = 2048;
                int       actual_length = 0;
                char      plog[2048];
                glGetProgramInfoLog(
                    shader_program, max_length, &actual_length, plog);
                    listen_opengl_errors();
                std::cerr << "program info log for GL index " << shader_program
                          << plog << std::endl;

                glDeleteProgram(shader_program);
                listen_opengl_errors();
                return 0;
            }
        }

        return shader_program;
    }
    
    std::string load_file(std::string_view path)
    {
        std::cout << path << "\tloading" << std::endl;
        std::ifstream file(path.data(), std::ios_base::in | std::ios_base::ate);
        file.exceptions(std::ios_base::failbit);
        auto size = file.tellg();
        std::string result(size,'\0');
        file.seekg(0);
        file.read(&result[0],size);
        return result;
    }
    GLuint create_shader_from_file(const char* vertex_shader_filename,
                                   const char* fragment_shader_filename) final
    {
        assert(vertex_shader_filename && fragment_shader_filename);

        std::cout << "loading shader from file: " << vertex_shader_filename << "and: "
                    << fragment_shader_filename<< std::endl; 
        auto vs_shader_str  = load_file(vertex_shader_filename);
        auto fs_shader_str = load_file(fragment_shader_filename);

        return create_shader_from_string(vs_shader_str.c_str(), fs_shader_str.c_str());
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
            listen_opengl_errors();
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
    void triangle_render(const triangle& t,int count, float size) final
    {
    std::vector<float> colors;
    for (int i = 0; i < count * 3 * 3; ++i)
    {
        colors.push_back(static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    }

 
    std::vector<float> vertices;
    vertices.reserve(count * 3 * 3);
    for (int i = 0; i < count; ++i)
    {
        float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * size - size / 2.0f;
        float y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * size - size / 2.0f;
        float z = 0.0f;
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);

        x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * size - size / 2.0f;
        y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * size - size / 2.0f;
        z = 0.0f;
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);

        x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * size - size / 2.0f;
        y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * size - size / 2.0f;
        z = 0.0f;
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    }

    
    glVertexAttribPointer(
        0,          
        3,          
        GL_FLOAT,   
        GL_FALSE,   
        0,          
        vertices.data() 
    );
    listen_opengl_errors();
    glVertexAttribPointer(
        1,          
        3,          
        GL_FLOAT,   
        GL_FALSE,   
        0,          
        colors.data()   
    );
    listen_opengl_errors();

    
    glEnableVertexAttribArray(0);
    listen_opengl_errors();
    glEnableVertexAttribArray(1);
    listen_opengl_errors();

    
    glDrawArrays(GL_TRIANGLES, 0, count * 3); 
    listen_opengl_errors();
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

        glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
        listen_opengl_errors();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        listen_opengl_errors();
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
