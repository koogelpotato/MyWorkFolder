#include "engine.hpp"
#include "keybind.hpp"
#include <SDL3/SDL.h>
#include <algorithm>
#include <iostream>
#include <string_view>
#include <unordered_map>
class engine_impl final : public engine {
public:
  engine_impl() {}
  SDL_Window *window;
  virtual std::string initialize(std::string_view config) override {

    using namespace std;
    cout << "Current videodriver: " << SDL_GetCurrentVideoDriver();
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      success = false;
      return "SDL could not initialize! SDL Error:\n" + (string)SDL_GetError();
    } else {
      cout << "Current video driver:" << SDL_GetCurrentVideoDriver() << endl;
      window = SDL_CreateWindow("infinite loop", 640, 480, SDL_WINDOW_OPENGL);
      if (window == NULL) {
        success = false;
        return "Window could not be created! SDL Error:\n" +
               (string)SDL_GetError();
      }
    }

    return "Initialized succesfully";
  }

  virtual bool
  read_input(SDL_Event &event,
             std::unordered_map<SDL_Keycode, keybinds> &keymap) override {
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
        case KEY_PRESS_REMAP: {
          cout << "Input a key to remap\n" << endl;
          SDL_Keycode old_key = event.key.keysym.sym;
          SDL_Delay(5000);
          cout << "Input the new key" << endl;
          SDL_Keycode new_key = event.key.keysym.sym;
          remap_key(old_key, new_key, keymap);
        } break;
        case KEY_PRESS_PRINTALL: {
          cout << "Here are the keys: " << endl;
          print_all_keys(keymap);
          break;
        }
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

  virtual void uninitialize() override {
    SDL_DestroyWindow(window);
    SDL_Quit();
  }
  virtual void
  print_all_keys(std::unordered_map<SDL_Keycode, keybinds> &keymap) override {
    using namespace std;
    for_each(keymap.begin(), keymap.end(), [](const auto &key_value_pair) {
      cout << "Key:[" << key_value_pair.first << "] Action:["
           << key_value_pair.second << "]\n";
    });
  }
  virtual void
  remap_key(SDL_Keycode old_key, SDL_Keycode new_key,
            std::unordered_map<SDL_Keycode, keybinds> &keymap) override {
    auto it = keymap.find(old_key);
    if (it != keymap.end()) {
      auto keybind = it->second;
      keymap.erase(it);
      keymap[new_key] = keybind;
    }
  }

  virtual ~engine_impl() override {}
};

static bool already_exist = false;

engine *create_engine() {
  if (already_exist) {
    throw std::runtime_error("engine already exist");
  }
  engine *result = new engine_impl();
  already_exist = true;
  return result;
}

void destroy_engine(engine *e) {
  if (!already_exist) {
    throw std::runtime_error("engine not created");
  }
  if (nullptr == e) {
    throw std::runtime_error("e is nullptr");
  }
  delete e;
}
engine::~engine() = default;
