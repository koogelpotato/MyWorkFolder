#include "engine.hxx"
#include <chrono>
#include <iostream>
#include <ostream>
#include <thread>
#include <chrono>

class my_console_game : public game
{
public:
    explicit my_console_game(engine&)
        : progress{ 0.0 }
        , barwidth{ 150 }
        , pos{ 0 }
        ,anim_speed{0.1}
    {
    }

    void init() override {}
    void on_event() override {}
    void update() override
    {
        if(progress < 1.0)
        {
            pos = barwidth * progress;
            progress += anim_speed; 
            
        }
        else
        {
            progress = 0;
            pos = 0;
        }

        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    void render() const override
    {
        std::cout << "[";
        for (int i = 0; i < barwidth; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << int(progress * 100.0) << " %\r" ;
        std::cout.flush();
    }

private:
    float progress;
    uint32_t barwidth;
    uint32_t pos;
    double anim_speed;
};

game* create_game(engine* engine)
{
    if (engine != nullptr)
    {
        return new my_console_game(*engine);
    }

    return nullptr;
}