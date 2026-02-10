#pragma once
/**
******************************************************************************
* @file bank_service.hpp
* @author Robert Myers Jr.
* @version V1.0
* @brief BankService that handles deposits and withdraws for a single account.
******************************************************************************
*/

#include <netinet/in.h>
#include <string>
class BankService {
    public:
        BankService(int client_socket, int account_number);
        void send_prompt();
        bool run_bank_service(std::string command);
    private:
        int client_socket_;
        int account_number_;
        double balance;
        struct sockaddr_in *client_addr;

        /**
         * @brief Sends a message to the client
         */
        void send_message(std::string message);

        /**
         * @brief Writes successful deposit actions to the transaction log
         */
        void write_deposit_to_transcript(double deposit_amount);
        /**
         * @brief Writes successful withdraw actions to the transaction log
         */
        void write_withdraw_to_transcript(double withdraw_amount);

        /**
         * @brief Parse and executes a withdraw command. Returns a message indicating what happened
         *
         * @param withdraw_command The command itself
         */
        std::string withdraw_command(std::string withdraw_command);

        /**
         * @brief Parse and executes a deposit command. Returns a message indicating what happened
         *
         * @param deposit_command The command itself
         */
        std::string deposit_command(std::string deposit_command);

        /**
         * @brief Executes the balance command
         */
        std::string balance_command();

        /**
         * @brief Executes the exit command
         */
        void exit_command();
};
