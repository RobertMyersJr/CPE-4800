/**
******************************************************************************
* @file user_input.cpp
* @author Robert Myers Jr.
* @version V1.0
* @brief Implementation of the get_user_input function to get user input
* verification.
******************************************************************************
*/
#include "user_input.hpp"

#include <iostream>

std::string get_user_input(std::string_view prompt) {
    std::string response;
    std::cout << prompt;
    std::cin >> response;
    return response;
}
