#include "ft_nm.h"

void* int_to_hex_string(unsigned long long value, unsigned short byte_number) {
    const char *hex_digits = "0123456789abcdef";
    int num_chars = byte_number * 2; // Each byte is represented by 2 hex digits
    char *buffer = (char *)malloc((num_chars + 1) * sizeof(char)); // +1 for the null terminator
    if (buffer == NULL)
        return NULL;
    for (int i = 0; i < num_chars; i++)
        buffer[i] = ' ';
    buffer[num_chars] = '\0';
    if (!value)
        return buffer;
    for (int i = num_chars - 1; i >= 0; --i) {
        buffer[i] = hex_digits[value & 0xF];
        value >>= 4;
    }
    return buffer;
}

static void print_symbol(unsigned long long addr, unsigned short addr_size, const char sym_type, char *sym_name)
{
    char *addr_value = int_to_hex_string(addr, addr_size);
    write(1, addr_value, (addr_size * 2));
    write(1, " ", 1);
    write(1, &sym_type, 1);
    write(1, " ", 1);
    write(1, sym_name, strlen(sym_name));
    write(1, "\n", 1);
    free(addr_value);
}


void bubble_sort_symbols(t_symbol *symbols, size_t symbol_count) {
    for (size_t i = 0; i < symbol_count - 1; i++) {
        for (size_t j = 0; j < symbol_count - i - 1; j++) {
            if (strcmp(symbols[j].name, symbols[j + 1].name) > 0) {
                t_symbol temp = symbols[j];
                symbols[j] = symbols[j + 1];
                symbols[j + 1] = temp;
            }
        }
    }
}

void print_symbols(t_symbol *symbols, unsigned short addr_size, size_t symbol_count)
{
    bubble_sort_symbols(symbols, symbol_count);
    for (size_t i = 0; i < symbol_count; i++)
        print_symbol(symbols[i].addr, addr_size, symbols[i].type, symbols[i].name);
}