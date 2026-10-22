#include <iostream>
#include <string>
#include <limits>
#include <cmath>

using namespace std;

int main() {
    int choice;
    string accountNumber, transferAccount;
    double balance = 0, amount;
    string name;

    cout << "===== WELCOME TO MINI BANK =====" << endl;

    // Create Account
    cout << "Enter your name: ";
    getline(cin, name);

    cout << "Enter account number: ";
    cin >> accountNumber;

    cout << "Enter initial deposit (Minimum 500): ";
    cin >> amount;

    if (!cin || !isfinite(amount) || amount < 500) {
        cout << "Account creation failed! Minimum deposit is 500." << endl;
        return 0;
    }

    balance = amount;

    cout << "\nAccount created successfully!" << endl;
    cout << "Welcome " << name << "!" << endl;

    // Banking Menu
    while (1) {
        cout << "\n===== MINI BANK MENU =====" << endl;
        cout << "1. Deposit Money" << endl;
        cout << "2. Withdraw Money" << endl;
        cout << "3. Transfer Money" << endl;
        cout << "4. Check Balance" << endl;
        cout << "5. Exit" << endl;

        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {

            case 1:
                cout << "Enter amount to deposit: ";
                if (!(cin >> amount)) {
                    cout << "Invalid amount!" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }

                if (isfinite(amount) && amount > 0
                    && isfinite(balance + amount)) {
                    balance += amount;
                    cout << "Deposit successful!" << endl;
                } else {
                    cout << "Invalid amount!" << endl;
                }
                break;

            case 2:
                cout << "Enter amount to withdraw: ";
                if (!(cin >> amount)) {
                    cout << "Invalid amount!" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }

                if (isfinite(amount) && amount > 0 && amount <= balance) {
                    balance -= amount;
                    cout << "Withdrawal successful!" << endl;
                } else {
                    cout << "Insufficient balance or invalid amount!" << endl;
                }
                break;

            case 3:
                cout << "Enter account number to transfer to: ";
                cin >> transferAccount;

                cout << "Enter amount to transfer: ";
                if (!(cin >> amount)) {
                    cout << "Invalid amount!" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }

                if (isfinite(amount) && amount > 0 && amount <= balance
                    && transferAccount != accountNumber) {
                    balance -= amount;
                    cout << "Transfer successful!" << endl;
                    cout << "Money sent to account "
                         << transferAccount << endl;
                } else {
                    cout << "Transfer failed!" << endl;
                }
                break;

            case 4:
                cout << "Your current balance is: "
                     << fixed << setprecision(2) << balance << endl;
                break;

            case 5:
                cout << "Thank you for using Mini Bank!" << endl;
                return 0;

            default:
                cout << "Invalid choice! Try again." << endl;
        }
    }

    return 0;
}
