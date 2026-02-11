#pragma once
/**
 ******************************************************************************
 * @file Server.hpp
 * @author Robert Myers Jr.
 * @version V1.0
 * @brief A server class made to make communicating with clients easier.
 *******************************************************************************
 */
#include <string>

#include <string_view>
#include <sys/types.h> 
#include <sys/socket.h>
#include <openssl/evp.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>  

constexpr int buflen = 256;
class Server {
    public:
        Server();

        /**
         * @brief Starts the server and starts receiving messages. The program control is transferred to this function and will not return.
         */
        void start_server();
    private:
        /**
         * @brief Sends a message to a connected client
         */
        void send_message(std::string message);

        /**
         * @brief Sends a message to a connected client using RSA encryption
         */
        void send_encrpyted_message(std::string message, EVP_PKEY * public_key);

        /**
         * @brief Sends a message to a connected client
         */
        std::string read_message();

        /**
         * @brief Reads a message from a connected client using RSA encryption.
         */
        std::string read_encrpyted_message(EVP_PKEY * private_key);

        /**
         * @brief Gets the account number
         */
        int get_account_number();

        /**
         * @brief Writes a server message to the log
         */
        void server_message_to_log(std::string_view message_to_write);

        /**
         * @brief Writes a client message to the log
         */
        void client_message_to_log(std::string_view message_to_write);
        int sock_;
        int client_sock_;

        std::string message_out_;
        char message_in_[buflen];

        struct sockaddr_in server_addr_;
        struct sockaddr_in client_addr_;

};
