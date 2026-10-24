#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define MAX_TEST_SIZE 1048576U

/*
 * Corrected packet processing implementation.
 */
static int process_packet(uint32_t payload_length)
{
    if (payload_length == 0 ||
        payload_length > MAX_TEST_SIZE) {
        fprintf(stderr,
                "[PATCHED] Invalid payload length\n");
        return 1;
    }

    /*
     * The protocol's simulated allocation field
     * supports at most UINT16_MAX bytes.
     *
     * Reject lengths that cannot be represented.
     */
    if (payload_length > UINT16_MAX) {
        fprintf(stderr,
                "[PATCHED] Rejected oversized payload: "
                "%" PRIu32 "\n",
                payload_length);
        return 1;
    }

    size_t allocation_length =
        (size_t)payload_length;

    unsigned char *source =
        malloc(allocation_length);

    if (source == NULL) {
        perror("malloc source");
        return 1;
    }

    unsigned char *destination =
        malloc(allocation_length);

    if (destination == NULL) {
        perror("malloc destination");
        free(source);
        return 1;
    }

    memset(source, 'A', allocation_length);

    /*
     * Safe: destination capacity is at least
     * the number of bytes being copied.
     */
    memcpy(
        destination,
        source,
        allocation_length
    );

    printf("[PATCHED] Accepted payload: %zu bytes\n",
           allocation_length);

    printf("[PATCHED] Copy completed safely\n");

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
