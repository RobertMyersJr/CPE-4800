#include "server.hpp"
#include "evp.hpp"
#include "user_input.hpp"
#include "rsa_key_tools.hpp"
#include <openssl/evp.h>

#include <iostream>
#include <iterator>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>  
#include <sys/unistd.h>
#include <vector>

constexpr int max = 256;
constexpr int port = 1234;

Server::Server() {
    sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_ < 0)
    {
        std::cout << "socket call failed\n";
    }
    int yes = 1;
    setsockopt(sock_,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));

    server_addr_.sin_family = AF_INET;
    //inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    server_addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr_.sin_port = htons(port);

    printf("bind the socket: ");
    int r = bind(sock_, (struct sockaddr*)&server_addr_, sizeof(server_addr_));
    if (r<0)
    {
        printf("bind failed\n");
    }
    printf("server is listening  ...\n");
    listen(sock_, 5); // queue length of 5
    printf("server init done\n");
}

std::string Server::read_encrpyted_message(EVP_PKEY * private_key) {
    bzero(message_in_, max);
    auto len = recvfrom(client_sock_, message_in_, max, 0, NULL, NULL);
    std::cout << "len" << len << "\n";
    auto encrpyted_message = std::vector<char>(message_in_, 
            message_in_ + len);
    return decrypt_message(encrpyted_message, private_key);
}

std::string Server::read_message() {
    bzero(message_in_, max);
    auto len = recvfrom(client_sock_, message_in_, max, 0, NULL, NULL);
    return std::string(message_in_, len);
}

void Server::send_encrpyted_message(std::string message, EVP_PKEY * public_key){
    auto encrpyted_message = encrypt_message(message, public_key);
    sendto(client_sock_, encrpyted_message.data(), encrpyted_message.size(), 0, (struct sockaddr *)&client_addr_, sizeof(client_addr_));
}

void Server::send_message(std::string message){
    sendto(client_sock_, message.data(), message.size(), 0, (struct sockaddr *)&client_addr_, sizeof(client_addr_));
}

void Server::start_server() {
    EVP_PKEY * private_key;
    EVP_PKEY * other_user_public_key;
    if(!open_private_key(&private_key, "server.pem")) {
        std::cout << "Failed to get private key\n";
        std::cout << "Ending program...\n";
        exit(0);
    }
    if(!open_public_key(&other_user_public_key, "client.pem.pub")) {
        std::cout << "Failed to get public key\n";
        std::cout << "Ending program...\n";
        exit(0);
    }
    //auto * other_user_public_key = prompt_for_public_key();
    while (1) //Try to accept a client request
    {
        printf("server: accepting new connection ...\n");

        socklen_t len = sizeof(client_addr_);
        client_sock_ = accept(sock_, (struct sockaddr *)&client_addr_, &len);
        if (client_sock_ < 0)
        {
            printf("server accept error \n");
        }
        printf("server accepted a client connection from \n");
        printf("Client: IP= %s port=%d \n", inet_ntoa(client_addr_.sin_addr), ntohs(client_addr_.sin_port));

        while(1)
        {
            auto client_message = read_encrpyted_message(private_key);
            if (client_message == "")
            {
                printf("server client died, server loops\n");
                close(client_sock_);
                break;
            } else if(client_message == "quit") {
                std::cout << "Closing connection...\n";
                close(sock_);
                std::cout << "Stopping server...\n";
                exit(0);
            }

            std::cout << "Client: " << client_message << "\n";
            auto message_to_be_sent = get_user_input("Server: ");

            send_encrpyted_message(message_to_be_sent, other_user_public_key);
            if(message_to_be_sent == "quit") {
                std::cout << "Closing connection...\n";
                close(sock_);
                std::cout << "Stopping server...\n";
                exit(0);
            }
        }
    }

}
