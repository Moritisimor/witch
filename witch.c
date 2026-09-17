#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define CHUNK_SIZE 8

void print_byte_array_hex(uint8_t *byte_array, size_t size, size_t offset) 
{
    printf("0x%010zx: ", offset);
    for (size_t i = 0; i < size; i++) 
        printf("0x%02x ", byte_array[i]);

    printf("|");
    for (size_t i = 0; i < size; i++) 
    {
        if (byte_array[i] < 32 || byte_array[i] > 126)
            printf(".");
        else
            printf("%c", byte_array[i]);
    }

    printf("|\n");
}

void print_byte_array_dec(uint8_t *byte_array, size_t size, size_t offset) 
{
    printf("%010ld: ", offset);
    for (size_t i = 0; i < size; i++) 
        printf("%03d ", byte_array[i]);

    printf("|");
    for (size_t i = 0; i < size; i++) 
    {
        if (byte_array[i] < 32 || byte_array[i] > 126)
            printf(".");
        else
            printf("%c", byte_array[i]);
    }

    printf("|\n");
}

void print_byte_array_bin(uint8_t *byte_array, size_t size, size_t offset) 
{
    printf("%016zb: ", offset);
    for (size_t i = 0; i < size; i++) 
        printf("%08b ", byte_array[i]);

    printf("|");
    for (size_t i = 0; i < size; i++) 
    {
        if (byte_array[i] < 32 || byte_array[i] > 126)
            printf(".");
        else
            printf("%c", byte_array[i]);
    }

    printf("|\n");
}

int main(int argc, char **argv) 
{
    if (argc < 2) 
    {
        fprintf(stderr, "Wrong amount of args!\nUse --help for help.");
        return 1;
    }

    bool hex = true;
    bool decimal = false;
    bool binary = false;

    for (int i = 1; i < argc; i++) 
    {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) 
        {
            printf("Usage: witch <flags...> <file>\n");
            printf("Flags:\n");
            printf("\t-x | --hex\tPrints bytes and offsets as hexadecimal numbers (Default)\n");
            printf("\t-b | --binary\tPrints bytes and offsets as binary numbers\n");
            printf("\t-d | --decimal\tPrints bytes and offsets as decimal numbers\n");
            printf("\t-h | --help\tPrints this\n");

            return 0;
        }
        else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--decimal")) 
        {
            hex = false; 
            binary = false; 
            decimal = true;
        }
        else if (!strcmp(argv[i], "-b") || !strcmp(argv[i], "--binary")) 
        {
            hex = false; 
            decimal = false; 
            binary = true;
        }
        else if (!strcmp(argv[i], "-x") || !strcmp(argv[i], "--hex")) 
        {
            binary = false;
            decimal = false;
            hex = true;
        }
        else 
        {
            if (i == argc - 1)
                break;

            printf("Unknown flag '%s'", argv[i]);
            return 1;
        }
    }

    FILE *f = fopen(argv[argc - 1], "r");
    if (f == NULL) 
    {
        fprintf(stderr, "Could not read %s (check existence and permissions)", argv[1]);
        return 1;
    }

    uint8_t buf[CHUNK_SIZE];
    size_t bytes_read = 0;
    size_t current_offset = 0;

    while ((bytes_read = fread(buf, 1, CHUNK_SIZE, f)) > 0) 
    {
        if (binary)
            print_byte_array_bin(buf, bytes_read, current_offset);
        else if (decimal)
            print_byte_array_dec(buf, bytes_read, current_offset);
        else
            print_byte_array_hex(buf, bytes_read, current_offset);

        current_offset += CHUNK_SIZE;
    }

    fclose(f);
    return 0;
}
