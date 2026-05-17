#pragma once
#include <string>
 
enum class AccountStatus { ACTIVE, BLOCKED, CLOSED };
enum class AccountType   { CHECKING, SAVINGS, INVESTMENT };
 
class Account {
protected:
    std::string accountId;
    double      balance;
    AccountStatus status;
    std::string currency;
    AccountType type;
 
    // Only subclasses may touch balance directly
    void updateBalance(double amount) { balance += amount; }
 
public:
    Account(const std::string& id, double initialBalance,
            const std::string& currency, AccountType type);
    virtual ~Account() = default;
 
    // Pure virtual — each subclass implements its own rules
    virtual bool deposit(double amount)  = 0;
    virtual bool withdraw(double amount) = 0;
 
    // Common operations
    void changeStatus(AccountStatus s) { status = s; }
    bool isActive() const { return status == AccountStatus::ACTIVE; }
 
    // Getters
    std::string   getAccountId() const { return accountId; }
    double        getBalance()   const { return balance;   }
    AccountStatus getStatus()    const { return status;    }
    AccountType   getType()      const { return type;      }
    std::string   getCurrency()  const { return currency;  }
 
    std::string statusToString() const;
    std::string typeToString()   const;
 
    virtual void printInfo() const;
};