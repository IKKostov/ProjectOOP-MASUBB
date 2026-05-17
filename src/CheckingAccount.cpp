#include "CheckingAccount.h"
#include <iostream>
#include <iomanip>
 
CheckingAccount::CheckingAccount(const std::string& id, double initialBalance,
                                 const std::string& currency, double overdraft)
    : Account(id, initialBalance, currency, AccountType::CHECKING),
      overdraftLimit(overdraft) {}
 
bool CheckingAccount::deposit(double amount) {
    if (!isActive()) {
        std::cout << "  [!] Account is not active.\n";
        return false;
    }
    if (amount <= 0) {
        std::cout << "  [!] Amount must be positive.\n";
        return false;
    }
    updateBalance(amount);
    std::cout << "  [OK] Deposited " << std::fixed << std::setprecision(2)
              << amount << " " << getCurrency() << ". New balance: "
              << getBalance() << " " << getCurrency() << "\n";
    return true;
}
 
bool CheckingAccount::withdraw(double amount) {
    if (!isActive()) {
        std::cout << "  [!] Balance is not active.\n";
        return false;
    }
    if (amount <= 0) {
        std::cout << "  [!] Amount must be positive.\n";
        return false;
    }
    if (getBalance() - amount < -overdraftLimit) {
        std::cout << "  [!] Insufficient balance (overdraft limit: "
                  << overdraftLimit << " " << getCurrency() << ").\n";
        return false;
    }
    updateBalance(-amount);
    std::cout << "  [OK] Withdrawn " << std::fixed << std::setprecision(2)
              << amount << " " << getCurrency() << ". New balance: "
              << getBalance() << " " << getCurrency() << "\n";
    return true;
}
 
void CheckingAccount::printInfo() const {
    Account::printInfo();
    std::cout << "  Overdraft: " << std::fixed << std::setprecision(2)
              << overdraftLimit << " " << getCurrency() << "\n";
}