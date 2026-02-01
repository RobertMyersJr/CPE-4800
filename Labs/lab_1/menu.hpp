#pragma once

#include <string>
#include <string_view>
enum menus {
    MAIN_MENU,
    REGISTER_NEW_USER_MENU,
    CHANGE_PASSWORD_MENU,
    EXIT_MENU
};
class Menu {
    public:
        Menu(std::string_view current_user);
        menus display_menu(menus menu);
    private:
        menus display_main_menu();
        void register_newuser_menu();
        void change_password_menu();
        std::string current_user_;
};
