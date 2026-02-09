#include "client.hpp"
#include "tools/evp.hpp"

#include <iostream>
#include <openssl/evp.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>  
#include <openssl/rand.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

constexpr int max = 1024;
constexpr int port = 1234;
constexpr const char * server_ip = "127.0.0.1";

Client::Client() {
  sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  server_addr_.sin_family = AF_INET;
  inet_pton(AF_INET, server_ip, &server_addr_.sin_addr);

  server_addr_.sin_port = htons(port);

  r_ = connect(sock_, (struct sockaddr*)&server_addr_, sizeof(server_addr_));
}

std::string Client::send_message(std::string message) {
    write(sock_, message.c_str(), max);

    bzero(message_in_, max);
    read(sock_, message_in_, max);
    return std::string(message_in_);
}

std::string Client::send_message_aes(std::string message) {
    auto encrypted_message = quick_encrypt(message);
    write(sock_, encrypted_message.data(), max);

    int n = read(sock_, message_in_, max);

    return quick_decrypt(std::string(message_in_));
}
