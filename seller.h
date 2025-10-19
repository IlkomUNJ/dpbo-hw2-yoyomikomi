#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "transaction.h"
using namespace std;

struct Item {
    string name;
    double price;
    int stock;
    int soldCount;
};

class Seller : public User {
public:
    unordered_map<string, int> buyerPurchaseCount;
    unordered_map<string, Item> items;
    vector<Transaction> sellerSales;

    string getRole() const override {
        return "Seller";
    }

    void registerItem(const string& name, double price, int stock) {
        if (items.find(name) != items.end()) {
            cout << "Item already exists.\n";
            return;
        }
        items[name] = {name, price, stock, 0};
        cout << "Item " << name << " added with price " << price << " and stock " << stock << endl;
    }

    void replenishItem(const string& name, int quantity) {
        if (items.find(name) == items.end()) {
            cout << "Item not found.\n";
            return;
        }
        items[name].stock += quantity;
        cout << "Item " << name << " replenished. New stock: " << items[name].stock << endl;
    }

    void discardItem(const string& name) {
        if (items.erase(name)) {
            cout << "Item discarded.\n";
        } else {
            cout << "Item not found.\n";
        }
    }

    void setPrice(const string& name, double newPrice) {
        if (items.find(name) == items.end()) {
            cout << "Item not found.\n";
            return;
        }
        items[name].price = newPrice;
        cout << "New price for " << name << ": " << newPrice << endl;
    }

    double getItemPrice(const string& name) const {
        auto it = items.find(name);
        return (it != items.end()) ? it->second.price : -1.0;
    }

    vector<string> getTopKItems(int k) {
        unordered_map<string, int> itemCount;
        time_t now = time(nullptr);
        tm* now_tm = localtime(&now);
        for (const auto& t : sellerSales) {
            tm* trans_tm = localtime(&t.timestamp);
            if (trans_tm->tm_year == now_tm->tm_year && trans_tm->tm_mon == now_tm->tm_mon) {
                itemCount[t.itemName]++;
            }
        }

        vector<pair<string, int>> sortedItems(itemCount.begin(), itemCount.end());
        sort(sortedItems.begin(), sortedItems.end(),
            [](const pair<string, int>& a, const pair<string, int>& b) {
                return a.second > b.second; 
            });
        vector<string> topItems;
        for (int i = 0; i < min(k, (int)sortedItems.size()); ++i) {
            topItems.push_back(sortedItems[i].first);
        }
        return topItems;
    }

    string getLoyalCustomer() {
        unordered_map<string, int> buyerCount;
        time_t now = time(nullptr);
        tm* now_tm = localtime(&now);
        for (const auto& t : sellerSales) {
            tm* trans_tm = localtime(&t.timestamp);
            if (trans_tm->tm_year == now_tm->tm_year && trans_tm->tm_mon == now_tm->tm_mon) {
                buyerCount[t.buyerId]++;
            }
        }
        string loyalBuyer;
        int maxCount = 0;
        for (const auto& [buyer, count] : buyerCount) {
            if (count > maxCount) {
                maxCount = count;
                loyalBuyer = buyer;
            }
        }
        return loyalBuyer;
    }
};