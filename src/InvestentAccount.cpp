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
        std::cout << "  [!] Сметката не е активна.\n";
        return false;
    }
    if (amount <= 0) {
        std::cout << "  [!] Сумата трябва да е положителна.\n";
        return false;
    }
    updateBalance(amount);
    std::cout << "  [OK] Депозирани " << std::fixed << std::setprecision(2)
              << amount << " " << getCurrency() << ". Нов баланс: "
              << getBalance() << " " << getCurrency() << "\n";
    return true;
}

bool InvestmentAccount::withdraw(double amount) {
    if (!isActive()) {
        std::cout << "  [!] Сметката не е активна.\n";
        return false;
    }
    if (amount <= 0) {
        std::cout << "  [!] Сумата трябва да е положителна.\n";
        return false;
    }
    if (amount > getBalance()) {
        std::cout << "  [!] Недостатъчен баланс.\n";
        return false;
    }
    updateBalance(-amount);
    std::cout << "  [OK] Изтеглени " << std::fixed << std::setprecision(2)
              << amount << " " << getCurrency() << ". Нов баланс: "
              << getBalance() << " " << getCurrency() << "\n";
    return true;
}

double InvestmentAccount::calcReturns(int years) const {
    // Compound interest: P * (1 + r)^n
    return getBalance() * (std::pow(1.0 + returnRate, years) - 1.0);
}

std::string InvestmentAccount::riskToString() const {
    switch (riskLevel) {
        case RiskLevel::LOW:    return "Нисък";
        case RiskLevel::MEDIUM: return "Среден";
        case RiskLevel::HIGH:   return "Висок";
    }
    return "Неизвестен";
}

void InvestmentAccount::printInfo() const {
    Account::printInfo();
    std::cout << "  Риск    : " << riskToString() << "\n"
              << "  Доходност: " << std::fixed << std::setprecision(1)
              << returnRate * 100 << "% годишно\n";
}

void InvestmentAccount::applyInterest() {
    if (!isActive()) {
        std::cout << "  [!] Сметката не е активна.\n";
        return;
    }
    double gain = getBalance() * returnRate;
    updateBalance(gain);
    std::cout << "  [OK] Начислена годишна доходност: "
              << std::fixed << std::setprecision(2)
              << gain << " " << getCurrency()
              << ". Нов баланс: " << getBalance() << " " << getCurrency() << "\n";
}