#include "InvestmentAccount.h"
#include <iostream>
#include <iomanip>
#include <cmath>
 
InvestmentAccount::InvestmentAccount(const std::string& id, double initialBalance,
                                     const std::string& currency,
                                     RiskLevel risk, double rate)
    : Account(id, initialBalance, currency, AccountType::INVESTMENT),
      riskLevel(risk), returnRate(rate) {}
 
bool InvestmentAccount::deposit(double amount) {
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
 
bool InvestmentAccount::withdraw(double amount) {
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
              << amount << " " << getCurrency() << ". New balance : "
              << getBalance() << " " << getCurrency() << "\n";
    return true;
}
 
double InvestmentAccount::calcReturns(int years) const {
    // Compound interest: P * (1 + r)^n
    return getBalance() * (std::pow(1.0 + returnRate, years) - 1.0);
}
 
std::string InvestmentAccount::riskToString() const {
    switch (riskLevel) {
        case RiskLevel::LOW:    return "Low";
        case RiskLevel::MEDIUM: return "Medium";
        case RiskLevel::HIGH:   return "High";
    }
    return "Unknown";
}
 
void InvestmentAccount::printInfo() const {
    Account::printInfo();
    std::cout << "  Risk    : " << riskToString() << "\n"
              << "  Return Rate: " << std::fixed << std::setprecision(1)
              << returnRate * 100 << "% annually\n";
}