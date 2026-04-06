/**
 ******************************************************************************
 * @file bank_service.hpp
 * @author Robert Myers Jr.
 * @version V1.0
 * @brief Implementation of the BankService class. See header for more information.
 ******************************************************************************
 */
#include "bank_service.hpp"
#include "client.hpp"
#include "srp_utils.hpp"
#include "tools/evp.hpp"
#include "user_input.hpp"
#include <bit>
#include <cctype>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <strings.h>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <thread>
#include <vector>
#include <format>
#include <regex>

namespace {
    constexpr int b = 7;
    constexpr int max = 256;
    constexpr std::string_view TRANSCRIPT_FILE = "transcript_log.txt";
    bool convert_string(std::string_view floating_point_number, double & output) {
        for(int i = 0; i < floating_point_number.length(); i++) {
            if(!std::isdigit(floating_point_number[i]) && floating_point_number[i] != '.') {
                return false;
            }
        }
        try {
            output = std::stod(floating_point_number.data());
            return true;
        } catch (const std::invalid_argument& e) {
            return false;
        } catch (const std::out_of_range& e) {
            return false;
        }
    }
    double truncate(double floating_point_number) {
        return std::trunc(floating_point_number*100.00)/100.0;
    }
}

void BankService::send_message(std::string message, std::optional<uint64_t> key_mask){
    std::cout << "Sending over " <<message << "\n";
    auto encrypted_message = quick_encrypt(message, key_mask);
    sendto(client_socket_, encrypted_message.data(), encrypted_message.size(), 0, (struct sockaddr *)&client_addr, sizeof(*client_addr));
}

BankService::BankService(
        int client_socket, 
        int account_number,
        std::string salt, 
        std::string verifier) {
    client_socket_ = client_socket;
    account_number_ = account_number;
    write_srp_information(std::to_string(account_number), salt, verifier);
    std::cout << "Account created!" << std::endl;
}

BankService::BankService(int client_socket, 
        int account_number,
        int A): 
    client_socket_(client_socket), 
    account_number_(account_number),
    balance(1000.0) {
        // Account validation
        std::ifstream transcript_file(TRANSCRIPT_FILE.data());

        std::stringstream buffer;
        buffer << transcript_file.rdbuf();
        auto transcation_log = buffer.str();

        auto line_to_find = std::format("- Account: {}", account_number_);

        size_t found_pos = transcation_log.rfind(line_to_find);

        // File is empty let's not try to read
        if (found_pos != std::string::npos) {
            size_t lineStart = transcation_log.rfind('\n', found_pos);
            
            // Move past the newline character
            lineStart++; 

            size_t lineEnd = transcation_log.find('\n', found_pos);

            std::string lastLine = transcation_log.substr(lineStart, lineEnd - lineStart);

            size_t last_space_pos = lastLine.rfind(' ');

            balance = std::stod(lastLine.substr(last_space_pos + 2));
        } 
        auto [I, S, V] = get_srp_information();


        auto B = SRP::generate_B(SRP::g, b, SRP::k, std::stoull(V), SRP::n);
        auto message_for_client = std::format("{}:{}", S, B);
        std::cout << "Sending this to client" << message_for_client << "\n";
        send_message(message_for_client);

        auto U = SRP::hash_to_u64(std::format("{}:{}", A, B));

        std::cout << "U: " << U << "\n";

        auto key = SRP::generate_key_server(A, std::stoull(V), U, b, SRP::n);
        std::cout << "KEY:"<<key<<"\n";
        auto message = read_message(key);
        std::cout << "Message received: " << message << std::endl;
        if(message == "Hi, I am the client") {
            std::cout << "Client authenticated\n";
            send_message("Hi, I am the server", std::make_optional(key));
        } else {
            std::cout << "Client authentication failed!\n";
        }
    }

bool BankService::run_bank_service(std::string input) {
    std::string message_to_send;
    if(input.starts_with("withdraw")) {
        message_to_send = withdraw_command(input);
    } else if(input.starts_with("deposit")) {
        message_to_send = deposit_command(input);
    } else if(input == "balance") {
        message_to_send = balance_command();
    } else if(input == "exit") {
        exit_command();
        return false;
    } else {
        message_to_send = "Error: invalid command\n";
    }
    message_to_send += "Enter Command (withdraw <amount>, deposit <amount>, balance, exit): ";
    send_message(message_to_send);
    return true;
}

void BankService::send_prompt()  {
    std::string message_to_send = "Enter Command (withdraw <amount>, deposit <amount>, balance, exit): ";
    send_message(message_to_send);
}

std::string BankService::deposit_command(std::string withdraw_command) {
    std::stringstream stream(withdraw_command);

    std::vector<std::string> words;
    std::string word;

    while(stream >> word) {
        words.push_back(word);
    }
    if(words.size() >= 3 || words.size() <= 1) {
        return std::format("Error: expected 1 numeric argument for the deposit command but got {} arguments\n", words.size() - 1);
    }

    double amount_to_deposit = 0;

    // Verify the argument is valid
    if(!convert_string(words.at(1), amount_to_deposit) && amount_to_deposit >= 0.0) {
        return "Error: Amount specified is invalid\n";
    }

    auto truncated_amount_to_deposit = truncate(amount_to_deposit);

    std::string message_to_send = "";
    if(truncated_amount_to_deposit != amount_to_deposit) {
        message_to_send = std::format("Truncating ${:.3f} to ${:.2f}\n", amount_to_deposit, truncated_amount_to_deposit);
    }

    balance += truncated_amount_to_deposit;
    message_to_send += std::format("Depositing ${:.2f}. Balance: ${:.2f}\n", truncated_amount_to_deposit, balance);

    write_deposit_to_transcript(truncated_amount_to_deposit);

    return message_to_send;
}

std::string BankService::withdraw_command(std::string withdraw_command) {
    std::stringstream stream(withdraw_command);

    std::vector<std::string> words;
    std::string word;

    while(stream >> word) {
        words.push_back(word);
    }
    if(words.size() >= 3 || words.size() <= 1) {
        return std::format("Error: expected 1 numeric argument for the withdraw command but got {} arguments\n", words.size() - 1);
    }

    double amount_to_withdraw = 0;

    // Verify the amount is a floating_point_number and is positive
    if(!convert_string(words.at(1), amount_to_withdraw) && amount_to_withdraw >= 0.0) {
        return "Error: Amount specified is invalid\n";
    }

    auto truncated_amount_to_withdraw = truncate(amount_to_withdraw);

    // Don't take out more money than whats in the account
    if(truncated_amount_to_withdraw > balance) {
        return std::format("Error: can't withdraw over the current balance of {:.2f}\n", balance);
    }

    std::string message_to_send = "";

    if(truncated_amount_to_withdraw != amount_to_withdraw) {
        message_to_send = std::format("Truncating ${:.3f} to ${:.2f}\n", amount_to_withdraw, truncated_amount_to_withdraw);
    }

    balance -= truncated_amount_to_withdraw;

    write_withdraw_to_transcript(truncated_amount_to_withdraw);
    return message_to_send + std::format("Withdrawing ${:.2f}. Balance: ${:.2f}\n", truncated_amount_to_withdraw, balance);
}

std::string BankService::balance_command() {
    return std::format("Current balance ${:.2f}\n", balance);
}

void BankService::exit_command() {
    std::string message_to_send = "Exiting...\n";
    send_message(message_to_send);
    close(client_socket_);
    exit(0);
}

std::string get_time() {
    auto now = std::chrono::system_clock::now();

    auto local_now = std::chrono::zoned_time{std::chrono::current_zone(), now};

    auto seconds_only = std::chrono::floor<std::chrono::seconds>(local_now.get_local_time());

    return std::format("{:%Y-%m-%d %H:%M:%S}\n", seconds_only);
}
void BankService::write_withdraw_to_transcript(double withdraw_amount) {
    std::ofstream transcript_file(TRANSCRIPT_FILE.data(), std::ios_base::app);

    auto formatted_time = get_time();

    std::string formatted_deposit = std::format("- Account: {} - Withdraw of ${:.2f} successful. New balance : ${:.2f}\n", account_number_, withdraw_amount, balance);

    transcript_file << formatted_time;
    transcript_file << formatted_deposit;
}
void BankService::write_deposit_to_transcript(double deposit_amount) {
    std::ofstream transcript_file(TRANSCRIPT_FILE.data(), std::ios_base::app);

    auto formatted_time = get_time();

    std::string formatted_deposit = std::format("- Account: {} - Deposit of ${:.2f} successful. New balance : ${:.2f}\n", account_number_, deposit_amount, balance);

    transcript_file << formatted_time;
    transcript_file << formatted_deposit;

}

std::string BankService::read_message(std::optional<uint64_t> key_mask) {
    bzero(message_in_, max);
    int len = recvfrom(client_socket_, message_in_, max, 0, NULL, NULL);
    if (len == 0)
    {
        return "";
    }
    auto message_recevied = std::vector<unsigned char>(message_in_, 
            message_in_ + len);
    return std::string(quick_decrypt(message_recevied, key_mask));
}

void BankService::write_srp_information(std::string_view identify, std::string_view salt, std::string_view verifier) {
    std::ofstream srp_file("srp_information.txt", std::ios_base::app);

    auto formatted_time = get_time();

    std::string formatted_srp_information = std::format("- Account: {} - identify: {} salt: {} verifier: {}\n", account_number_, identify, salt, verifier);
    srp_file << formatted_srp_information;
}

std::tuple<std::string, std::string, std::string> BankService::get_srp_information() {
    std::ifstream srp_file("srp_information.txt");

    std::stringstream buffer;
    buffer << srp_file.rdbuf();
    auto srp_log = buffer.str();

    auto line_to_find = std::format("- Account: {}", account_number_);

    size_t found_pos = srp_log.rfind(line_to_find);

    // File is empty let's not try to read
    if (found_pos == std::string::npos) {
        return std::make_tuple("", "", "");
    } else {
        // Authorization step
    }

    size_t lineStart = srp_log.rfind('\n', found_pos);
    
    // Move past the newline character
    lineStart++; 

    size_t lineEnd = srp_log.find('\n', found_pos);

    std::string lastLine = srp_log.substr(lineStart, lineEnd - lineStart);

    std::string line = "- Account: 123 - identify: 123 salt: 5 verifier: 2680355691976787848";
    std::regex reg("identify: (\\S+) salt: (\\d+) verifier: (\\d+)");
    std::smatch matches;

    if (std::regex_search(line, matches, reg)) {
        std::string identify = matches[1].str();
        uint64_t salt = std::stoull(matches[2].str());
        uint64_t verifier = std::stoull(matches[3].str());
        return std::make_tuple(
            identify, 
            std::to_string(salt), 
            std::to_string(verifier)
        );
    }


    return std::make_tuple("", "", "");
}
