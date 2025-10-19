#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <ctime>
#include "banktransaction.h"
using namespace std;

class User; 

class Bank {
protected:
    string ownerId;
    vector<BankTransaction> ledger;

public:
    Bank() {}
    Bank(const string& ownerId) : ownerId(ownerId) {}

    void recordBank(const string& type, double amount) {
        BankTransaction t;
        t.Id = "TX" + to_string(ledger.size() + 1);
        t.type = type;
        t.amount = amount;
        t.timestamp = time(nullptr);
        t.ownerId = ownerId;
        ledger.push_back(t);
    }

    bool deposit(User& account, double amount) {
        if (amount <= 0) {
            cout << "Invalid deposit amount.\n";
            return false;
        }
        account.balance += amount;
        recordBank("Deposit", amount);
        return true;
    }

    bool withdraw(User& account, double amount) {
        if (amount <= 0 || amount > account.balance) {
            cout << "Insufficient balance.\n";
            return false;
        }
        account.balance -= amount;
        recordBank("Withdraw", amount);
        return true;
    }

    bool transferTo(User& sender, User& recipient, double amount) {
        if (amount <= 0 || amount > sender.balance) {
            cout << "Transfer failed: invalid or insufficient balance.\n";
            return false;
        }
        sender.balance -= amount;
        recipient.balance += amount;
        recordBank("Transfer Out", amount);
        // Optional: record for recipient
        return true;
    }
};

class BankSystem {
public:
    unordered_map<string, Bank> accounts;

    bool login(const string& email) {
        return User::emails.count(email); // returns true if email exists
    }

    double getBalance(const string& email) {
        if (User::emails.count(email)) {
            return User::emails[email].getBalance();
        }
        return 0.0;
    }

    bool deposit(const string& email, double amount) {
        return accounts[email].deposit(User::emails[email], amount);
    }

    bool withdraw(const string& email, double amount) {
        return accounts[email].withdraw(User::emails[email], amount);
    }

    bool transfer(const string& senderEmail, const string& receiverEmail, double amount) {
        if (!accounts.count(senderEmail) || !accounts.count(receiverEmail)) return false;
        return accounts[senderEmail].transferTo(User::emails[senderEmail], User::emails[receiverEmail], amount);
    }
};
