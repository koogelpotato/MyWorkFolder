#include <iostream>

#include <SDL3/SDL.h>
#include <keybind.hpp>
#include <keybindfunctions.hpp>

bool init(); 

SDL_Window* window; 

int main(int argc, char *argv[])
{
    if (!init())
    {
        using namespace std;
        cout << "Failed to initialize! SDL Error:\n" << SDL_GetError() << endl;
        return 1;
    }

    bool quit = false;

    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            quit = process_key(event, keymap);
        }
    }

    close();
    return 0;
}
bool init()
{
    using namespace std;
    bool success = true;
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		cout <<  "SDL could not initialize! SDL Error:\n" << SDL_GetError() << endl;
		success = false;
	}
    else
    {
        window = SDL_CreateWindow( "infinite loop", 640, 480, SDL_WINDOW_OPENGL);
		if( window == NULL )
		{
            using namespace std;
			cout << "Window could not be created! SDL Error:\n" << SDL_GetError() << endl;
			success = false;
		}
    }
    return success;
}
void close()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool process_key(SDL_Event& event, std::unordered_map<SDL_Keycode, keybinds>& keymap)
{
    using namespace std;
    bool quit = false;

    if (event.type == SDL_EVENT_KEY_DOWN) {
        auto it = keymap.find(event.key.keysym.sym);
        if (it != keymap.end()) {
            switch (it->second) {
                case KEY_PRESS_UP:
                    cout << "UP key pressed\n" << endl;
                    break;
                case KEY_PRESS_DOWN:
                    cout << "DOWN key pressed\n" << endl;
                    break;
                case KEY_PRESS_LEFT:
                    cout << "LEFT key pressed\n" << endl;
                    break;
                case KEY_PRESS_RIGHT:
                    cout << "RIGHT key pressed\n" << endl;
                    break;
                case KEY_PRESS_ACTION1:
                    cout << "ACTION1 key pressed\n" << endl;
                    break;
                case KEY_PRESS_ACTION2:
                    cout << "ACTION2 key pressed\n" << endl;
                    break;
                case KEY_PRESS_REMAP :
                {
                    cout << "Input a key to remap\n" << endl;
                    SDL_Keycode old_key = event.key.keysym.sym;
                    SDL_Delay(1000);
                    cout << "Input the new key" << endl;
                    SDL_Keycode new_key = event.key.keysym.sym;
                    remap_key(old_key,new_key, keymap);
                }
                    break;
                case KEY_PRESS_PRINTALL:
                    print_all_keys(keymap);
                    break;
                case KEY_PRESS_QUIT:
                    cout << "Thank you, and goodbye!" << endl;
                    quit = true;
                    break;
                default:
                    break;
            }
        }
    }

    return quit;
}
void remap_key(SDL_Keycode old_key, SDL_Keycode new_key, std::unordered_map<SDL_Keycode, keybinds>& keymap)
{
    auto it = keymap.find(old_key);
    if (it != keymap.end()) {
        auto keybind = it->second;
        keymap.erase(it);
        keymap[new_key] = keybind;
    }
}
void print_all_keys(std::unordered_map<SDL_Keycode, keybinds>& keymap)
{
    using namespace std;
    auto print_key_value = [](const auto& key, const auto& value)
    {
        cout << "Key:[" << key << "] Action:[" << value << "]\n";
    };
}