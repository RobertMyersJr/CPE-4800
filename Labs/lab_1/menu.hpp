#pragma once

enum menus {
    MAIN_MENU,
    REGISTER_NEW_USER_MENU,
    CHANGE_PASSWORD_MENU,
    EXIT_MENU
};
class Menu {
    public:
        menus display_menu(menus menu);
    private:
        menus display_main_menu();
        void register_newuser_menu();
        void change_password_menu();
};
