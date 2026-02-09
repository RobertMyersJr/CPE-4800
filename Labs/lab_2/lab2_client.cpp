#include "client.hpp"
#include "user_input.hpp"
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <iostream>

int main() {
    Client client;

    std::string server_message;
    //server_message = client.send_message(" ");
    std::cout << "Connection established\n";
    std::cout << client.read_message_aes();
    while(1) {
        server_message = client.send_message_aes(get_user_input(server_message));
        if(server_message.find("Exiting") != std::string::npos) {
            std::cout << server_message;
            exit(0);
        }
    }
}
