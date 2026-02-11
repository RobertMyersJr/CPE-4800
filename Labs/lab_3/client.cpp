/**
******************************************************************************
* @file client.cpp
* @author Robert Myers Jr.
* @version V1.0
* @brief Implementation of the client class. See header for more information
******************************************************************************
*/
#include "client.hpp"
#include "evp.hpp"

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
constexpr int port = 8080;
constexpr const char * local_server_ip = "127.0.0.1";

Client::Client(std::string server_ip):
server_ip_(server_ip) {
  sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  server_addr_.sin_family = AF_INET;
  inet_pton(AF_INET, server_ip_.c_str(), &server_addr_.sin_addr);

  server_addr_.sin_port = htons(port);

  std::cout << "Connecting" << std::endl;
  r_ = connect(sock_, (struct sockaddr*)&server_addr_, sizeof(server_addr_));
  std::cout << "Fin Connecting" << std::endl;

  if(r_ == -1) {
      std::cout << "Failed to connect\nStopping Application...\n";
      exit(1);
  }
}

Client::~Client() {
    close(sock_);
}

std::string Client::send_message(std::string message) {
    write(sock_, message.c_str(), message.length());
    bzero(message_in_, max);
    read(sock_, message_in_, max);
    return std::string(message_in_);
}

std::string Client::read_message() {
    bzero(message_in_, max);
    read(sock_, message_in_, max);
    return std::string(message_in_);
}

std::string Client::read_message_rsa(EVP_PKEY *private_key) {
    bzero(message_in_, max);
    int len = read(sock_, message_in_, max);
    auto message_recevied = std::vector<char>(message_in_, 
            message_in_ + len);
    return decrypt_message(message_recevied, private_key);
}

void Client::send_message_rsa(std::string message, EVP_PKEY * public_key) {
    auto encrypted_message = encrypt_message(message, public_key);
    write(sock_, encrypted_message.data(), encrypted_message.size());
}
