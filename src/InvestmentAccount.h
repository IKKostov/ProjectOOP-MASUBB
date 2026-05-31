#pragma once
#include "Account.h"

enum class RiskLevel { LOW, MEDIUM, HIGH };

class InvestmentAccount : public Account {
private:
    RiskLevel riskLevel;
    double    returnRate;

public:
    InvestmentAccount(const std::string& id, double initialBalance,
                      const std::string& currency,
                      RiskLevel risk = RiskLevel::MEDIUM,
                      double returnRate = 0.07);

    bool deposit(double amount)  override;
    bool withdraw(double amount) override;

    // Изчислява очаквана доходност за N години (не променя баланса)
    double calcReturns(int years) const;

    // Начислява едногодишна доходност към баланса
    void applyInterest();

    RiskLevel   getRiskLevel()  const { return riskLevel;  }
    double      getReturnRate() const { return returnRate; }
    std::string riskToString()  const;
    void        printInfo()     const override;
};