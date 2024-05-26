#ifndef __FT_NM_H__
# define __FT_NM_H__

#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#define ELF_FORMAT      "\x7f""ELF"
#define NULL            ((void *)0)
#define ADDR_SIZE_64    8
#define ADDR_SIZE_32    4
#define DEFAULT_NM_FILE "./a.out"

#define IDENTIFIER_HEADER(exec_name, len) {\
                        write(1, exec_name, len);\
                        write(1, ": ", 2);\
                    }

#define NOT_ELF 1000
#define NO_SYMBOL 1001
#define TRUE 1
#define FALSE 0

typedef unsigned char t_bool;
typedef struct s_symbol {
    unsigned long long  addr;
    char                type;
    char*               name;
}               t_symbol;

void            nm_64(void *file, char *file_name, t_bool single);
void            nm_32(void *file, char *file_name, t_bool single);
void            print_symbols(t_symbol *symbols, unsigned short addr_size, size_t symbol_count);
int             strcmp(const char *s1, const char *s2);
int		        memcmp(const void *s1, const void *s2, size_t n);
long unsigned   strlen(const char *buffer);

#endif