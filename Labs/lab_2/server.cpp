#include "server.hpp"
#include "bank_service.hpp"
#include "tools/evp.hpp"

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

std::string Server::read_message() {
    bzero(message_in_, max);
    int len = recvfrom(client_sock_, message_in_, max, 0, NULL, NULL);
    if (len == 0)
    {
        return "";
    }
    auto message_recevied = std::vector<unsigned char>(message_in_, 
            message_in_ + len);
    return std::string(quick_decrypt(message_recevied));
}

std::string Server::send_message(std::string message){
    auto encrypted_message = quick_encrypt(message);
    sendto(client_sock_, encrypted_message.data(), encrypted_message.size(), 0, (struct sockaddr *)&client_addr_, sizeof(client_addr_));

    bzero(message_in_, max);
    int n = recvfrom(client_sock_, message_in_, max, 0, NULL, NULL);

    auto message_received = std::vector<unsigned char>(message_in_, message_in_ + n);
    
    auto received = quick_decrypt(message_received);

    return received;
}

int Server::get_account_number() {
    std::string message_to_send("Enter account number: ");

    std::string account_number_given = send_message(message_to_send);

    for(int i = 0; i < account_number_given.length(); i++) {
        char letter = account_number_given.c_str()[i];
        if(!isdigit(letter)) {
            return -1;
        }
    }
    auto account_number = std::stoi(account_number_given);
    if(account_number < 0) {
        return -1;
    }
    return account_number;
}

void Server::start_bank_server() {
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

        auto account_number = get_account_number();
        if(account_number == -1) {
            std::cout << "account number input failure\n";
            std::string message_to_send{"Account number is invalid. Exiting..."};
            send_message(message_to_send);
            close(client_sock_);
            continue;
        }
        BankService bank_service(client_sock_, account_number);
        bank_service.send_prompt();
        while(1)
        {
            auto received_message = read_message();
            if (received_message == "")
            {
                printf("server client died, server loops\n");
                close(client_sock_);
                break;
            }
            if(!bank_service.run_bank_service(received_message)) {
                break;
            }
        }
    }

}
