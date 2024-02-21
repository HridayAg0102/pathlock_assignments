#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <cstdlib>

using namespace std;

// Bank class for managing multiple accounts and transactions
class Bank {
public:
    // Constructor to initialize the bank with accounts and initial balances
    Bank(int numAccounts, vector<int> initialBalances)
        : accounts(numAccounts, 0), mutexes(numAccounts) {
            for (int i = 0; i < initialBalances.size(); ++i) {
                if (i < numAccounts) {
                    accounts[i] = initialBalances[i];
                }
            }
        }

    // Transfer money from one account to another
    void transfer(int from, int to, int amount) {
        lock_guard<mutex> lock1(mutexes[from]);
        lock_guard<mutex> lock2(mutexes[to]);

        if (accounts[from] >= amount) {
            accounts[from] -= amount;
            if (to < accounts.size()) {
                accounts[to] += amount;
                cout << "Transferred $" << amount << " from Account " << from + 1
                     << " to Account " << to + 1 << " successful." << endl;
            } else {
                cout << "Transferred $" << amount << " from Account " << from + 1
                     << " to External Account successful." << endl;
            }
        } else {
            cout << "Insufficient funds in Account " << from + 1 << " for transfer." << endl;
        }
    }

    // Get the total balance across all accounts
    string getTotalBalance() {
        lock_guard<mutex> lock(mutexTotal);
        int totalBalance = 0;
        int i = 1;
        for (int balance : accounts) {
            cout << "Balance in account "<< i << " Rs. "<< balance << "\n";
            i++;
            totalBalance += balance;
        }
        return "Final total balance of the bank is: Rs. " + to_string(totalBalance);
    }

private:
    vector<int> accounts; // Vector to store balances of multiple accounts
    vector<mutex> mutexes; // Vector of mutexes to ensure thread safety for each account
    mutex mutexTotal; // Mutex for accessing total balance
};

// Function to simulate transactions between accounts
void simulateTransactions(Bank& bank, int from, int to, int amount, int numTransactions) {
    for (int i = 0; i < numTransactions; ++i) {
        bank.transfer(from, to, amount);
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

// Main function to execute the program
int main() {
    // Number of accounts and initial balances
    const int numAccounts = 10; 
    vector<int> initialBalances = {1500, 2000, 1200, 1800, 2500, 3000, 3500, 4000, 4500, 5000}; // Different initial balances for each account

    // Create a Bank object
    Bank bank(numAccounts, initialBalances);

    // Number of threads for concurrent transactions
    const int numThreads = 4;
    vector<thread> threads;

    // Spawn threads to simulate transactions between different account pairs
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(simulateTransactions, ref(bank), i % numAccounts, (rand()) % (numAccounts), rand()%1000, 4);
    }

    // Wait for all threads to finish
    for (thread& thread : threads) {
        thread.join();
    }

    // Display the final total balance after all transactions
    cout << bank.getTotalBalance() << endl;

    return 0;
}
