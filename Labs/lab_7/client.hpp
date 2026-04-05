#pragma once
/**
******************************************************************************
* @file client.hpp
* @author Robert Myers Jr.
* @version V1.0
* @brief A client class made to make communicating with the server easier
******************************************************************************
*/

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

constexpr int buflen = 1024;
class Client {
    public:
        Client();
        /**
         * @brief Sends and receives a message
         *
         * @param message The message received
         */
        std::string send_message(std::string message);

        /**
         * @brief Reads a message
         */
        std::string read_message();

        /**
         * @brief Reads a AES encypted message. Will return the decrypted message
         */
        std::string read_message_aes();

        /**
         * @brief Sends and receives a message with AES encryption. Will returned a decrypted string
         *
         * @param message The message being sent
         */
        std::string send_message_aes(std::string message);
    private:
        int sock_;
        int r_;
        struct sockaddr_in server_addr_;
        char message_out_[buflen];
        char message_in_[buflen];
};
