#include "client.hpp"

#include <sys/types.h> 
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>  

#include <string.h>
#include <strings.h>

#include <unistd.h>

constexpr int max = 256;
constexpr int port = 1234;
constexpr const char * server_ip = "127.0.0.1";

Client::Client() {
  sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  server_addr_.sin_family = AF_INET;
  inet_pton(AF_INET, server_ip, &server_addr_.sin_addr);

  server_addr_.sin_port = htons(port);

  r_ = connect(sock_, (struct sockaddr*)&server_addr_, sizeof(server_addr_));
}

void Client::sendMessage(std::string message) {
    bzero(line_, max);
    fgets(line_, max, stdin);
    line_[strlen(line_)-1] = 0; //remove the \n
    write(sock_, line_, max);
}
