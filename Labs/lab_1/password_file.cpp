#include "password_file.hpp"
#include <array>
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <string>
#include <openssl/sha.h>
#include <string_view>
#include <sstream>
#include <vector>


constexpr std::string_view USER_CREDENTIAL_FILE = {"user_credentials_plain.txt"};
constexpr size_t SIZE_OF_CREDENTIALS_BUFFER{101};

constexpr size_t MIN_PASSWORD_SIZE = 8;

user_credential_t::user_credential_t(std::string_view username, std::string_view password) : username_{username}, password_{password} {}

void PasswordFile::write_plain_username_and_password(std::string_view username, std::string_view password) {
    std::string user_credentials = std::string(username) + " " + std::string(password);
    auto user_credentials_view = std::string_view(user_credentials);
    write_credentials_to_file(user_credentials_view);
}

void PasswordFile::write_encrypted_username_and_password(std::string_view username, std::string_view password) {
    std::string user_credentials = std::string(username) + " " + get_sha256_string(password);
    write_credentials_to_file(user_credentials);
}

void PasswordFile::write_credentials_to_file(std::string_view user_credentials) {
    std::ofstream test_file(USER_CREDENTIAL_FILE.data(), std::ios_base::app);
    test_file << user_credentials << "\n";
}

std::string PasswordFile::get_sha256_string(std::string_view str) {
    unsigned char hash_digest[SHA256_DIGEST_LENGTH];

    SHA256(reinterpret_cast<const unsigned char*>(str.data()), str.length(), hash_digest);
    std::stringstream ss;

    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash_digest[i]);
    }
    return std::string(ss.str());
}

bool PasswordFile::check_for_credentials(std::string_view user, std::string_view password) {
    std::array<char, SIZE_OF_CREDENTIALS_BUFFER> credentials;
    std::ifstream user_credential_file(USER_CREDENTIAL_FILE.data(), std::ios::in);
    
    std::string given_credentials = std::string(user) + " " + get_sha256_string(password);
    
    while(user_credential_file.getline(credentials.data(), SIZE_OF_CREDENTIALS_BUFFER - 1)) {
        if(given_credentials == std::string(credentials.data())) {
            return true;
        }
    }
    return false;

}

std::vector<user_credential_t> PasswordFile::get_current_users_and_passwords() {
    std::ifstream user_credential_file(USER_CREDENTIAL_FILE.data(), std::ios::in);

    std::array<char, SIZE_OF_CREDENTIALS_BUFFER> credentials;
    std::vector<user_credential_t> user_credentials;

    std::string current_user;
    std::string current_password;


    while(user_credential_file.getline(credentials.data(), SIZE_OF_CREDENTIALS_BUFFER - 1)) {
        std::stringstream credential_line(credentials.data());
        credential_line >> current_user;
        credential_line >> current_password;

        user_credentials.push_back(user_credential_t(
                    current_user,
                    current_password
                    ));
    }
    return user_credentials;
}

void PasswordFile::update_user_and_password(std::string_view username, std::string_view password) {
    std::fstream user_credential_file(USER_CREDENTIAL_FILE.data());

    std::string file_content = "";

    std::array<char, SIZE_OF_CREDENTIALS_BUFFER> credentials;
    std::vector<user_credential_t> user_credentials;

    std::string given_credentials = std::string(username) + " " + get_sha256_string(password);
    auto position = user_credential_file.tellg();
    while(user_credential_file.getline(credentials.data(), SIZE_OF_CREDENTIALS_BUFFER - 1)) {
        auto current_line = std::string(credentials.data());

        if(current_line.starts_with(username)) {
            std::string given_credentials = std::string(username) + " " + get_sha256_string(password);
            user_credential_file.seekp(position);
            user_credential_file << given_credentials;
        } 
        position = user_credential_file.tellg();
    }

}

bool PasswordFile::check_if_password_is_valid(std::string_view password) {
    auto password_length = password.length();

    bool has_uppercase = false;
    bool has_lowercase = false;
    bool has_symbol = false;

    if(password_length < MIN_PASSWORD_SIZE) {
        std::cout << "Password is not 8+\n";
        return false;
    }

    for(int i = 0; i < password_length; i++) {
        if(std::isupper(password[i])){
            has_uppercase = true;
        } else if (std::islower(password[i])) {
            has_lowercase = true;
        } else if(password[i] == '%') {
            has_symbol = true;
        }
    }
    
    if(!has_uppercase) {
        std::cout << "Password need to have at least one uppercase letter\n";
    }

    if(!has_lowercase) {
        std::cout << "Password need to have at least lowercase letter\n";
    }
    
    if(!has_symbol) {
        std::cout << "Password need to have the '%' symbol\n";
    }

    return (has_lowercase && has_uppercase && has_symbol);
}
