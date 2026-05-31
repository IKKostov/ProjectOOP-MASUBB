#include "CurrencyConverter.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

CurrencyConverter::CurrencyConverter() {
    // Фиксирани курсове спрямо BGN
    rates["BGN"] = 1.0;
    rates["EUR"] = 1.9558;   // 1 EUR = 1.9558 BGN (фиксиран курс)
    rates["USD"] = 1.81;     // приблизителен курс
    rates["GBP"] = 2.28;
    rates["CHF"] = 2.02;
}

double CurrencyConverter::convert(double amount,
                                   const std::string& from,
                                   const std::string& to) const {
    if (from == to) return amount;

    if (rates.find(from) == rates.end())
        throw std::invalid_argument("Неподдържана валута: " + from);
    if (rates.find(to) == rates.end())
        throw std::invalid_argument("Неподдържана валута: " + to);

    // Конвертираме: from -> BGN -> to
    double inBGN = amount * rates.at(from);
    return inBGN / rates.at(to);
}

void CurrencyConverter::printRates() const {
    std::cout << "  ---- Валутни курсове (спрямо BGN) ----\n";
    for (const auto& pair : rates) {
        std::cout << "  1 " << std::left << std::setw(4) << pair.first
                  << " = " << std::fixed << std::setprecision(4)
                  << pair.second << " BGN\n";
    }
}

bool CurrencyConverter::isSupported(const std::string& currency) const {
    return rates.find(currency) != rates.end();
}

void CurrencyConverter::setRate(const std::string& currency, double rateVsBGN) {
    rates[currency] = rateVsBGN;
    std::cout << "  [OK] Курсът на " << currency << " е обновен на "
              << std::fixed << std::setprecision(4) << rateVsBGN << " BGN.\n";
}