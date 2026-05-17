#pragma once
#include "Account.h"
 
class SavingsAccount : public Account {
private:
    double interestRate; // annual rate, e.g. 0.05 = 5%
 
public:
    SavingsAccount(const std::string& id, double initialBalance,
                   const std::string& currency, double interestRate = 0.03);
 
    bool deposit(double amount)  override;
    bool withdraw(double amount) override;
 
    // Calculates and adds interest to the balance
    void applyInterest();
 
    double getInterestRate() const { return interestRate; }
    void   printInfo()       const override;
};