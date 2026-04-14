#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
using namespace std;

struct Transaction {
    int accNo;
    string type;
    double amount;
    string time;
};

vector<Transaction> transactions;

string getTime() {
    time_t now = time(0);
    string t = ctime(&now);
    return t.substr(0, t.length() - 1);
}

class Account {
private:
    int accNo;
    string name;
    int pin;
    double balance;

public:
    Account() {}

    Account(int a, string n, int p, double b) {
        accNo = a;
        name = n;
        pin = p;
        balance = b;
    }

    int getAccNo() { return accNo; }
    int getPin() { return pin; }
    double getBalance() { return balance; }

    void deposit(double amt) {
        balance += amt;
        cout << " Amount Deposited Successfully!\n";
    }

    void withdraw(double amt) {
        if (amt > balance) {
            cout << " Insufficient Balance!\n";
        } else {
            balance -= amt;
            cout << " Withdrawal Successful!\n";
        }
    }

    void display() {
        cout << "\nAccount No: " << accNo;
        cout << "\nName: " << name;
        cout << "\nBalance: ₹" << balance << endl;
    }

    void saveAccount() {
        ofstream file("accounts.txt", ios::app);
        file << accNo << " " << name << " " << pin << " " << balance << endl;
        file.close();
    }

    static vector<Account> loadAccounts() {
        vector<Account> list;
        ifstream file("accounts.txt");

        int a, p;
        string n;
        double b;

        while (file >> a >> n >> p >> b) {
            list.push_back(Account(a, n, p, b));
        }

        file.close();
        return list;
    }
};
class FraudDetector {
public:
    static void check(int accNo, double amount, double balance) {

        if (amount > 50000) {
            cout << "FRAUD ALERT: Large Transaction!\n";
        }

        if (amount > 0.8 * balance) {
            cout << "FRAUD ALERT: Unusual Withdrawal (80% balance)!\n";
        }

        int count = 0;
        for (int i = transactions.size() - 1; i >= 0 && i >= (int)transactions.size() - 5; i--) {
            if (transactions[i].accNo == accNo)
                count++;
        }

        if (count >= 3) {
            cout << " FRAUD ALERT: Too many recent transactions!\n";
        }
    }
};

void saveTransaction(Transaction t) {
    ofstream file("transactions.txt", ios::app);
    file << t.accNo << " " << t.type << " " << t.amount << " " << t.time << endl;
    file.close();

    transactions.push_back(t);
}

int login(vector<Account> &accounts) {
    int accNo, pin;

    cout << "Enter Account No: ";
    cin >> accNo;

    cout << "Enter PIN: ";
    cin >> pin;

    for (int i = 0; i < accounts.size(); i++) {
        if (accounts[i].getAccNo() == accNo && accounts[i].getPin() == pin) {
            cout << " Login Successful!\n";
            return i;
        }
    }

    cout << " Invalid Credentials!\n";
    return -1;
}

void miniStatement(int accNo) {
    cout << "\n--- Last 5 Transactions ---\n";

    int count = 0;
    for (int i = transactions.size() - 1; i >= 0 && count < 5; i--) {
        if (transactions[i].accNo == accNo) {
            cout << transactions[i].type << " ₹" << transactions[i].amount
                 << " | " << transactions[i].time << endl;
            count++;
        }
    }

    if (count == 0)
        cout << "No transactions found.\n";
}

int main() {

    vector<Account> accounts = Account::loadAccounts();
    int choice;

    while (true) {
        cout << "\nBANK SYSTEM \n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        //REGISTER
        if (choice == 1) {
            int accNo, pin;
            string name;
            double balance;

            cout << "Enter Account No: ";
            cin >> accNo;

            cout << "Enter Name: ";
            cin >> name;

            cout << "Set PIN: ";
            cin >> pin;

            cout << "Enter Initial Balance: ";
            cin >> balance;

            Account acc(accNo, name, pin, balance);
            acc.saveAccount();

            accounts.push_back(acc);

            cout << " Account Created Successfully!\n";
        }

        //LOGIN
        else if (choice == 2) {
            int index = login(accounts);

            if (index == -1) continue;

            while (true) {
                cout << "\n1. Deposit\n2. Withdraw\n3. Check Balance\n4. Mini Statement\n5. Logout\n";
                cout << "Enter choice: ";
                cin >> choice;

                if (choice == 1) {
                    double amt;
                    cout << "Enter amount: ";
                    cin >> amt;

                    accounts[index].deposit(amt);

                    saveTransaction({accounts[index].getAccNo(), "Deposit", amt, getTime()});
                    FraudDetector::check(accounts[index].getAccNo(), amt, accounts[index].getBalance());
                }

                else if (choice == 2) {
                    double amt;
                    cout << "Enter amount: ";
                    cin >> amt;

                    accounts[index].withdraw(amt);

                    saveTransaction({accounts[index].getAccNo(), "Withdraw", amt, getTime()});
                    FraudDetector::check(accounts[index].getAccNo(), amt, accounts[index].getBalance());
                }

                else if (choice == 3) {
                    accounts[index].display();
                }

                else if (choice == 4) {
                    miniStatement(accounts[index].getAccNo());
                }

                else {
                    cout << "Logging out...\n";
                    break;
                }
            }
        }

        else {
            cout << "Exiting...\n";
            break;
        }
    }

    return 0;
}
