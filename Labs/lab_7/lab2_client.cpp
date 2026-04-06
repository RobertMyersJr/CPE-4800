#include "client.hpp"
#include "user_input.hpp"
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <iostream>
#include <string_view>
#include <srp_utils.hpp>

namespace {
    constexpr int a = 5;
    constexpr int v = 7;
    constexpr int s = 7;
}
void register_procedure(Client& client) {
    std::cout << "Account Number not found. Starting registration process.\n";
    std::cout << "Input Account Number: ";
    std::string identify;
    std::getline(std::cin, identify);

    std::cout << "Input Password: ";
    std::string password;
    std::getline(std::cin, password);
    std::string salt = "Salt";

    auto verifier = SRP::create_verifier(
            std::string_view(salt), 
            std::string_view(identify), 
            std::string_view(password)
            );
    client.send_message_aes(
            std::format("{}:{}:{}", identify, salt, verifier)
    );
}

int main() {
    Client client;

    std::string server_message;
    //server_message = client.send_message(" ");
    std::cout << "Connection established\n";
    std::cout << client.read_message_aes();
    while(1) {
        server_message = client.send_message_aes(get_user_input(server_message));
        if(server_message == "register") {
            register_procedure(client);
        }
        if(server_message.find("Exiting") != std::string::npos) {
            std::cout << server_message;
            exit(0);
        } else {
            std::cout << server_message;
        }
    }
}
