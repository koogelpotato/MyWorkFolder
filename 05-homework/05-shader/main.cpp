#include <algorithm>
#include <array>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string_view>

#include "glcontext.hpp"

int main()
{
    using namespace std;
    unique_ptr<kgl_engine::engine, void (*)(kgl_engine::engine*)> engine(
        kgl_engine::create_engine(), kgl_engine::destroy_engine);
    engine->init("");

    bool running = true;
    while (running)
    {
        engine->input(running);
        if (running == false)
        {
            return EXIT_SUCCESS;
        }

        kgl_engine::triangle tri;
        engine->triangle_render(tri, 25, 15);
        engine->swap_buffer();
    }
    engine->uninitialize();
    return EXIT_SUCCESS;
}