#include <iostream>
#include <cstring>
#include <cstdint>
#include <iomanip>

using namespace std;

// Normal banking function
void createAccount() {
    cout << "\n[+] Account created successfully!\n";
}

// Simulated transfer function
void transferMoney() {
    cout << "\n[!] TRANSFER FUNCTION EXECUTED!\n";
    cout << "[!] Simulated transfer of 5000.\n";
    cout << "[!] No actual money was transferred.\n";
}

struct BankSession {
    char name[16];
    void (*nextAction)();
};

int main() {
    BankSession session{};

    session.nextAction = createAccount;

    cout << "===== MINI BANK MEMORY LAB =====\n";

    cout << "\n--- MEMORY ADDRESSES ---\n";
    cout << "Name buffer: "
         << static_cast<void*>(session.name) << '\n';

    cout << "Function pointer variable: "
         << static_cast<void*>(&session.nextAction) << '\n';

    cout << "Buffer size: "
         << sizeof(session.name) << " bytes\n";

    cout << "Function pointer size: "
         << sizeof(session.nextAction) << " bytes\n";

    cout << "\nEnter your name: ";

    // Read at most 15 characters plus '\0'.
    cin.getline(session.name, sizeof(session.name));

    if (cin.fail()) {
        cout << "\n[!] Input exceeded the name buffer.\n";
        cout << "[!] C++ input handling prevented the overflow.\n";
        return 1;
    }

    cout << "\nHello, " << session.name << '\n';

    cout << "\nExecuting next action...\n";

    session.nextAction();

    return 0;
}
