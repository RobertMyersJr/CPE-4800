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
#include <openssl/evp.h>
#include <sys/socket.h>
#include <netinet/in.h>

constexpr int buflen = 1024;
class Client {
    public:
        Client(std::string ip_address);
        ~Client();

        /**
         * @brief Sends a message
         *
         * @param message The message being sent
         */
        std::string send_message(std::string message);

        /**
         * @brief Reads a message
         */
        std::string read_message();


        /**
         * @brief Reads a RSA encrypted message using private key
         * @param private_key The private key used for decryption
         */
        std::string read_message_rsa(EVP_PKEY * private_key);

        /**
         * @brief Sends a message using RSA encryption using a public key
         *
         * @param message The message being sent
         * @param public_key The public key used for encryption
         */
        void send_message_rsa(std::string message, EVP_PKEY * public_key);
    private:
        int sock_;
        int r_;
        struct sockaddr_in server_addr_;
        char message_out_[buflen];
        char message_in_[buflen];
        std::string server_ip_;
};
