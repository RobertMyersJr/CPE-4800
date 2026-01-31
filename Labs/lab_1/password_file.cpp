#include "password_file.hpp"
#include <array>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <ios>
#include <string>
#include <openssl/sha.h>
#include <string_view>
#include <sstream>
#include <vector>


constexpr std::string_view USER_CREDENTIAL_FILE = {"user_credentials_plain.txt"};

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
    constexpr size_t SIZE_OF_CREDENTIALS_BUFFER{101};
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

std::vector<std::string> get_current_users() {
    std::ifstream user_credential_file(USER_CREDENTIAL_FILE.data(), std::ios::in);
    
    std::string given_credentials = std::string(user) + " " + get_sha256_string(password);
    
    while(user_credential_file.getline(credentials.data(), SIZE_OF_CREDENTIALS_BUFFER - 1)) {
        if(given_credentials == std::string(credentials.data())) {
            return true;
        }
    }
    return false;

}
