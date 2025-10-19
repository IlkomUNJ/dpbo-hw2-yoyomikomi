#pragma once
#include <string>
#include <ctime>
using namespace std;

struct Transaction {
    string id;
    string buyerId;
    string sellerId;
    string itemName;
    double amount;
    string status;
    time_t timestamp;
};
