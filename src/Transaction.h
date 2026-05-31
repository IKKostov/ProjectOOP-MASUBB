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
    std::string toAccountId;
    std::string timestamp;

    std::string typeToString() const;
    void print() const;
};

class TransactionManager {
private:
    std::vector<Transaction> history;
    static int txCounter;

    std::string generateId();
    std::string currentTimestamp();

public:
    // Функционалност 3 — депозит & теглене
    bool deposit (Account* acc, double amount);
    bool withdraw(Account* acc, double amount);

    // Функционалност 4 — превод (поддържа различни валути)
    bool transfer(Account* from, Account* to,
                  double amount, double convertedAmount);

    // Функционалност 5 — история
    void printHistory() const;
    void printHistoryForAccount(const std::string& accountId) const;

    // За извлечения
    const std::vector<Transaction>& getHistory() const { return history; }
};