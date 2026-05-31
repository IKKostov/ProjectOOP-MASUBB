#include "Transaction.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

int TransactionManager::txCounter = 1;

// ── Transaction struct ───────────────────────────────────────────────────────

std::string Transaction::typeToString() const {
    switch (type) {
        case TxType::DEPOSIT:       return "Депозит";
        case TxType::WITHDRAWAL:    return "Теглене";
        case TxType::TRANSFER_OUT:  return "Превод (изх.)";
        case TxType::TRANSFER_IN:   return "Превод (вх.)";
    }
    return "Неизвестен";
}

void Transaction::print() const {
    std::cout << "  [" << txId << "] " << timestamp
              << "  " << std::left  << std::setw(16) << typeToString()
              << "  " << std::right << std::fixed << std::setprecision(2)
              << std::setw(10) << amount << " " << currency;
    if (!toAccountId.empty())
        std::cout << "  -> " << toAccountId;
    if (!fromAccountId.empty() && type == TxType::TRANSFER_IN)
        std::cout << "  от " << fromAccountId;
    std::cout << "\n";
}

// ── TransactionManager ───────────────────────────────────────────────────────

std::string TransactionManager::generateId() {
    return "TX" + std::to_string(txCounter++);
}

std::string TransactionManager::currentTimestamp() {
    std::time_t t  = std::time(nullptr);
    std::tm*    tm = std::localtime(&t);
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

bool TransactionManager::transfer(Account* from, Account* to,
                                   double amount, double convertedAmount) {
    if (!from || !to) {
        std::cout << "  [!] Невалидна сметка за превод.\n";
        return false;
    }
    if (from->getAccountId() == to->getAccountId()) {
        std::cout << "  [!] Не може да превеждате към същата сметка.\n";
        return false;
    }

    bool ok = from->withdraw(amount);
    if (!ok) return false;

    to->deposit(convertedAmount);

    std::string ts = currentTimestamp();
    history.push_back({
        generateId(), TxType::TRANSFER_OUT, amount,
        from->getCurrency(), from->getAccountId(), to->getAccountId(), ts
    });
    history.push_back({
        generateId(), TxType::TRANSFER_IN, convertedAmount,
        to->getCurrency(), from->getAccountId(), to->getAccountId(), ts
    });

    std::cout << "  [OK] Преводът е успешен: "
              << from->getAccountId() << " -> " << to->getAccountId() << "\n";
    return true;
}

void TransactionManager::printHistory() const {
    if (history.empty()) {
        std::cout << "  Няма записани транзакции.\n";
        return;
    }
    std::cout << "  ---- Пълна история на транзакциите ----\n";
    for (const auto& tx : history) tx.print();
}

void TransactionManager::printHistoryForAccount(const std::string& accountId) const {
    bool found = false;
    std::cout << "  ---- История за сметка " << accountId << " ----\n";
    for (const auto& tx : history) {
        if (tx.fromAccountId == accountId || tx.toAccountId == accountId) {
            tx.print();
            found = true;
        }
    }
    if (!found)
        std::cout << "  Няма транзакции за тази сметка.\n";
}