#pragma once
#include "Account.h"
 
enum class RiskLevel { LOW, MEDIUM, HIGH };
 
class InvestmentAccount : public Account {
private:
    RiskLevel riskLevel;
    double    returnRate; // expected annual return rate
 
public:
    InvestmentAccount(const std::string& id, double initialBalance,
                      const std::string& currency,
                      RiskLevel risk = RiskLevel::MEDIUM,
                      double returnRate = 0.07);
 
    bool deposit(double amount)  override;
    bool withdraw(double amount) override;
 
    // Calculates expected returns without modifying balance
    double calcReturns(int years) const;
 
    RiskLevel getRiskLevel()  const { return riskLevel;  }
    double    getReturnRate() const { return returnRate; }
    std::string riskToString() const;
    void printInfo() const override;
};