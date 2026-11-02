#include <stdio.h>

/* Each gate takes inputs of 0 or 1 and returns 0 or 1 */

int AND(int a, int b)  { return a & b; }
int OR(int a, int b)   { return a | b; }
int XOR(int a, int b)  { return a ^ b; }
int NOT(int a)         { return !a; }          /* use ! for a single bit */
int NAND(int a, int b) { return NOT(AND(a, b)); }
int NOR(int a, int b)  { return NOT(OR(a, b)); }
int XNOR(int a, int b) { return NOT(XOR(a, b)); }

void print_table(const char *name, int (*gate)(int, int)) {
    printf("\n%s\n", name);
    printf(" A B | Out\n");
    printf("-----+----\n");
    for (int a = 0; a <= 1; a++) {
        for (int b = 0; b <= 1; b++) {
            printf(" %d %d |  %d\n", a, b, gate(a, b));
        }
    }
}

int main(void) {
    print_table("AND",  AND);
    print_table("OR",   OR);
    print_table("XOR",  XOR);
    print_table("NAND", NAND);
    print_table("NOR",  NOR);
    print_table("XNOR", XNOR);

    printf("\nNOT\n");
    printf(" A | Out\n");
    printf("---+----\n");
    for (int a = 0; a <= 1; a++) {
        printf(" %d |  %d\n", a, NOT(a));
    }

    return 0;
}
