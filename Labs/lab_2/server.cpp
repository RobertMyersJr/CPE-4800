#include "server.hpp"
#include "bank_service.hpp"

#include <iostream>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>  
#include <sys/unistd.h>

constexpr int max = 256;
constexpr int port = 1234;

Server::Server() {
    sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_ < 0)
    {
        std::cout << "socket call failed\n";
    }
    int yes;
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

int Server::get_account_number() {
    std::string message_to_send("Enter account number: ");
    sendto(client_sock_, message_to_send.c_str(), message_to_send.length(), 0, (struct sockaddr *)&client_addr_, sizeof(client_addr_));
    bzero(message_in_, max);
    int n = recvfrom(client_sock_, message_in_, max, 0, NULL, NULL);
    std::string account_number_given(message_in_);
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

        BankService bank_service(client_sock_, 123);
        int n = recvfrom(client_sock_, message_in_, max, 0, NULL, NULL);
        if(get_account_number() == -1) {
            std::string message_to_send{"Account number is invalid. Exiting..."};
            sendto(client_sock_, message_to_send.c_str(), message_to_send.length(), 0, (struct sockaddr *)&client_addr_, sizeof(client_addr_));
            close(client_sock_);
            continue;
        }
        bank_service.send_prompt();
        while(1)
        {
            int n = recvfrom(client_sock_, message_in_, max, 0, NULL, NULL);
            if (n == 0)
            {
                printf("server client died, server loops\n");
                close(client_sock_);
                break;
            }
            // show the string
            printf("server read %s\n", message_in_);
            // write back to client
            //n = write(client_sock_, line, max);
            if(!bank_service.run_bank_service(std::string(message_in_))) {
                break;
            }
        }
    }

}
