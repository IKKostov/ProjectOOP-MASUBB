#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include "Bank.h"

// ── Helpers ──────────────────────────────────────────────────────────────────

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int readInt(const std::string& prompt) {
    int val;
    std::cout << prompt;
    while (!(std::cin >> val)) { clearInput(); std::cout << prompt; }
    clearInput();
    return val;
}

double readDouble(const std::string& prompt) {
    double val;
    std::cout << prompt;
    while (!(std::cin >> val)) { clearInput(); std::cout << prompt; }
    clearInput();
    return val;
}

std::string readString(const std::string& prompt) {
    std::string val;
    std::cout << prompt;
    std::getline(std::cin, val);
    return val;
}

// ── Меню 1: Клиенти ──────────────────────────────────────────────────────────

void menuClients(Bank& bank) {
    int choice;
    do {
        std::cout << "\n=== УПРАВЛЕНИЕ НА КЛИЕНТИ ===\n"
                  << "1. Регистрирай нов клиент\n"
                  << "2. Покажи всички клиенти\n"
                  << "3. Търси клиент по ID\n"
                  << "4. Търси клиент по ЕГН\n"
                  << "0. Назад\n";
        choice = readInt("Избор: ");

        if (choice == 1) {
            std::string name = readString("Три имена: ");
            std::string egn  = readString("ЕГН: ");
            bank.registerClient(name, egn);

        } else if (choice == 2) {
            bank.listAllClients();

        } else if (choice == 3) {
            std::string id = readString("ID на клиент: ");
            Client* c = bank.findClientById(id);
            if (!c) { std::cout << "  [!] Клиент не беше намерен.\n"; continue; }
            c->printInfo();
            c->listAccounts();

        } else if (choice == 4) {
            std::string egn = readString("ЕГН: ");
            Client* c = bank.findClientByEGN(egn);
            if (!c) { std::cout << "  [!] Клиент не беше намерен.\n"; continue; }
            c->printInfo();
            c->listAccounts();
        }
    } while (choice != 0);
}

// ── Меню 2: Сметки ───────────────────────────────────────────────────────────

void menuAccounts(Bank& bank) {
    int choice;
    do {
        std::cout << "\n=== УПРАВЛЕНИЕ НА СМЕТКИ ===\n"
                  << "1. Отвори сметка\n"
                  << "2. Блокирай сметка\n"
                  << "3. Активирай сметка\n"
                  << "4. Закрий сметка\n"
                  << "0. Назад\n";
        choice = readInt("Избор: ");

        if (choice == 1) {
            std::string cid = readString("ID на клиент: ");
            std::cout << "Тип: 1=Разплащателна  2=Спестовна  3=Инвестиционна\n";
            int type    = readInt("Тип: ");
            double init = readDouble("Начален баланс: ");
            std::string cur = readString("Валута (BGN/EUR/USD/GBP/CHF): ");
            double extra = 0;
            if (type == 1) extra = readDouble("Овърдрафт лимит (0 = без): ");
            if (type == 2) extra = readDouble("Лихвен процент (напр. 0.04 за 4%): ");
            if (type == 3) extra = readDouble("Очаквана доходност (напр. 0.07 за 7%): ");
            AccountType at = (type == 1) ? AccountType::CHECKING
                           : (type == 2) ? AccountType::SAVINGS
                                         : AccountType::INVESTMENT;
            bank.openAccount(cid, at, init, cur, extra);

        } else if (choice == 2) {
            std::string cid = readString("ID на клиент: ");
            std::string aid = readString("ID на сметка: ");
            bank.blockAccount(cid, aid);

        } else if (choice == 3) {
            std::string cid = readString("ID на клиент: ");
            std::string aid = readString("ID на сметка: ");
            bank.unblockAccount(cid, aid);

        } else if (choice == 4) {
            std::string cid = readString("ID на клиент: ");
            std::string aid = readString("ID на сметка: ");
            bank.closeAccount(cid, aid);
        }
    } while (choice != 0);
}

// ── Меню 3: Транзакции ───────────────────────────────────────────────────────

void menuTransactions(Bank& bank) {
    int choice;
    do {
        std::cout << "\n=== ТРАНЗАКЦИИ ===\n"
                  << "1. Депозит\n"
                  << "2. Теглене\n"
                  << "3. Превод между сметки\n"
                  << "4. Пълна история на транзакциите\n"
                  << "5. История по конкретна сметка\n"
                  << "0. Назад\n";
        choice = readInt("Избор: ");

        if (choice == 1) {
            std::string aid = readString("ID на сметка: ");
            double amt = readDouble("Сума: ");
            bank.deposit(aid, amt);

        } else if (choice == 2) {
            std::string aid = readString("ID на сметка: ");
            double amt = readDouble("Сума: ");
            bank.withdraw(aid, amt);

        } else if (choice == 3) {
            std::string from = readString("От сметка ID: ");
            std::string to   = readString("Към сметка ID: ");
            double amt = readDouble("Сума (в оригинална валута): ");
            bank.transfer(from, to, amt);

        } else if (choice == 4) {
            bank.printAllTransactions();

        } else if (choice == 5) {
            std::string aid = readString("ID на сметка: ");
            bank.printTransactionsForAccount(aid);
        }
    } while (choice != 0);
}

// ── Меню 4: Лихви и доходност ────────────────────────────────────────────────

void menuInterest(Bank& bank) {
    int choice;
    do {
        std::cout << "\n=== ЛИХВИ И ДОХОДНОСТ ===\n"
                  << "1. Начисли лихва / доходност към сметка\n"
                  << "2. Прогноза за доходност (инвестиционна сметка)\n"
                  << "0. Назад\n";
        choice = readInt("Избор: ");

        if (choice == 1) {
            std::string aid = readString("ID на сметка (спестовна или инвестиционна): ");
            bank.applyInterest(aid);

        } else if (choice == 2) {
            std::string aid = readString("ID на инвестиционна сметка: ");
            Account* acc = bank.findAccountGlobal(aid);
            if (!acc) { std::cout << "  [!] Сметка не беше намерена.\n"; continue; }
            InvestmentAccount* ia = dynamic_cast<InvestmentAccount*>(acc);
            if (!ia) { std::cout << "  [!] Сметката не е инвестиционна.\n"; continue; }
            int years = readInt("Брой години: ");
            double returns = ia->calcReturns(years);
            std::cout << "  Прогнозна доходност за " << years << " год.: "
                      << std::fixed << std::setprecision(2)
                      << returns << " " << ia->getCurrency() << "\n";
            std::cout << "  Очакван краен баланс: "
                      << (ia->getBalance() + returns) << " " << ia->getCurrency() << "\n";
        }
    } while (choice != 0);
}

// ── Меню 5: Известия ─────────────────────────────────────────────────────────

void menuNotifications(Bank& bank) {
    int choice;
    do {
        std::cout << "\n=== ИЗВЕСТИЯ ===\n"
                  << "1. Покажи всички известия\n"
                  << "2. Покажи само непрочетените\n"
                  << "3. Маркирай всички като прочетени\n"
                  << "0. Назад\n";
        choice = readInt("Избор: ");

        if      (choice == 1) bank.printNotifications();
        else if (choice == 2) bank.printUnreadNotifications();
        else if (choice == 3) bank.markAllNotificationsSeen();
    } while (choice != 0);
}

// ── Меню 6: Валути ───────────────────────────────────────────────────────────

void menuCurrency(Bank& bank) {
    int choice;
    do {
        std::cout << "\n=== ВАЛУТИ ===\n"
                  << "1. Покажи валутни курсове\n"
                  << "2. Конвертирай сума\n"
                  << "3. Обнови валутен курс\n"
                  << "0. Назад\n";
        choice = readInt("Избор: ");

        if (choice == 1) {
            bank.printCurrencyRates();

        } else if (choice == 2) {
            double amt    = readDouble("Сума: ");
            std::string f = readString("От валута (BGN/EUR/USD/GBP/CHF): ");
            std::string t = readString("Към валута: ");
            double result = bank.convertCurrency(amt, f, t);
            if (result >= 0)
                std::cout << "  " << std::fixed << std::setprecision(2)
                          << amt << " " << f << " = " << result << " " << t << "\n";

        } else if (choice == 3) {
            std::string cur = readString("Валута: ");
            double rate = readDouble("Нов курс спрямо BGN: ");
            bank.setCurrencyRate(cur, rate);
        }
    } while (choice != 0);
}

// ── Меню 7: Извлечение ────────────────────────────────────────────────────────

void menuStatement(Bank& bank) {
    std::string aid = readString("\nID на сметка за извлечение: ");
    bank.printStatement(aid);
}

// ── Main ──────────────────────────────────────────────────────────────────────

int main() {
    Bank bank("MASUBB Банка");
    bank.printWelcome();

    int choice;
    do {
        // Показваме брой непрочетени при главното меню
        std::cout << "\n=== ГЛАВНО МЕНЮ ===\n"
                  << "1. Управление на клиенти\n"
                  << "2. Управление на сметки\n"
                  << "3. Транзакции\n"
                  << "4. Лихви и доходност\n"
                  << "5. Известия\n"
                  << "6. Валути\n"
                  << "7. Банково извлечение\n"
                  << "0. Изход\n";
        choice = readInt("Избор: ");

        switch (choice) {
            case 1: menuClients(bank);       break;
            case 2: menuAccounts(bank);      break;
            case 3: menuTransactions(bank);  break;
            case 4: menuInterest(bank);      break;
            case 5: menuNotifications(bank); break;
            case 6: menuCurrency(bank);      break;
            case 7: menuStatement(bank);     break;
            case 0: std::cout << "\n  Довиждане!\n"; break;
            default: std::cout << "  [!] Невалиден избор.\n";
        }
    } while (choice != 0);

    return 0;
}