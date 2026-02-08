#pragma once

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

constexpr int buflen = 256;
class Client {
    public:
        Client();
        void sendMessage(std::string message);
    private:
        int sock_;
        int r_;
        struct sockaddr_in server_addr_;
        char line_[buflen];
};
