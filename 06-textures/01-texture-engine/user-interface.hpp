#pragma once

#include <SDL3/SDL.h>
#include "imgui_pack/imgui.h"
#include "imgui_pack/imgui_impl_opengl3.h"
#include "imgui_pack/imgui_impl_sdl3.h"

class User_Interface
{
public:
    User_Interface(){};

    void ui_init(SDL_Window* window, SDL_GLContext* gl_context);
    void ui_handle_event(const SDL_Event& event);
    void ui_init_new_frame();
    void ui_render_data();
    void ui_shutdown();
};

