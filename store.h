#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "transaction.h"
#include <algorithm>
using namespace std;


class Store {
protected:
    vector<Transaction> storeLedger;

public:
    bool purchaseItem(User& buyer, Seller& seller, const string& itemName, int quantity) {
        double price = seller.getItemPrice(itemName);
        if (price <= 0) {
            cout << "Item not found or invalid price.\n";
            return false;
        }
        double totalCost = price * quantity;

        if(buyer.getBalance() < totalCost) {
            cout << "Insufficient balance.\n";
            return false;
        }

        Transaction t;
        t.id = "TR" + to_string(storeLedger.size() + 1);
        t.buyerId = buyer.getUsername();
        t.sellerId = seller.getUsername();
        t.itemName = itemName;
        t.amount = totalCost;
        t.status = "Paid";
        t.timestamp = time(0);
        storeLedger.push_back(t);

        cout << "Purchase successful! Transaction ID: " << t.id << endl;
        return true;
    }

    void purchaseItemFlow() {
        string buyerName, sellerName, itemName;
        int quantity;

        cout << "Enter your username: ";
        cin >> buyerName;
        if (User::storeAccounts.find(buyerName) == User::storeAccounts.end()) {
            cout << "Buyer not found.\n";
            return;
        }
        User& buyer = User::storeAccounts[buyerName];

        cout << "Enter seller username: ";
        cin >> sellerName;
        if (User::storeAccounts.find(sellerName) == User::storeAccounts.end()) {
            cout << "Seller not found.\n";
            return;
        }
        Seller& seller = dynamic_cast<Seller&>(User::storeAccounts[sellerName]);

        if (!ensureBankAccount(buyer) || !ensureBankAccount(seller)) return;

        cout << "Enter item name: ";
        cin >> itemName;
        cout << "Enter quantity: ";
        cin >> quantity;

    
        this->purchaseItem(buyer, seller, itemName, quantity);
    }



    void listTransactionsLastKDays(int k) {
        time_t now = time(nullptr);
        cout << "\n--- Transactions in Last " << k << " Days ---\n";
        for (const auto& t : storeLedger) {
            double diff = difftime(now, t.timestamp);
            int days = diff / (60 * 60 * 24);
            if (days < k) {
                cout << "ID: " << t.id << ", Item: " << t.itemName
                     << ", Buyer: " << t.buyerId << ", Seller: " << t.sellerId
                     << ", Status: " << t.status << ", Date: " << ctime(&t.timestamp);
            }
        }
    }

    void listUncompletedPaidTransactions() {
        cout << "\n--- Paid but Not Completed Transactions ---\n";
        for (const auto& t : storeLedger) {
            if (t.status == "Paid") {
                cout << "ID: " << t.id << ", Item: " << t.itemName
                     << ", Buyer: " << t.buyerId << ", Seller: " << t.sellerId
                     << ", Date: " << ctime(&t.timestamp);
            }
        }
    }

    vector<string> mostFrequentItems(int m) {
        unordered_map<string, int> freq;
        for (const auto& t : storeLedger) {
            freq[t.itemName]++;
        }
        vector<pair<string, int>> sortedStuff(freq.begin(), freq.end());
        sort(sortedStuff.begin(), sortedStuff.end(),
            [](const pair<string, int>& a, const pair<string, int>& b) {
                return a.second > b.second; 
            });
        vector<string> result;
        for (int i = 0; i < min(m, (int)sortedStuff.size()); ++i) {
            result.push_back(sortedStuff[i].first);
        }
        return result;
    }

    unordered_map<string, int> mostActiveBuyersPerDay() {
        unordered_map<string, int> buyerCount;
        time_t now = time(nullptr);
        tm* now_tm = localtime(&now);
        for (const auto& t : storeLedger) {
            tm* trans_tm = localtime(&t.timestamp);
            if (trans_tm->tm_year == now_tm->tm_year &&
                trans_tm->tm_mon == now_tm->tm_mon &&
                trans_tm->tm_mday == now_tm->tm_mday) {
                buyerCount[t.buyerId]++;
            }
        }
        return buyerCount;
    }

    unordered_map<string, int> mostActiveSellersPerDay() {
        unordered_map<string, int> sellerCount;
        time_t now = time(nullptr);
        tm* now_tm = localtime(&now);
        for (const auto& t : storeLedger) {
            tm* trans_tm = localtime(&t.timestamp);
            if (trans_tm->tm_year == now_tm->tm_year &&
                trans_tm->tm_mon == now_tm->tm_mon &&
                trans_tm->tm_mday == now_tm->tm_mday) {
                sellerCount[t.sellerId]++;
            }
        }
        return sellerCount;
    }
};
