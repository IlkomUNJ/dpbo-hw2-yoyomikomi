#pragma once
#include <string>
#include <ctime>
using namespace std;


struct BankTransaction {
    string Id;
    string type;       // Deposit, Withdraw, Transfer In, Transfer Out
    double amount;
    time_t timestamp;
    string ownerId;


};

