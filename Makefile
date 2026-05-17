CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET   = bank.exe
 
SRCS = src/main.cpp \
       src/Account.cpp \
       src/CheckingAccount.cpp \
       src/SavingsAccount.cpp \
       src/InvestmentAccount.cpp \
       src/Client.cpp \
       src/Transaction.cpp
 
all: $(TARGET)
 
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)
 
clean:
	del /f $(TARGET)
