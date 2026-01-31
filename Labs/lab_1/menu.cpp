#include "menu.hpp"
#include "user_input.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

void Menu::register_newuser_menu() {
    auto username = get_user_input("Username: ");
    auto password = get_user_input("Password: ");
}

void Menu::change_password_menu() {
    // Find user
    auto password = get_user_input("Password: ");
    std::cout << "";
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
    std::cout << "Error: not a valid option please try again";
    return MAIN_MENU;
}
