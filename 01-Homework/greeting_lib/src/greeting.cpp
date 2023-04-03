#include <cstdlib>
#include <string>
#include <iostream>
#include "greeting.hpp"

bool greet(const std::string& user_name){
    if(user_name.empty()){
        return false;
    }
    std::cout << "Hello, " << user_name << "!" << std::endl;
    return true;
}