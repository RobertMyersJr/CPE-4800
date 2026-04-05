/**
******************************************************************************
* @file user_input.hpp
* @author Robert Myers Jr.
* @version V1.0
* @brief Header file that declares the get_user_input helper function
******************************************************************************
*/
#pragma once
#include <string>

/**
 * @brief Get input from user 
 *
 * @param prompt The prompt to be printed out 
 */
std::string get_user_input(std::string_view prompt);
