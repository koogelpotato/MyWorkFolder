#include "game.hpp"
#include "resource-manager.hpp"
#include "sprite-renderer.hpp"
#include "game-object.hpp"




Sprite_Renderer  *renderer;
Game_Object *player;


Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), keys(), width(width), height(height)
{ 

}

Game::~Game()
{
    delete renderer;
    delete player;
}

void Game::init_game()
{
    
    Resource_Manager::load_shader("/home/koogel/.config/MyWorkFolder/06-textures/01-texture-engine/sprite.vs", "/home/koogel/.config/MyWorkFolder/06-textures/01-texture-engine/sprite.frag", "sprite");

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width), 
        static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);
    Resource_Manager::get_shader("sprite").use_current_shader().set_integer("image", 0);
    Resource_Manager::get_shader("sprite").set_matrix4("projection", projection);

    renderer = new Sprite_Renderer(Resource_Manager::get_shader("sprite"));

    Resource_Manager::load_texture("/home/koogel/.config/MyWorkFolder/06-textures/01-texture-engine/skull.png", true, "skull");
    
    glm::vec2 playerPos = glm::vec2(this->width / 2.0f - PLAYER_SIZE.x / 2.0f, this->height - PLAYER_SIZE.y);
    player = new Game_Object(playerPos, PLAYER_SIZE, Resource_Manager::get_texture("skull") );
}


void Game::update(float dt)
{
    
}

void Game::proccess_input(float dt)
{
    SDL_Event event;
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        /*if (this->keys[SDL_SCANCODE_A])
        {
            if (player->position.x >= 0.0f)
            {
                player->position.x -= velocity;
                player->rotation = 270.0f;;
            }
                
        }
        if (this->keys[SDL_SCANCODE_D])
        {
            if (player->position.x <= this->width - player->size.x)
            {
                player->position.x += velocity;
                player->rotation = 90.0f;
            }
                
        }
        if (this->keys[SDL_SCANCODE_S])
        {
            if (player->position.y <= this->height - player->size.y)
            {
                player->position.y += velocity;
                player->rotation = 180.0f;
            }
                
        }
        if (this->keys[SDL_SCANCODE_W])
        {
            if (player->position.y >= 0.0f)
            {
                player->position.y += velocity;
                player->rotation = 0.0f;
            }
                
        }*/

        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_EVENT_KEY_DOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDL_SCANCODE_A:
                            if (player->position.x >= 0.0f)
                            {
                                player->position.x -= velocity;
                                player->rotation = 270.0f;;
                            }
                            break;
                        case SDL_SCANCODE_D:
                            if (player->position.x <= this->width - player->size.x)
                            {
                                player->position.x += velocity;
                                player->rotation = 90.0f;
                            }
                            break;
                        case SDL_SCANCODE_W:
                            if (player->position.y >= 0.0f)
                            {
                                player->position.y += velocity;
                                player->rotation = 0.0f;
                            }
                            break;
                        case SDL_SCANCODE_S:
                            if (player->position.y <= this->height - player->size.y)
                            {
                                player->position.y += velocity;
                                player->rotation = 180.0f;
                            }
                            break;    
                        default:
                        break;
                    }
                case SDL_EVENT_QUIT:
                    this->State = GAME_EXIT;
                    break;
                default:
                    break;

            }
                

        }
    }
}

void Game::render()
{
    player->Draw(*renderer);
}