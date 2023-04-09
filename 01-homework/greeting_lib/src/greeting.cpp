#include "greeting.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

bool greet(const std::string &user_name) {
  if (user_name.empty()) {
    return false;
  }
  std::cout << "Hello, " << user_name << "!" << std::endl;
  return true;
}