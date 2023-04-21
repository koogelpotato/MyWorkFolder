#include <string>
#include <string_view>

class engine;

engine* create_engine();
void    destroy_engine(engine* e);
class engine
{
public:
    virtual ~engine();
    virtual std::string init(std::string_view config) = 0;
    virtual bool        input()                       = 0;
    virtual void        uninitialize()                = 0;
};
struct game
{
    virtual ~game()             = default;
    virtual void init()         = 0;
    virtual void update()       = 0;
    virtual void on_event()     = 0;
    virtual void render() const = 0;
};

extern "C" game* create_game(engine*);