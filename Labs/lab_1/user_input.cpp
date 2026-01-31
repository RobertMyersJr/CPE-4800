#include "user_input.hpp"

#include <iostream>

std::string get_user_input(std::string_view prompt) {
    std::string response;
    std::cout << prompt;
    std::cin >> response;
    return response;
}
