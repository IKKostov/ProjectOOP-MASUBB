#include "Client.h"
#include <iostream>
#include <iomanip>
 
int Client::idCounter = 1000;
 
Client::Client(const std::string& id, const std::string& n, const std::string& e)
    : clientId(id), name(n), egn(e) {}
 
Client::~Client() {
    for (Account* acc : accounts)
        delete acc;
}
 
void Client::printInfo() const {
    std::cout << "=============================\n"
              << "  Client  : " << name << "\n"
              << "  ID      : " << clientId << "\n"
              << "  EGN     : " << egn << "\n"
              << "  Accounts: " << accounts.size() << "\n"
              << "=============================\n";
}
 
Account* Client::openAccount(AccountType type, double initialBalance,
                              const std::string& currency, double extraParam) {
    std::string accId = "ACC" + std::to_string(++idCounter);
    Account* acc = nullptr;
 
    switch (type) {
        case AccountType::CHECKING:
            acc = new CheckingAccount(accId, initialBalance, currency, extraParam);
            break;
        case AccountType::SAVINGS:
            acc = new SavingsAccount(accId, initialBalance, currency,
                                     extraParam > 0 ? extraParam : 0.03);
            break;
        case AccountType::INVESTMENT:
            acc = new InvestmentAccount(accId, initialBalance, currency,
                                        RiskLevel::MEDIUM,
                                        extraParam > 0 ? extraParam : 0.07);
            break;
    }
 
    if (acc) {
        accounts.push_back(acc);
        std::cout << "  [OK] Account found " << accId
                  << " (" << acc->typeToString() << ") for " << name << "\n";
    }
    return acc;
}
 
bool Client::closeAccount(const std::string& accountId) {
    Account* acc = findAccount(accountId);
    if (!acc) {
        std::cout << "  [!] Account " << accountId << " not found.\n";
        return false;
    }
    acc->changeStatus(AccountStatus::CLOSED);
    std::cout << "  [OK] Account " << accountId << " closed.\n";
    return true;
}
 
bool Client::blockAccount(const std::string& accountId) {
    Account* acc = findAccount(accountId);
    if (!acc) {
        std::cout << "  [!] Account " << accountId << " not found.\n";
        return false;
    }
    acc->changeStatus(AccountStatus::BLOCKED);
    std::cout << "  [OK] Account " << accountId << " blocked.\n";
    return true;
}
 
bool Client::unblockAccount(const std::string& accountId) {
    Account* acc = findAccount(accountId);
    if (!acc) {
        std::cout << "  [!] Account " << accountId << " not found.\n";
        return false;
    }
    acc->changeStatus(AccountStatus::ACTIVE);
    std::cout << "  [OK] Account " << accountId << " unblocked  .\n";
    return true;
}
 
Account* Client::findAccount(const std::string& accountId) const {
    for (Account* acc : accounts)
        if (acc->getAccountId() == accountId)
            return acc;
    return nullptr;
}
 
void Client::listAccounts() const {
    if (accounts.empty()) {
        std::cout << "  No accounts found.\n";
        return;
    }
    std::cout << "  Accounts for " << name << ":\n";
    for (Account* acc : accounts) {
        std::cout << "  ---------------------------\n";
        acc->printInfo();
    }
}