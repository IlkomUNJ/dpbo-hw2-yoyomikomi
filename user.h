#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "transaction.h"
using namespace std;

class Bank; // forward declaration

class User {
protected:
    string email;
    string password;
    string bankAccount;
    string storeUsername;
    vector<Transaction> buyerOrders;
    double balance = 0;

public:
    User() {}
    User(const string& first, const string& second, bool isBank = false) {
        if (isBank) {
            email = first;
            bankAccount = second;
        } else {
            storeUsername = first;
            password = second;
        }
    }

    static unordered_map<string, User> storeAccounts;
    static unordered_map<string, User> emails;

    string getAccount() const { return bankAccount; }
    string getUsername() const { return storeUsername; }
    string getEmail() const { return email; }
    string getPassword() const { return password; }
    double getBalance() const { return balance; }

    virtual string getRole() const { return "Buyer"; }

    bool checkExistingEmail(const string& email) const {
        return emails.find(email) != emails.end();
    }

    bool CreateBankAccount() {
        char choice;
        cout << "Enter email: ";
        cin >> email;
        while (checkExistingEmail(email)) {
            cout << "Email exists. Use another? (y/n): ";
            cin >> choice;
            if (choice == 'y') cin >> email;
            else return false;
        }
        cout << "Enter name: ";
        cin >> bankAccount;
        emails[email] = User(email, bankAccount, true);
        cout << "Bank account created successfully!\n";
        return true;
    }

    bool checkExistingAccounts(const string& username) const {
        return storeAccounts.find(username) != storeAccounts.end();
    }

    bool UserRegister() {
        char choice;
        cout << "Enter username: ";
        cin >> storeUsername;
        while (checkExistingAccounts(storeUsername)) {
            cout << "Username exists. Use another? (y/n): ";
            cin >> choice;
            if (choice == 'y') cin >> storeUsername;
            else return false;
        }
        cout << "Enter password: ";
        cin >> password;
        storeAccounts[storeUsername] = User(storeUsername, password);
        cout << "Registration successful!\n";
        return true;
    }

    void viewOrders() const {
        cout << "\n--- Your Orders ---\n";
        for (const auto& t : buyerOrders) {
            cout << "Item: " << t.itemName
                 << ", Amount: " << t.amount
                 << ", Status: " << t.status
                 << ", Date: " << ctime(&t.timestamp);
        }
    }

    void filterOrdersByStatus(const string& status) const {
        cout << "\n--- Orders with status: " << status << " ---\n";
        for (const auto& t : buyerOrders) {
            if (t.status == status) {
                cout << "Item: " << t.itemName
                     << ", Amount: " << t.amount
                     << ", Date: " << ctime(&t.timestamp);
            }
        }
    }

    double spendingLastKDays(int k) const {
        time_t now = time(nullptr);
        double total = 0;
        for (const auto& t : buyerOrders) {
            double diff = difftime(now, t.timestamp);
            int days = diff / (60 * 60 * 24);
            if (days < k && t.status == "Paid") total += t.amount;
        }
        return total;
    }

    friend class Bank;
};

unordered_map<string, User> User::storeAccounts;
unordered_map<string, User> User::emails;
