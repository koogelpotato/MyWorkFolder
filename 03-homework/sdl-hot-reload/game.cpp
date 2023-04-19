#include "engine.hpp"

#include <iostream>
#include <array>
class new_game : public game
{
public:
    explicit new_game(engine&) : progress{0}, barwidth{100}, pos{0}
    {
    }
    void create_game() override {}
    void on_event(SDL_Event& event) override {}
    void update() override
    {
        while (progress < 1.0) {
        int pos = barwidth * progress;
        std::cout.flush();
 
        progress += 0.16; 
    }
    std::cout << std::endl;
    }
    void render() const override
    {
        std::cout << "[";
        for (int i = 0; i < barwidth; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << int(progress * 100.0) << " %\r";
    }

private:
    uint32_t                  progress = 0;
    uint32_t                  barwidth = 0;
    uint32_t                  pos = 0;
};

game* create_game(engine* engine)
{
    if (engine != nullptr)
    {
        return new new_game(*engine);
    }
    return nullptr;
}

[[maybe_unused]] void destroy_game(game* game)
{
    delete game;
}