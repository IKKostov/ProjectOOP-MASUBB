#include "Bank.h"
#include "SavingsAccount.h"
#include "InvestmentAccount.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

int Bank::clientCounter = 1;

static std::string nowString() {
    std::time_t t  = std::time(nullptr);
    std::tm*    tm = std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

Bank::Bank(const std::string& n, double threshold)
    : name(n), notificationThreshold(threshold) {}

Bank::~Bank() {
    for (Client* c : clients) delete c;
}

void Bank::printWelcome() const {
    std::cout << "\n"
              << "  ╔══════════════════════════════════════════╗\n"
              << "  ║        MASUBB Банкова система            ║\n"
              << "  ║   Managing A System Used By Banks        ║\n"
              << "  ╚══════════════════════════════════════════╝\n"
              << "  Банка: " << name << "\n\n";
}

// ── Функционалност 1: Клиенти ────────────────────────────────────────────────

Client* Bank::registerClient(const std::string& clientName,
                              const std::string& egn) {
    // Проверка за дублиращо ЕГН
    if (findClientByEGN(egn)) {
        std::cout << "  [!] Клиент с ЕГН " << egn << " вече е регистриран.\n";
        return nullptr;
    }
    std::string id = "CLI" + std::to_string(clientCounter++);
    Client* c = new Client(id, clientName, egn);
    clients.push_back(c);
    std::cout << "  [OK] Клиент \"" << clientName
              << "\" регистриран с ID: " << id << "\n";
    return c;
}

Client* Bank::findClientById(const std::string& id) const {
    for (Client* c : clients)
        if (c->getClientId() == id) return c;
    return nullptr;
}

Client* Bank::findClientByEGN(const std::string& egn) const {
    for (Client* c : clients)
        if (c->getEGN() == egn) return c;
    return nullptr;
}

void Bank::listAllClients() const {
    if (clients.empty()) {
        std::cout << "  Няма регистрирани клиенти.\n";
        return;
    }
    std::cout << "  ---- Регистрирани клиенти (" << clients.size() << ") ----\n";
    for (Client* c : clients) {
        c->printInfo();
        c->listAccounts();
    }
}

// ── Функционалност 2: Сметки ─────────────────────────────────────────────────

Account* Bank::openAccount(const std::string& clientId, AccountType type,
                            double initialBalance, const std::string& currency,
                            double extraParam) {
    Client* c = findClientById(clientId);
    if (!c) {
        std::cout << "  [!] Клиент с ID " << clientId << " не беше намерен.\n";
        return nullptr;
    }
    if (!converter.isSupported(currency)) {
        std::cout << "  [!] Валутата " << currency << " не се поддържа.\n";
        return nullptr;
    }
    return c->openAccount(type, initialBalance, currency, extraParam);
}

bool Bank::blockAccount(const std::string& clientId,
                         const std::string& accountId) {
    Client* c = findClientById(clientId);
    if (!c) { std::cout << "  [!] Клиент не беше намерен.\n"; return false; }
    bool ok = c->blockAccount(accountId);
    if (ok) notifManager.add(NotificationType::ACCOUNT_BLOCKED, accountId,
                              "Сметката беше блокирана.");
    return ok;
}

bool Bank::unblockAccount(const std::string& clientId,
                           const std::string& accountId) {
    Client* c = findClientById(clientId);
    if (!c) { std::cout << "  [!] Клиент не беше намерен.\n"; return false; }
    return c->unblockAccount(accountId);
}

bool Bank::closeAccount(const std::string& clientId,
                         const std::string& accountId) {
    Client* c = findClientById(clientId);
    if (!c) { std::cout << "  [!] Клиент не беше намерен.\n"; return false; }
    bool ok = c->closeAccount(accountId);
    if (ok) notifManager.add(NotificationType::ACCOUNT_CLOSED, accountId,
                              "Сметката беше закрита.");
    return ok;
}

// ── Функционалност 3: Депозит / Теглене ─────────────────────────────────────

bool Bank::deposit(const std::string& accountId, double amount) {
    Account* acc = findAccountGlobal(accountId);
    if (!acc) { std::cout << "  [!] Сметка не беше намерена.\n"; return false; }
    bool ok = txManager.deposit(acc, amount);
    if (ok) notifManager.checkAndNotify(NotificationType::LARGE_DEPOSIT,
                                         accountId, amount,
                                         notificationThreshold);
    return ok;
}

bool Bank::withdraw(const std::string& accountId, double amount) {
    Account* acc = findAccountGlobal(accountId);
    if (!acc) { std::cout << "  [!] Сметка не беше намерена.\n"; return false; }
    bool ok = txManager.withdraw(acc, amount);
    if (ok) notifManager.checkAndNotify(NotificationType::LARGE_WITHDRAWAL,
                                         accountId, amount,
                                         notificationThreshold);
    return ok;
}

// ── Функционалност 4: Преводи ─────────────────────────────────────────────────

bool Bank::transfer(const std::string& fromId, const std::string& toId,
                    double amount) {
    Account* from = findAccountGlobal(fromId);
    Account* to   = findAccountGlobal(toId);

    if (!from || !to) {
        std::cout << "  [!] Една или двете сметки не бяха намерени.\n";
        return false;
    }

    // Ако валутите са различни — конвертираме
    double convertedAmount = amount;
    if (from->getCurrency() != to->getCurrency()) {
        try {
            convertedAmount = converter.convert(amount,
                                                 from->getCurrency(),
                                                 to->getCurrency());
            std::cout << "  [ВАЛУТА] " << std::fixed << std::setprecision(2)
                      << amount << " " << from->getCurrency()
                      << " = " << convertedAmount << " " << to->getCurrency() << "\n";
        } catch (const std::exception& e) {
            std::cout << "  [!] Грешка при конвертация: " << e.what() << "\n";
            return false;
        }
    }

    bool ok = txManager.transfer(from, to, amount, convertedAmount);
    if (ok) notifManager.checkAndNotify(NotificationType::LARGE_TRANSFER,
                                         fromId, amount, notificationThreshold);
    return ok;
}

// ── Функционалност 5: История ─────────────────────────────────────────────────

void Bank::printAllTransactions() const {
    txManager.printHistory();
}

void Bank::printTransactionsForAccount(const std::string& accountId) const {
    txManager.printHistoryForAccount(accountId);
}

// ── Функционалност 6: Лихва ───────────────────────────────────────────────────

void Bank::applyInterest(const std::string& accountId) {
    Account* acc = findAccountGlobal(accountId);
    if (!acc) { std::cout << "  [!] Сметка не беше намерена.\n"; return; }

    if (SavingsAccount* sa = dynamic_cast<SavingsAccount*>(acc)) {
        double before = sa->getBalance();
        sa->applyInterest();
        double interest = sa->getBalance() - before;
        notifManager.add(NotificationType::INTEREST_APPLIED, accountId,
                         "Начислена лихва: " + std::to_string(interest)
                         + " " + sa->getCurrency());
    } else if (InvestmentAccount* ia = dynamic_cast<InvestmentAccount*>(acc)) {
        ia->applyInterest();
        notifManager.add(NotificationType::INTEREST_APPLIED, accountId,
                         "Начислена годишна доходност.");
    } else {
        std::cout << "  [!] Тази сметка не поддържа лихвено начисляване.\n";
    }
}

// ── Функционалност 7: Извлечение ──────────────────────────────────────────────

void Bank::printStatement(const std::string& accountId) const {
    Account* acc = findAccountGlobal(accountId);
    if (!acc) { std::cout << "  [!] Сметка не беше намерена.\n"; return; }

    std::string owner = getOwnerName(accountId);
    Statement stmt(accountId, owner, acc->getBalance());
    stmt.setPeriod("(начало)", nowString());

    // Вземаме транзакциите за тази сметка от мениджъра
    const auto& txList = txManager.getHistory();
    for (const auto& tx : txList) {
        if (tx.fromAccountId == accountId || tx.toAccountId == accountId) {
            StatementEntry e;
            e.txId      = tx.txId;
            e.timestamp = tx.timestamp;
            e.typeStr   = tx.typeToString();
            e.amount    = tx.amount;
            e.currency  = tx.currency;
            e.counterpart = (tx.toAccountId != accountId)
                            ? tx.toAccountId : tx.fromAccountId;
            stmt.addEntry(e);
        }
    }
    stmt.setClosingBalance(acc->getBalance());
    stmt.print();
}

// ── Функционалност 9: Известия ────────────────────────────────────────────────

void Bank::printNotifications() const       { notifManager.printAll();      }
void Bank::printUnreadNotifications() const { notifManager.printUnread();   }
void Bank::markAllNotificationsSeen()       { notifManager.markAllSeen();   }

// ── Функционалност 10: Валути ─────────────────────────────────────────────────

void Bank::printCurrencyRates() const {
    converter.printRates();
}

void Bank::setCurrencyRate(const std::string& currency, double rate) {
    converter.setRate(currency, rate);
}

double Bank::convertCurrency(double amount,
                              const std::string& from,
                              const std::string& to) const {
    try {
        return converter.convert(amount, from, to);
    } catch (const std::exception& e) {
        std::cout << "  [!] " << e.what() << "\n";
        return -1;
    }
}

// ── Помощни ───────────────────────────────────────────────────────────────────

Account* Bank::findAccountGlobal(const std::string& accountId) const {
    for (Client* c : clients) {
        Account* acc = c->findAccount(accountId);
        if (acc) return acc;
    }
    return nullptr;
}

std::string Bank::getOwnerName(const std::string& accountId) const {
    for (Client* c : clients)
        if (c->findAccount(accountId)) return c->getName();
    return "Неизвестен";
}