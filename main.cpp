#include <iostream>
#include <string>
#include <vector>
#include "user.h"
#include "seller.h"
#include "store.h"
#include "bank.h"
using namespace std;

enum MainMenu { MENU_LOGIN = 1, MENU_REGISTER, MENU_BANKING, MENU_STORE, MENU_PURCHASE, MENU_EXIT };
enum BankingMenu { BANK_CHECK_BALANCE = 1, BANK_DEPOSIT, BANK_WITHDRAW, BANK_TRANSFER, BANK_BACK };
enum StoreMenu { STORE_LIST_K_DAYS = 1, STORE_LIST_UNCOMPLETED, STORE_TOP_M_ITEMS, STORE_ACTIVE_BUYERS, STORE_ACTIVE_SELLERS, STORE_BACK };

void showMainMenu() {
    cout << "\n==== MAIN MENU ====\n";
    cout << "1. Login\n2. Register\n3. Banking\n4. Store Analytics\n5. Purchase Item\n6. Exit\n";
}
void showBankingMenu() {
    cout << "\n==== BANKING MENU ====\n";
    cout << "1. Check Balance\n2. Deposit\n3. Withdraw\n4. Transfer\n5. Back\n";
}
void showStoreMenu() {
    cout << "\n==== STORE ANALYTICS MENU ====\n";
    cout << "1. List transactions in last K days\n2. List paid but not completed transactions\n";
    cout << "3. List top M frequent items\n4. Most active buyers today\n5. Most active sellers today\n6. Back\n";
}

User* ensureBankAccount(User& user) {
    if (user.getAccount().empty()) {
        cout << "You don't have a bank account yet. Please create one first.\n";
        if (!user.CreateBankAccount()) {
            cout << "Bank account creation cancelled.\n";
            return nullptr; 
        }
    }
    return &user;
}

int main() {
    BankSystem bankSystem;
    Store storeSystem;
    string currentUserEmail;
    bool loggedIn = false;
    int choice;

    do {
        showMainMenu();
        cout << "Choose: ";
        cin >> choice;

        switch (choice) {

        case MENU_REGISTER: {
            cout << "\n--- REGISTER ---\n";
            cout << "Do you want to be a buyer or seller? (b/s): ";
            char role; cin >> role;

            if (role == 'b') {
                User tempUser;
                if (tempUser.UserRegister()) {
                    cout << "Buyer registered successfully.\n";
                }
            } else if (role == 's') {
                Seller tempSeller;
                if (tempSeller.UserRegister()) {
                    cout << "Seller registered successfully.\n";

                    char addMore = 'y';
                    while (addMore == 'y') {
                        string itemName;
                        double price;
                        int stock;
                        cout << "Enter item name: "; cin >> itemName;
                        cout << "Enter price: "; cin >> price;
                        cout << "Enter stock: "; cin >> stock;
                        tempSeller.registerItem(itemName, price, stock);
                        cout << "Add another item? (y/n): "; cin >> addMore;
                    }
                }
            } else {
                cout << "Invalid role.\n";
            }
            break;
        }

        case MENU_LOGIN: {
            string uname, pw;
            cout << "\n--- LOGIN ---\nUsername: ";
            cin >> uname;
            if (User::storeAccounts.find(uname) == User::storeAccounts.end()) {
                cout << "User not found.\n";
            } else {
                User& u = User::storeAccounts[uname];
                cout << "Password: ";
                cin >> pw;
                if (pw == u.getPassword()) {
                    loggedIn = true;
                    currentUserEmail = u.getEmail();
                    cout << "Login successful! Welcome, " << u.getUsername() << "\n";
                    cout << "Role: " << u.getRole() << "\n";
                } else {
                    cout << "Wrong password.\n";
                }
            }
            break;
        }

        case MENU_BANKING: {
            if (!loggedIn) { cout << "You must log in first.\n"; break; }
            int bankChoice;
            User& currentUser = User::emails[currentUserEmail];
            do {
                showBankingMenu();
                cout << "Choose: "; cin >> bankChoice;
                switch (bankChoice) {
                    case BANK_CHECK_BALANCE:
                        cout << "Balance: " << currentUser.getBalance() << endl; break;
                    case BANK_DEPOSIT: {
                        double amt; cout << "Enter amount: "; cin >> amt;
                        bankSystem.deposit(currentUserEmail, amt); break;
                    }
                    case BANK_WITHDRAW: {
                        double amt; cout << "Enter amount: "; cin >> amt;
                        bankSystem.withdraw(currentUserEmail, amt); break;
                    }
                    case BANK_TRANSFER: {
                        double amt; string rec;
                        cout << "Receiver email: "; cin >> rec;
                        cout << "Amount: "; cin >> amt;
                        bankSystem.transfer(currentUserEmail, rec, amt); break;
                    }
                    case BANK_BACK: cout << "Returning...\n"; break;
                    default: cout << "Invalid choice.\n";
                }
            } while (bankChoice != BANK_BACK);
            break;
        }

        case MENU_STORE: {
            int storeChoice;
            do {
                showStoreMenu();
                cout << "Choose: "; cin >> storeChoice;
                switch (storeChoice) {
                    case STORE_LIST_K_DAYS: {
                        int k; cout << "Enter number of days: "; cin >> k;
                        storeSystem.listTransactionsLastKDays(k);
                        break;
                    }
                    case STORE_LIST_UNCOMPLETED:
                        storeSystem.listUncompletedPaidTransactions();
                        break;
                    case STORE_TOP_M_ITEMS: {
                        int m; cout << "Enter number of top items: "; cin >> m;
                        vector<string> topItems = storeSystem.mostFrequentItems(m);
                        for (auto& item : topItems) cout << "- " << item << endl;
                        break;
                    }
                    case STORE_ACTIVE_BUYERS: {
                        auto buyers = storeSystem.mostActiveBuyersPerDay();
                        for (auto& [b, count] : buyers) cout << b << ": " << count << " transactions\n";
                        break;
                    }
                    case STORE_ACTIVE_SELLERS: {
                        auto sellers = storeSystem.mostActiveSellersPerDay();
                        for (auto& [s, count] : sellers) cout << s << ": " << count << " transactions\n";
                        break;
                    }
                    case STORE_BACK: cout << "Returning...\n"; break;
                    default: cout << "Invalid choice.\n";
                }
            } while (storeChoice != STORE_BACK);
            break;
        }

        case MENU_PURCHASE: {
            storeSystem.purchaseItemFlow();
            break;
        }

        case MENU_EXIT:
            cout << "Exiting...\n"; break;

        default:
            cout << "Invalid choice.\n";
        }

    } while (choice != MENU_EXIT);

    return 0;
}
