#pragma once

#include <vector>
struct account_t {
    account_t(int account_number, int account_balance);
    int account_number_;
    int account_balance_;
};

class AccountFile {
    public:
     account_t get_account(int account_number);    
     void write_account(account_t account);    
    private:
     std::vector<account_t> list_of_accounts_;
};
