/**
******************************************************************************
* @file password_file.hpp
* @author Robert Myers Jr.
* @version V1.0
* @brief Class used to read and write the password file.
******************************************************************************
*/
#pragma once
#include <vector>
#include <string>
#include <string_view>

/**
 * @class user_credential_t
 * @brief A struct that represents a user and their password read from the text file
 */
struct user_credential_t {
    user_credential_t(std::string_view username, std::string_view password);
    std::string username_;
    std::string password_;
};

class PasswordFile {
    public:
        /**
         * @brief Writes a username and password in plain text to the textfile
         */
        void write_plain_username_and_password(std::string_view username, std::string_view password);
        /**
         * @brief Writes a username and password in plain text to the textfile where the password is 
         * encrypted
         */
        void write_encrypted_username_and_password(std::string_view username, std::string_view password);
        /**
         * @brief Check if username and password are in the text file.
         */
        bool check_for_credentials(std::string_view user, std::string_view password);

        /**
         * @brief Check if a password is valid. I.e. upper and lower case, 8+ characters, and has a symbol
         */
        bool check_if_password_is_valid(std::string_view password);

        /**
         * @brief Gets a list of users
         */
        std::vector<user_credential_t> get_current_users_and_passwords();
        
        /**
         * @brief Update a specific user's password
         */
        void update_user_and_password(std::string_view username, std::string_view password);
    private:
        /**
         * @brief Write credentials to file
         */
        void write_credentials_to_file(std::string_view user_credentials);
        /**
         * @brief Helper method for getting the sha256 of a string
         */
        std::string get_sha256_string(std::string_view str);
};
