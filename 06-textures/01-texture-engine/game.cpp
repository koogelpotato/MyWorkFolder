#include "game.hpp"
#include "resource-manager.hpp"
#include "sprite-renderer.hpp"



Sprite_Renderer  *renderer;


Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), keys(), width(width), height(height)
{ 

}

Game::~Game()
{
    delete renderer;
}

void Game::init_game()
{
    
    Resource_Manager::load_shader("sprite.vs", "sprite.frag", "sprite");

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width), 
        static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);
    Resource_Manager::get_shader("sprite").use_current_shader().set_integer("image", 0);
    Resource_Manager::get_shader("sprite").set_matrix4("projection", projection);

    renderer = new Sprite_Renderer(Resource_Manager::get_shader("sprite"));

    Resource_Manager::load_texture("skull.png", true, "skull");
}

void Game::update(float dt)
{
    
}

void Game::proccess_input(float dt)
{
   
}

void Game::render()
{
    renderer->draw_sprite(Resource_Manager::get_texture("skull"), glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}