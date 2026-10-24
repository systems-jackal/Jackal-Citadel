#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define MAX_TEST_SIZE 1048576U

/*
 * Deliberately vulnerable demonstration.
 *
 * This is a synthetic packet parser, not AnyDesk code.
 */
static int process_packet(uint32_t payload_length)
{
    if (payload_length == 0 ||
        payload_length > MAX_TEST_SIZE) {
        fprintf(stderr, "Invalid test length\n");
        return 1;
    }

    unsigned char *source = malloc(payload_length);

    if (source == NULL) {
        perror("malloc source");
        return 1;
    }

    memset(source, 'A', payload_length);

    /*
     * BUG: Truncation from uint32_t to uint16_t.
     */
    uint16_t allocation_length =
        (uint16_t)payload_length;

    /*
     * Allocate a smaller buffer than required.
     * Allocate at least one byte for deterministic behavior.
     */
    size_t capacity = allocation_length ?
        allocation_length : 1;

    unsigned char *destination = malloc(capacity);

    if (destination == NULL) {
        perror("malloc destination");
        free(source);
        return 1;
    }

    printf("[VULNERABLE] Declared length: %" PRIu32 "\n",
           payload_length);

    printf("[VULNERABLE] Allocation length: %zu\n",
           capacity);

    printf("[VULNERABLE] Copy length: %" PRIu32 "\n",
           payload_length);

    fflush(stdout);

    /*
     * VULNERABILITY:
     *
     * The copy uses the original 32-bit length,
     * but the destination was allocated using
     * the truncated 16-bit length.
     */
    memcpy(destination, source, payload_length);

    printf("[VULNERABLE] Copy completed\n");

    free(destination);
    free(source);

    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr,
                "Usage: %s <payload_length>\n",
                argv[0]);
        return 2;
    }

    char *end = NULL;

    unsigned long value = strtoul(argv[1], &end, 10);

    if (*argv[1] == '\0' ||
        *end != '\0' ||
        value > UINT32_MAX) {
        fprintf(stderr, "Invalid length argument\n");
        return 2;
    }

    return process_packet((uint32_t)value);
}
