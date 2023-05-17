#pragma once

#include <map>
#include <string>

#include "glad/glad.h"

#include "texture.hpp"
#include "shader.hpp"

class Resource_Manager
{
public:
    static std::map<std::string, Shader>    shaders;
    static std::map<std::string, Texture> textures;
   
    static Shader    load_shader(const char *vertex_shader_file, const char *fragment_shader_file, std::string name);

    static Shader&    get_shader(std::string name);

    static Texture load_texture(const char *file, bool alpha, std::string name);

    static Texture& get_texture(std::string name);

    static void      clear();
private:
    
    Resource_Manager() { }
    
    static Shader    load_shader_from_file(const char *vertex_shader_file, const char *fragment_shader_file);
    
    static Texture load_texture_from_file(const char *file, bool alpha);
};