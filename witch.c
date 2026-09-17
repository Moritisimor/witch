#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define CHUNK_SIZE 8

void print_byte_array_hex(uint8_t *byte_array, size_t size, size_t offset) {
    printf("0x%010zx: ", offset);
    for (size_t i = 0; i < size; i++) 
        printf("0x%02x ", byte_array[i]);

    printf("|");
    for (size_t i = 0; i < size; i++) {
        if (byte_array[i] < 32 || byte_array[i] > 126)
            printf(".");
        else
            printf("%c", byte_array[i]);
    }

    printf("|\n");
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Wrong amount of args!\nUse --help for help.");
        return 1;
    }

    if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
        printf("Usage: witch <file>\nhttps://github.com/Moritisimor/witch\n");
        return 0;
    }

    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        fprintf(stderr, "Could not read %s (check existence and permissions)", argv[1]);
        return 1;
    }

    uint8_t buf[CHUNK_SIZE];
    size_t bytes_read = 0;
    size_t current_offset = 0;

    while ((bytes_read = fread(buf, 1, CHUNK_SIZE, f)) > 0) {
        print_byte_array_hex(buf, bytes_read, current_offset);
        current_offset += CHUNK_SIZE;
    }

    fclose(f);
    return 0;
}
