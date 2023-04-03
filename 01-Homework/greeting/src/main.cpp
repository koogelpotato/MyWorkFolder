#include <cstdlib>
#include <cstring>
#include <greeting.hpp>

int main(){
    const char* user_name_cstr = std::getenv("USER");
    std::string user_name = user_name_cstr ? user_name_cstr : "unknown";

    greet(user_name[0]);

    return 0;
}