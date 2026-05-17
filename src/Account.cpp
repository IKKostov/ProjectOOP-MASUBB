#include "Account.h"
#include <iostream>
#include <iomanip>
 
Account::Account(const std::string& id, double initialBalance,
                 const std::string& curr, AccountType t)
    : accountId(id), balance(initialBalance),
      status(AccountStatus::ACTIVE), currency(curr), type(t) {}
 
std::string Account::statusToString() const {
    switch (status) {
        case AccountStatus::ACTIVE:  return "Active";
        case AccountStatus::BLOCKED: return "Blocked";
        case AccountStatus::CLOSED:  return "Closed";
    }
    return "Unknown";
}
 
std::string Account::typeToString() const {
    switch (type) {
        case AccountType::CHECKING:   return "Checking";
        case AccountType::SAVINGS:    return "Savings";
        case AccountType::INVESTMENT: return "Investment";
    }
    return "Unknown";
}
 
void Account::printInfo() const {
    std::cout << "  ID      : " << accountId << "\n"
              << "  Type    : " << typeToString() << "\n"
              << "  Balance : " << std::fixed << std::setprecision(2)
                                << balance << " " << currency << "\n"
              << "  Status  : " << statusToString() << "\n";
}
 