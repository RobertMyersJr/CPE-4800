#pragma once
#include <string>

#include <sys/types.h> 
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>  

constexpr int buflen = 256;
class Server {
    public:
        Server();

        /**
         * @brief Starts the bank server and receiving messages. The program control is transferred to this function and will not return.
         */
        void start_bank_server();
    private:
        void send_message(std::string message);
        int get_account_number();
        int sock_;
        int client_sock_;
        std::string message_out_;
        char message_in_[buflen];
        struct sockaddr_in server_addr_;
        struct sockaddr_in client_addr_;
};
