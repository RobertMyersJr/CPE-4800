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
    while(1) {
        auto message_to_send = get_user_input("Client: ");

        client.send_message_rsa(message_to_send);
        server_message = client.read_message();
        if(server_message == "quit" || message_to_send == "quit") {
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
