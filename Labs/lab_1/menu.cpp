#include "menu.hpp"
#include "password_file.hpp"
#include "user_input.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>

void Menu::register_newuser_menu() {
    PasswordFile password_file;

    auto users_and_passwords = password_file.get_current_users_and_passwords();

    auto username = get_user_input("Username: ");

    bool user_exists = false;

    for(auto user_credentials : users_and_passwords) {
        if(user_credentials.username_ == username) {
            user_exists = true;
            break;
        }
    }

    if(user_exists) {
        std::cout << "User is already in database. Leaving user registration menu\n";
    }

    auto password = get_user_input("Password: ");

    if(!password_file.check_if_password_is_valid(password)) {
        return;
    }
    password_file.write_encrypted_username_and_password(username, password);
    std::cout << "User has been registered!\n";
}

void Menu::change_password_menu() {
    PasswordFile password_file;

    auto password = get_user_input("Password: ");

    if(password_file.check_if_password_is_valid(password)) {
        password_file.update_user_and_password(current_user_, password);
    }
}

menus Menu::display_menu(menus menu) {
    switch(menu) {
        case MAIN_MENU:
            return display_main_menu();
        case REGISTER_NEW_USER_MENU:
            register_newuser_menu();
            return MAIN_MENU;
        case CHANGE_PASSWORD_MENU:
            change_password_menu();
            return MAIN_MENU;
        case EXIT_MENU:
            exit(0);
        default:
            return display_main_menu();
    };
}


menus Menu::display_main_menu() {
    auto input = get_user_input("Type 'newuser' to add a new user, 'changepass' to change your password, or 'logout' to exit: ");

    if(input == "newuser") {
        return REGISTER_NEW_USER_MENU;
    }
    if(input == "changepass") {
        return CHANGE_PASSWORD_MENU;
    }
    if(input == "logout") {
        return EXIT_MENU;
    } 
    std::cout << "Error: not a valid option please try again\n";
    return MAIN_MENU;
}

Menu::Menu(std::string_view current_user) : current_user_(current_user) {
}
