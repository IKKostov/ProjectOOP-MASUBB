#pragma once
#include "Account.h"
 
class CheckingAccount : public Account {
private:
    double overdraftLimit; // how much below 0 the client may go
 
public:
    CheckingAccount(const std::string& id, double initialBalance,
                    const std::string& currency, double overdraftLimit = 0.0);
 
    bool deposit(double amount)  override;
    bool withdraw(double amount) override;
 
    double getOverdraftLimit() const { return overdraftLimit; }
    void   printInfo()         const override;
};