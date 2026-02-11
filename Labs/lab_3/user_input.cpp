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
#include <string>

std::string get_user_input(std::string_view prompt) {
    std::string response;
    std::string current_word_in_response;
    std::cout << prompt;
    
    std::getline(std::cin, response);

    return response;
}
