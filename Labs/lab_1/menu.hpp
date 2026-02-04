/**
******************************************************************************
* @file menu.hpp
* @author Robert Myers Jr.
* @version V1.0
* @brief The Menu Class works similar to a state machine where the main loop simply
* feeds the state back into the display_menu function which determines what state it 
* needs to put the menu
******************************************************************************
*/
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
        /**
         * @brief Displays the given menu.
         *
         * @param menu The menu to display
         * @return The menu to display next.
         */
        menus display_menu(menus menu);
    private:
        /**
         * @brief Displays main menu
         */
        menus display_main_menu();
        /**
         * @brief Displays register new user menu
         */
        void register_newuser_menu();
        /**
         * @brief Displays change password menu
         */
        void change_password_menu();
        std::string current_user_;
};
