#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>
#include "Client.h"
#include "Transaction.h"
#include "SavingsAccount.h"
#include "InvestmentAccount.h"
 
// ── Helpers ─────────────────────────────────────────────────────────────────
 
void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
 
int readInt(const std::string& prompt) {
    int val;
    std::cout << prompt;
    while (!(std::cin >> val)) { clearInput(); std::cout << prompt; }
    clearInput();
    return val;
}
 
double readDouble(const std::string& prompt) {
    double val;
    std::cout << prompt;
    while (!(std::cin >> val)) { clearInput(); std::cout << prompt; }
    clearInput();
    return val;
}
 
std::string readString(const std::string& prompt) {
    std::string val;
    std::cout << prompt;
    std::getline(std::cin, val);
    return val;
}
 
// ── Find helpers ─────────────────────────────────────────────────────────────
 
Client* findClient(std::vector<Client*>& clients, const std::string& id) {
    for (Client* c : clients)
        if (c->getClientId() == id) return c;
    return nullptr;
}
 
Account* findAccountGlobal(std::vector<Client*>& clients, const std::string& accId) {
    for (Client* c : clients) {
        Account* acc = c->findAccount(accId);
        if (acc) return acc;
    }
    return nullptr;
}
 
// ── Menus ────────────────────────────────────────────────────────────────────
 
void menuClients(std::vector<Client*>& clients) {
    static int clientCounter = 1;
    int choice;
    do {
        std::cout << "\n=== Client Management ===\n"
                  << "1. Register New Client\n"
                  << "2. Show All Clients\n"
                  << "3. Client Details\n"
                  << "0. Back\n";
        choice = readInt("Choice: ");
 
        if (choice == 1) {
            std::string name = readString("Full Name: ");
            std::string egn  = readString("EGN: ");
            std::string id   = "CLI" + std::to_string(clientCounter++);
            clients.push_back(new Client(id, name, egn));
            std::cout << "  [OK] Client registered with ID: " << id << "\n";
 
        } else if (choice == 2) {
            if (clients.empty()) { std::cout << "  No registered clients.\n"; continue; }
            for (Client* c : clients) c->printInfo();
 
        } else if (choice == 3) {
            std::string id = readString("Client ID: ");
            Client* c = findClient(clients, id);
            if (!c) { std::cout << "  [!] Client not found.\n"; continue; }
            c->printInfo();
            c->listAccounts();
        }
    } while (choice != 0);
}
 
void menuAccounts(std::vector<Client*>& clients) {
    int choice;
    do {
        std::cout << "\n=== Account Management ===\n"
                  << "1. Open Account\n"
                  << "2. Block Account\n"
                  << "3. Unblock Account\n"
                  << "4. Close Account\n"
                  << "0. Back\n";
        choice = readInt("Choice: ");
 
        if (choice == 1) {
            std::string cid = readString("Client ID: ");
            Client* c = findClient(clients, cid);
            if (!c) { std::cout << "  [!] Client not found.\n"; continue; }
 
            std::cout << "Type: 1=Checking  2=Savings  3=Investment\n";
            int type = readInt("Type: ");
            double init = readDouble("Initial Balance: ");
            std::string cur = readString("Currency (BGN/EUR/USD): ");
            double extra = 0;
            if (type == 1) extra = readDouble("Overdraft Limit (0 = none): ");
            if (type == 2) extra = readDouble("Interest Rate (e.g., 0.04): ");
            if (type == 3) extra = readDouble("Expected Return (e.g., 0.07): ");
 
            AccountType at = (type == 1) ? AccountType::CHECKING
                           : (type == 2) ? AccountType::SAVINGS
                                         : AccountType::INVESTMENT;
            c->openAccount(at, init, cur, extra);
 
        } else if (choice == 2) {
            std::string cid = readString("Client ID: ");
            Client* c = findClient(clients, cid);
            if (!c) { std::cout << "  [!] Client not found.\n"; continue; }
            std::string aid = readString("Account ID: ");
            c->blockAccount(aid);
 
        } else if (choice == 3) {
            std::string cid = readString("Client ID: ");
            Client* c = findClient(clients, cid);
            if (!c) { std::cout << "  [!] Client not found.\n"; continue; }
            std::string aid = readString("Account ID: ");
            c->unblockAccount(aid);
 
        } else if (choice == 4) {
            std::string cid = readString("Client ID: ");
            Client* c = findClient(clients, cid);
            if (!c) { std::cout << "  [!] Client not found.\n"; continue; }
            std::string aid = readString("Account ID: ");
            c->closeAccount(aid);
        }
    } while (choice != 0);
}
 
void menuTransactions(std::vector<Client*>& clients, TransactionManager& tm) {
    int choice;
    do {
        std::cout << "\n=== Transaction Management ===\n"
                  << "1. Deposit\n"
                  << "2. Withdraw\n"
                  << "3. Transfer Between Accounts\n"
                  << "4. History (All)\n"
                  << "5. History By Account\n"
                  << "0. Back\n";
        choice = readInt("Choice: ");
 
        if (choice == 1) {
            std::string aid = readString("Account ID: ");
            Account* acc = findAccountGlobal(clients, aid);
            if (!acc) { std::cout << "  [!] Account not found.\n"; continue; }
            double amt = readDouble("Amount: ");
            tm.deposit(acc, amt);
 
        } else if (choice == 2) {
            std::string aid = readString("Account ID: ");
            Account* acc = findAccountGlobal(clients, aid);
            if (!acc) { std::cout << "  [!] Account not found.\n"; continue; }
            double amt = readDouble("Amount: ");
            tm.withdraw(acc, amt);
 
        } else if (choice == 3) {
            std::string fromId = readString("From Account ID: ");
            std::string toId   = readString("To Account ID: ");
            Account* from = findAccountGlobal(clients, fromId);
            Account* to   = findAccountGlobal(clients, toId);
            if (!from || !to) { std::cout << "  [!] Invalid account.\n"; continue; }
            double amt = readDouble("Amount: ");
            tm.transfer(from, to, amt);
 
        } else if (choice == 4) {
            tm.printHistory();
 
        } else if (choice == 5) {
            std::string aid = readString("Account ID: ");
            tm.printHistoryForAccount(aid);
        }
    } while (choice != 0);
}
 
void menuExtra(std::vector<Client*>& clients) {
    int choice;
    do {
        std::cout << "\n=== Extra Operations ===\n"
                  << "1. Apply Interest (Savings Account)\n"
                  << "2. Return Projection (Investment Account)\n"
                  << "0. Back\n";
        choice = readInt("Choice: ");
 
        if (choice == 1) {
            std::string aid = readString("Savings Account ID: ");
            Account* acc = findAccountGlobal(clients, aid);
            if (!acc) { std::cout << "  [!] Account not found.\n"; continue; }
            SavingsAccount* sa = dynamic_cast<SavingsAccount*>(acc);
            if (!sa) { std::cout << "  [!] The account is not a savings account.\n"; continue; }
            sa->applyInterest();
 
        } else if (choice == 2) {
            std::string aid = readString("Investment Account ID: ");
            Account* acc = findAccountGlobal(clients, aid);
            if (!acc) { std::cout << "  [!] Account not found.\n"; continue; }
            InvestmentAccount* ia = dynamic_cast<InvestmentAccount*>(acc);
            if (!ia) { std::cout << "  [!] The account is not an investment account.\n"; continue; }
            int years = readInt("Number of Years: ");
            double returns = ia->calcReturns(years);
            std::cout << "  Projected Return for " << years << " Years: "
                      << std::fixed << std::setprecision(2)
                      << returns << " " << ia->getCurrency() << "\n";
        }
    } while (choice != 0);
}
 
// ── Main ─────────────────────────────────────────────────────────────────────
 
int main() {
    std::cout << "====================================\n"
              << "  MASUBB — Банкова система v1.0\n"
              << "  Managing A System Used By Banks\n"
              << "====================================\n";
 
    std::vector<Client*> clients;
    TransactionManager tm;
 
    int choice;
    do {
        std::cout << "\n=== Main Menu ===\n"
                  << "1. Client Management\n"
                  << "2. Account Management\n"
                  << "3. Transactions\n"
                  << "4. Extra Operations\n"
                  << "0. Exit\n";
        choice = readInt("Choice: ");
 
        switch (choice) {
            case 1: menuClients(clients); break;
            case 2: menuAccounts(clients);  break;
            case 3: menuTransactions(clients, tm); break;
            case 4: menuExtra(clients);     break;
        }
    } while (choice != 0);
 
    std::cout << "\n  Goodbye!\n";
 
    for (Client* c : clients) delete c;
    return 0;
}