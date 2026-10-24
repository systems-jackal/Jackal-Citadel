#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

int main(void)
{
    uint32_t original_length = 65540;

    uint16_t truncated_length =
        (uint16_t)original_length;

    printf("Original length: %" PRIu32 "\n",
           original_length);

    printf("Truncated length: %" PRIu16 "\n",
           truncated_length);

    printf("Difference: %" PRIu32 "\n",
           original_length - truncated_length);

    if (truncated_length != original_length) {
        printf("[!] Integer truncation detected\n");
    }

    return 0;
}
