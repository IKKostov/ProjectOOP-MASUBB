#include "SavingsAccount.h"
#include <iostream>
#include <iomanip>
 
SavingsAccount::SavingsAccount(const std::string& id, double initialBalance,
                               const std::string& currency, double rate)
    : Account(id, initialBalance, currency, AccountType::SAVINGS),
      interestRate(rate) {}
 
bool SavingsAccount::deposit(double amount) {
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
 
bool SavingsAccount::withdraw(double amount) {
    if (!isActive()) {
        std::cout << "  [!] Account is not active.\n";
        return false;
    }
    if (amount <= 0) {
        std::cout << "  [!] Amount must be positive.\n";
        return false;
    }
    if (amount > getBalance()) {
        std::cout << "  [!] Insufficient balance.\n";
        return false;
    }
    updateBalance(-amount);
    std::cout << "  [OK] Withdrawn " << std::fixed << std::setprecision(2)
              << amount << " " << getCurrency() << ". New balance: "
              << getBalance() << " " << getCurrency() << "\n";
    return true;
}
 
void SavingsAccount::applyInterest() {
    if (!isActive()) {
        std::cout << "  [!] Account is not active.\n";
        return;
    }
    double interest = getBalance() * interestRate;
    updateBalance(interest);
    std::cout << "  [OK] Interest applied: " << std::fixed << std::setprecision(2)
              << interest << " " << getCurrency()
              << ". New balance: " << getBalance() << " " << getCurrency() << "\n";
}
 
void SavingsAccount::printInfo() const {
    Account::printInfo();
    std::cout << "  Interest Rate: " << std::fixed << std::setprecision(1)
              << interestRate * 100 << "%\n";
}