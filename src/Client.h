#pragma once
#include <string>
#include <vector>
#include "Account.h"
#include "CheckingAccount.h"
#include "SavingsAccount.h"
#include "InvestmentAccount.h"
 
class Client {
private:
    std::string clientId;
    std::string name;
    std::string egn;         // ЕГН
    std::vector<Account*> accounts;
 
    static int idCounter;    // auto-increment for account IDs
 
public:
    Client(const std::string& clientId,
           const std::string& name,
           const std::string& egn);
    ~Client();
 
    // Functionality 1 — client info
    void printInfo() const;
 
    // Functionality 2 — account management
    Account* openAccount(AccountType type, double initialBalance,
                         const std::string& currency = "BGN",
                         double extraParam = 0.0);
    bool     closeAccount(const std::string& accountId);
    bool     blockAccount(const std::string& accountId);
    bool     unblockAccount(const std::string& accountId);
 
    // Lookup
    Account* findAccount(const std::string& accountId) const;
    void     listAccounts() const;
 
    // Getters
    std::string getClientId() const { return clientId; }
    std::string getName()     const { return name;     }
    std::string getEGN()      const { return egn;      }
    const std::vector<Account*>& getAccounts() const { return accounts; }
};
