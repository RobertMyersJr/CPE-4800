#pragma once

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

        void write_deposit_to_transcript(double deposit_amount);
        void write_withdraw_to_transcript(double withdraw_amount);
        std::string withdraw_command(std::string withdraw_command);
        std::string deposit_command(std::string deposit_command);
        std::string balance_command();
        void exit_command();
};
