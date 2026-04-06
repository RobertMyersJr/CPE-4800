#pragma once
/**
******************************************************************************
* @file Server.hpp
* @author Robert Myers Jr.
* @version V1.0
* @brief A server class made to make communicating with clients easier. Also runs the bank service.
******************************************************************************
*/
#include <string>

#include <sys/types.h> 
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>  

enum class UserPath { Login, Regristration, Error };
constexpr int buflen = 256;
class Server {
    public:
        Server();

        /**
         * @brief Starts the bank server and receiving messages. The program control is transferred to this function and will not return.
         */
        void start_bank_server();
    private:
        /**
         * @brief Sends a message to a connected client
         */
        std::string send_message(std::string message);

        /**
         * @brief Reads a message from a connected client
         */
        std::string read_message();

        /**
         * @brief Gets the account number
         */
        int get_account_number();

        /**
         * @brief Gets the account number
         */
        int get_account_number(std::string account_number_given);

        /**
         * Gets the prompt to login in or registration
         */
        UserPath get_prompt();

        int sock_;
        int client_sock_;

        std::string message_out_;
        char message_in_[buflen];

        struct sockaddr_in server_addr_;
        struct sockaddr_in client_addr_;
};
