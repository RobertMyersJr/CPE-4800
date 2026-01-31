#pragma once
#include <vector>
#include <string>
#include <string_view>

struct user_credential_t {
    user_credential_t(std::string_view username, std::string_view password);
    std::string username_;
    std::string password_;
};

class PasswordFile {
    public:
        void write_plain_username_and_password(std::string_view username, std::string_view password);
        void write_encrypted_username_and_password(std::string_view username, std::string_view password);
        bool check_for_credentials(std::string_view user, std::string_view password);

        bool check_if_password_is_valid(std::string_view password);

        std::vector<user_credential_t> get_current_users_and_passwords();
        
        void update_user_and_password(std::string_view username, std::string_view password);
    private:
        void write_credentials_to_file(std::string_view user_credentials);
        std::string get_sha256_string(std::string_view str);
};
