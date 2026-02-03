/**
 * TODO
 */

#include "password_file.hpp"
#include "menu.hpp"
#include "user_input.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <string_view>
#include <format>

std::string get_username() {
    return get_user_input("Enter username: ");
}

std::string get_password() {
    return get_user_input("Enter password: ");
}

std::string get_six_digit_code(std::string_view code) {
    std::string prompt = std::format("Enter the 6-digit code '{}' : ", code);
    return get_user_input(prompt);
}

std::string check_credentials() {
    PasswordFile passwordFile;

    auto username = get_username();
    auto password = get_password();

    if(!passwordFile.check_for_credentials(username, password)) {
        std::printf("The provided username and/or password is invalid. Please try again\n");
        exit(0);
    }
    return username;
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
    srand(time(0));
    auto username = check_credentials();
    digit_code();
    Menu menu(username);
    menus current_menu = MAIN_MENU;
    while(1) {
        current_menu = menu.display_menu(current_menu);
    };
}
