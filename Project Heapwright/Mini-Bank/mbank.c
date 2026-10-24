#include <stdio.h>

struct BankAccount {
    char name[32];
    int accountNumber;
    double balance;
};

int main(void) {
    /* C has no default member initializers (that's C++-only), so we use a
     * designated initializer here instead -- closest equivalent in C. */
    struct BankAccount account = {
        .name = "Jackal",
        .accountNumber = 12345,
        .balance = 5000.0
    };

    printf("===== MEMORY EXPLORER =====\n");

    printf("\n--- MEMORY ADDRESSES ---\n");
    printf("Struct:         %p\n", (void *)&account);
    printf("Name:           %p\n", (void *)account.name);
    printf("Account Number: %p\n", (void *)&account.accountNumber);
    printf("Balance:        %p\n", (void *)&account.balance);

    printf("\n--- CHARACTER MEMORY ---\n");

    for (int i = 0; account.name[i] != '\0'; i++) {
        unsigned char byte = (unsigned char)account.name[i];

        printf("Character: %c | Decimal: %d | Hex: 0x%X | Address: %p\n",
               account.name[i],
               (int)byte,
               (int)byte,
               (void *)&account.name[i]);
    }

    printf("\n--- VARIABLE SIZES ---\n");
    printf("Name size: %zu bytes\n", sizeof(account.name));
    printf("Account number size: %zu bytes\n", sizeof(account.accountNumber));
    printf("Balance size: %zu bytes\n", sizeof(account.balance));
    printf("Entire struct size: %zu bytes\n", sizeof(account));

    return 0;
}

/*
 * BUILD:
 *   gcc -g -O0 -Wall -Wextra -o memory_explorer memory_explorer.c
 *
 * RUN:
 *   ./memory_explorer
 *
 * NOTE ON PORTING DIFFERENCES:
 *   - C++'s `cout << hex << uppercase` is stateful formatting; in C,
 *     printf's %X format specifier does the same job per-call, no
 *     persistent stream state to reset afterward (no need for the
 *     trailing `cout << dec;` equivalent).
 *   - `static_cast<void*>(...)` becomes a plain C-style cast `(void *)...`.
 *     %p expects a void*, which is why both versions cast to it explicitly.
 *   - sizeof's result type is size_t in both languages; C's printf needs
 *     the explicit %zu specifier to print it correctly (C++ streams
 *     handle size_t automatically via operator<< overloading).
 */
