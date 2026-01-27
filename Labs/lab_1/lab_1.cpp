/**
 * TODO
 */

#include "password_file.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <string_view>

std::string get_user_input(std::string_view prompt) {
    std::string response;
    std::cout << prompt;
    std::cin >> response;
    return response;
}
std::string get_username() {
    return get_user_input("Enter username: ");
}

std::string get_password() {
    return get_user_input("Enter password: ");
}

std::string get_six_digit_code(std::string_view code) {
    std::string prompt = "Enter the 6-digit code: ";
    prompt += code;

    return get_user_input(prompt);
}

void check_credentials() {
    PasswordFile passwordFile;

    auto username = get_username();
    auto password = get_password();

    if(!passwordFile.check_for_credentials(username, password)) {
        std::printf("The provided username and/or password is invalid. Please try again\n");
        exit(0);
    }
}

unsigned int generate_code() {
    return ((unsigned int)rand()) % 1'000'000;
}

void digit_code() {
    std::string expected_code = std::string(
            std::to_string(generate_code())
            );
    auto six_digit_code = get_six_digit_code(expected_code);

    if(expected_code != six_digit_code) {
        std::printf("Incorrect code. Please try again\n");
        exit(0);
    }

}
int main() {
    // Seed the random generator at the beginning
    srand(0);
    check_credentials();
}
