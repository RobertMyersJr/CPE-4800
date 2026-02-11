#include "client.hpp"
#include "rsa_key_tools.hpp"
#include "user_input.hpp"
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <openssl/err.h>
#include <openssl/evp.h>

int main() {
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    EVP_PKEY * private_key;

    auto ip = get_user_input("Server's ip address: ");

    if(!open_private_key(&private_key, "client.pem")) {
        std::cout << "Failed to private key\n";
        std::cout << "Ending program...\n";
        exit(1);
    }
    auto * other_user_public_key = prompt_for_public_key();

    Client client(ip);

    std::string server_message;
    std::cout << "Connection established\n";
    while(1) {
        std::string message_to_be_sent = get_user_input("Client: ");
        while(message_to_be_sent == "") {
            std::cout << "ERROR: message is empty. Try again.\n";
            message_to_be_sent = get_user_input("Client: ");
        }

        client.send_message_rsa(message_to_be_sent, other_user_public_key);
        if(message_to_be_sent == "quit") {
            std::cout << "Closing connection...\n";
            std::cout << "Stopping client...\n";
            exit(0);
        }
        server_message = client.read_message_rsa(private_key);
        if(server_message == "quit") {
            std::cout << "Closing connection...\n";
            std::cout << "Stopping client...\n";
            exit(0);
        }
        if(server_message == "") {
            std::cout << "Server connection died...\n";
            std::cout << "Stopping client...\n";
            exit(0);
        }
        std::cout << "Server: " << server_message << "\n";
    }
}
