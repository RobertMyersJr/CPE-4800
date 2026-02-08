#include "server.hpp"

#include <sys/types.h> 
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>  
#include <arpa/inet.h>

constexpr int port = 1234;
Server::Server() {
  sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (mysock < 0)
  {
      std::cout << "socket call failed\n";
  }
  server_addr_.sin_family = AF_INET;
  //inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
  server_addr_.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr_.sin_port = htons(port);

  printf("bind the socket");
  r = bind(sock_, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (r<0)
  {
    printf("bind failed\n");
  }
  printf("server is listening  ...\n");
  listen(mysock, 5); // queue length of 5
  printf("server init done\n");

}
