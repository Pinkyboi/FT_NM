#include "ft_nm.h"
#include <stdio.h>

void *map_file(char *file_name)
{
    struct stat file_stat;
    void        *file;
    int         fd;

    fd = open(file_name, O_RDONLY);
    fstat(fd, &file_stat);
    if ((file = mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
        return (NULL);
    return (file);
}

char *get_section_name_by_index(char *sections_name, int section_index)
{
    for (int i = 0; i < section_index; i++)
        sections_name = sections_name + strlen(sections_name) + 1;
    return (sections_name);
}

#include <string.h>

Elf64_Shdr* get_shdr_by_name(Elf64_Ehdr* elf_header, char *name)
{
    Elf64_Shdr* section_header;
    Elf64_Shdr* strtab_sh;

    section_header = (void *)elf_header + elf_header->e_shoff;
    strtab_sh      = &section_header[elf_header->e_shstrndx];

    for (int i = 0; i < elf_header->e_shnum; ++i)
    {
        char *section_name = (void *)elf_header + strtab_sh->sh_offset + section_header[i].sh_name;
        if (!strcmp(section_name, name))
            return (&section_header[i]);
    }
    return (NULL);
}

void nm_64(void *file)
{
    Elf64_Ehdr* elf_header = file;

    Elf64_Shdr* sstrtab = get_shdr_by_name(elf_header, ".strtab");
    Elf64_Shdr* ssymtab = get_shdr_by_name(elf_header, ".symtab");

    char *strtab = (void *)elf_header + sstrtab->sh_offset;
    Elf64_Sym* symbol_head  = (void *)elf_header + ssymtab->sh_offset;
    for (int i = 0; i < ssymtab->sh_size / sizeof(Elf64_Sym); i++)
    {
        if (symbol_head[i].st_name)
        {
            if (symbol_head[i].st_value)
                printf("%016x %s\n", symbol_head[i].st_value, strtab + symbol_head[i].st_name);
            else
                printf("%16c %s\n", ' ', strtab + symbol_head[i].st_name);
        }
    }
}

int main(int argc, char **argv)
{
    void *file;

    if (argc != 2)
    {
        printf("Usage: ./ft_nm <file>\n");
        return (1);
    }
    file = map_file(argv[1]);
    if (file && *(int *)(file) == ELF_ID)
    {
        if (((unsigned char *)file)[EI_CLASS] == ELFCLASS64)
            nm_64(file);
        if (((unsigned char *)file)[EI_CLASS] == ELFCLASS32)
            printf("no 32 bit yet\n");
    }
}