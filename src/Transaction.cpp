#include "Transaction.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
 
int TransactionManager::txCounter = 1;
 
// ── Transaction struct helpers ──────────────────────────────────────────────
 
std::string Transaction::typeToString() const {
    switch (type) {
        case TxType::DEPOSIT:       return "Deposit";
        case TxType::WITHDRAWAL:    return "Withdrawal";
        case TxType::TRANSFER_OUT:  return "Transfer (out)";
        case TxType::TRANSFER_IN:   return "Transfer (in)";
    }
    return "Unknown";
}
 
void Transaction::print() const {
    std::cout << "  [" << txId << "] " << timestamp
              << "  " << std::left << std::setw(16) << typeToString()
              << "  " << std::right << std::fixed << std::setprecision(2)
              << std::setw(10) << amount << " " << currency;
    if (!toAccountId.empty())
        std::cout << "  -> " << toAccountId;
    if (!fromAccountId.empty() && type == TxType::TRANSFER_IN)
        std::cout << "  from " << fromAccountId;
    std::cout << "\n";
}
 
// ── TransactionManager ──────────────────────────────────────────────────────
 
std::string TransactionManager::generateId() {
    return "TX" + std::to_string(txCounter++);
}
 
std::string TransactionManager::currentTimestamp() {
    std::time_t t = std::time(nullptr);
    std::tm* tm   = std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
 
bool TransactionManager::deposit(Account* acc, double amount) {
    if (!acc) return false;
    bool ok = acc->deposit(amount);
    if (ok) {
        history.push_back({
            generateId(), TxType::DEPOSIT, amount,
            acc->getCurrency(), acc->getAccountId(), "", currentTimestamp()
        });
    }
    return ok;
}
 
bool TransactionManager::withdraw(Account* acc, double amount) {
    if (!acc) return false;
    bool ok = acc->withdraw(amount);
    if (ok) {
        history.push_back({
            generateId(), TxType::WITHDRAWAL, amount,
            acc->getCurrency(), acc->getAccountId(), "", currentTimestamp()
        });
    }
    return ok;
}
 
bool TransactionManager::transfer(Account* from, Account* to, double amount) {
    if (!from || !to) {
        std::cout << "  [!] Invalid account for transfer.\n";
        return false;
    }
    if (from->getAccountId() == to->getAccountId()) {
        std::cout << "  [!] Cannot transfer to the same account.\n";
        return false;
    }
 
    // Withdraw from source
    bool ok = from->withdraw(amount);
    if (!ok) return false;
 
    // Deposit to destination (same currency assumed; extension point for FX)
    to->deposit(amount);
 
    std::string ts = currentTimestamp();
    history.push_back({
        generateId(), TxType::TRANSFER_OUT, amount,
        from->getCurrency(), from->getAccountId(), to->getAccountId(), ts
    });
    history.push_back({
        generateId(), TxType::TRANSFER_IN, amount,
        to->getCurrency(), from->getAccountId(), to->getAccountId(), ts
    });
 
    std::cout << "  [OK] Transfer from " << from->getAccountId()
              << " to " << to->getAccountId()
              << " of " << std::fixed << std::setprecision(2)
              << amount << " " << from->getCurrency() << " was successful.\n";
    return true;
}
 
void TransactionManager::printHistory() const {
    if (history.empty()) {
        std::cout << "  No transactions found.\n";
        return;
    }
    std::cout << "  ---- Full Transaction History ----\n";
    for (const auto& tx : history)
        tx.print();
}
 
void TransactionManager::printHistoryForAccount(const std::string& accountId) const {
    bool found = false;
    std::cout << "  ---- History for Account " << accountId << " ----\n";
    for (const auto& tx : history) {
        if (tx.fromAccountId == accountId || tx.toAccountId == accountId) {
            tx.print();
            found = true;
        }
    }
    if (!found)
        std::cout << "  No transactions found for this account.\n";
}