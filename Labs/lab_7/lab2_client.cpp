#include "client.hpp"
#include "user_input.hpp"
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <srp_utils.hpp>

namespace {
    constexpr int a = 5;
    constexpr int v = 7;
    constexpr int s = 7;
}
void login_procedure(Client& client) {
    auto A = SRP::generate_A(SRP::g, a, SRP::n);

    std::cout << "Input Account Number: ";
    std::string identify;
    std::getline(std::cin, identify);

    std::cout << "Input Password: ";
    std::string password;
    std::getline(std::cin, password);

    auto challenge = client.send_message_aes(
            std::format("{}:{}", identify, A)
    );
    if (challenge == "ERROR: Account not found. Closing Connection...") {
        std::cout << challenge << std::endl;
        std::cout << "Exiting..."<< std::endl;
        exit(0);
    }

    std::stringstream ss(challenge);

    std::string salt;
    std::string B;

    std::getline(ss, salt, ':');
    std::getline(ss, B, ':');

    auto u = SRP::hash_to_u64(std::format("{}:{}",A,B));
    std::cout << "salt: " << salt << "\n";
    // std::cout << "U: " << u << "\n";

    auto x = SRP::create_x(salt, identify, password);
    std::cout << "X  loging " << x << std::endl;

    auto key_A = SRP::generate_key_client(x, std::stoull(B), SRP::k, SRP::g, a, u, SRP::n);

    std::cout << "KEY:"<<key_A<<"\n";

    auto server_message = client.send_message_aes("Hi, I am the client", std::make_optional(key_A));
    // std::cout << "Server message" << server_message << "\n";
    if(server_message == "Hi, I am the server") {
        std::cout << "Server authenticated\n";
    } else {
        std::cout << "Server authentication failed!\n";
        std::cout << "Exiting...\n";
        exit(0);
    }
}
void register_procedure(Client& client) {
    std::cout << "Starting registration process.\n";
    std::cout << "Input Account Number: ";
    std::string identify;
    std::getline(std::cin, identify);

    std::cout << "Input Password: ";
    std::string password;
    std::getline(std::cin, password);
    std::string salt = std::to_string(s);

    auto x = SRP::create_x(
            std::string_view(salt), 
            std::string_view(identify), 
            std::string_view(password)
            );
    std::cout << "X  registartion" << x << std::endl;
    auto verifier = SRP::create_verifier(x);
    std::cout << (client.send_message_aes(
            std::format("{}:{}:{}", identify, salt, verifier)
    )) << "\n";
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
            std::cout << "Exiting program...\n";
            exit(0);
        } else if(server_message == "login") {
            login_procedure(client);
            server_message = client.read_message_aes();
        } else if(server_message.find("Exiting") != std::string::npos) {
            std::cout << server_message;
            exit(0);
        } else if(server_message == "Received Garbage Closing connection") {

        } else {
            std::cout << server_message;
        }
    }
}
