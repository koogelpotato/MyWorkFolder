#include "user-interface.hpp"
#include <iostream>

void User_Interface::ui_init(SDL_Window* window, SDL_GLContext* gl_context)
{
    ImGui::CreateContext();
    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 300 es");
    ImGui::StyleColorsDark();
    std::cout << "Inited user_interface" << std::endl;
};

void User_Interface::ui_init_new_frame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
}
void User_Interface::ui_render_data()
{
    ImGui::Render();
}
void User_Interface::ui_shutdown()
{
    ImGui_ImplSDL3_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    std::cout << "Shut down user_interface" << std::endl;
}

void User_Interface::ui_handle_event(const SDL_Event& event)
{
    ImGui_ImplSDL3_ProcessEvent(&event);
}