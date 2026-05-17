#pragma once
#include <string>
#include <vector>
#include "Account.h"
 
enum class TxType { DEPOSIT, WITHDRAWAL, TRANSFER_OUT, TRANSFER_IN };
 
struct Transaction {
    std::string txId;
    TxType      type;
    double      amount;
    std::string currency;
    std::string fromAccountId;
    std::string toAccountId;   // empty for deposit/withdrawal
    std::string timestamp;     // simple string timestamp
 
    std::string typeToString() const;
    void print() const;
};
 
// TransactionManager handles execution and history
class TransactionManager {
private:
    std::vector<Transaction> history;
    static int txCounter;
 
    std::string generateId();
    std::string currentTimestamp();
 
public:
    // Functionality 3 — deposit & withdraw
    bool deposit (Account* acc, double amount);
    bool withdraw(Account* acc, double amount);
 
    // Functionality 4 — transfer between accounts
    bool transfer(Account* from, Account* to, double amount);
 
    // Functionality 5 — transaction history
    void printHistory() const;
    void printHistoryForAccount(const std::string& accountId) const;
};
 