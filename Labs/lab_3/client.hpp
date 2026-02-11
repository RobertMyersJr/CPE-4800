#pragma once

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

constexpr int buflen = 1024;
class Client {
    public:
        Client();
        ~Client();
        std::string send_message(std::string message);
        std::string read_message();
        std::string read_message_rsa();
        void send_message_rsa(std::string message);
    private:
        int sock_;
        int r_;
        struct sockaddr_in server_addr_;
        char message_out_[buflen];
        char message_in_[buflen];
};
