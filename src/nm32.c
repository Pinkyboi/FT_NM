
#include <elf.h>
#include <fcntl.h>
#include "ft_nm.h"

static Elf32_Shdr* get_shdr_by_name_32(Elf32_Ehdr* elf_header, char *name)
{
    Elf32_Shdr* section_header;
    Elf32_Shdr* strtab_sh;

    section_header = (void *)elf_header + elf_header->e_shoff;
    strtab_sh = &section_header[elf_header->e_shstrndx];

    for (int i = 0; i < elf_header->e_shnum; ++i)
    {
        char *section_name = (void *)elf_header + strtab_sh->sh_offset + section_header[i].sh_name;
        if (!strcmp(section_name, name))
            return &section_header[i];
    }
    return NULL;
}

static Elf32_Shdr* get_shdr_by_index_62(Elf32_Ehdr* elf_header, int index)
{
    return ((Elf32_Shdr *)((void *)elf_header + elf_header->e_shoff) + index);
}

static char switch_global_62(uint32_t info, char c)
{
    if (ELF32_ST_BIND(info) == STB_GLOBAL || ELF32_ST_BIND(info) == STB_WEAK)
        return (c >= 'a' && c <= 'z') ? c - 32 : c;
    return (c >= 'A' && c <= 'Z') ? c + 32 : c;
}

static char sym_type_32(Elf32_Sym sym, Elf32_Shdr *shdr)
{
    if (shdr == NULL)
        return switch_global_62(sym.st_info, 'a');
    if (ELF32_ST_BIND(sym.st_info) == STB_WEAK)
    {
        if (sym.st_shndx == SHN_UNDEF)
            return (ELF32_ST_TYPE(sym.st_info) == STT_OBJECT ? 'v' : 'w');
        return (ELF32_ST_TYPE(sym.st_info) == STT_OBJECT ? 'V' : 'W');
    }

    if (ELF32_ST_BIND(sym.st_info) == STB_GNU_UNIQUE)
        return 'u';
    if (sym.st_shndx == SHN_UNDEF)
        return 'U';
    if (sym.st_shndx == SHN_ABS)
        return 'A';
    if (sym.st_shndx == SHN_COMMON)
        return 'C';
    if (shdr->sh_type == SHT_PROGBITS && (shdr->sh_flags & SHF_EXECINSTR) && (shdr->sh_flags & SHF_ALLOC))
        return switch_global_62(sym.st_info, 't');
    if (shdr->sh_flags & SHF_ALLOC)
    {
        if (shdr->sh_flags & SHF_WRITE)
        {
            if (shdr->sh_type == SHT_NOBITS)
                return switch_global_62(sym.st_info, 'b');
            if (shdr->sh_type == SHT_PROGBITS || shdr->sh_type == SHT_DYNAMIC)
                return switch_global_62(sym.st_info, 'd');
        }
        if (shdr->sh_type == SHT_INIT_ARRAY || shdr->sh_type == SHT_FINI_ARRAY)
            return switch_global_62(sym.st_info, 'd');
    }
    if (shdr->sh_flags & SHF_ALLOC && !(shdr->sh_flags & SHF_WRITE))
        return switch_global_62(sym.st_info, 'r');
    if (shdr->sh_type == SHT_PROGBITS)
        return (switch_global_62(sym.st_info, 'n'));
    if (shdr->sh_type == SHT_NOTE)
        return 'N';
    return '?';
}

void print_symbols_32( Elf32_Ehdr* elf_header, Elf32_Shdr* ssymtab, char *strtab, Elf32_Sym* symbol_head){
    Elf32_Sym   current_symbol;
    t_symbol*   symbols;
    size_t      sym_count;
    size_t      valid_sym_count;

    valid_sym_count = 0;
    sym_count = ssymtab->sh_size / sizeof(Elf32_Sym);
    symbols = (t_symbol *)malloc(sizeof(t_symbol) * sym_count);
    if (symbols == NULL)
        return;
    for (long unsigned int i = 0; i < sym_count; i++)
    {
        current_symbol = symbol_head[i];
        char *symbol_name = strtab + current_symbol.st_name;
        Elf32_Shdr* symbol_header = NULL;
        if (current_symbol.st_shndx < SHN_LORESERVE)
            symbol_header = get_shdr_by_index_62(elf_header, current_symbol.st_shndx);
        if (current_symbol.st_name && current_symbol.st_info != STT_FILE && symbol_name)
            symbols[valid_sym_count++] = (t_symbol){current_symbol.st_value, sym_type_32(current_symbol, symbol_header), symbol_name};
    }
    print_symbols(symbols, ADDR_SIZE_32, valid_sym_count);
    free(symbols);
}

void nm_32(void *file, char *file_name, t_bool single)
{
    Elf32_Ehdr* elf_header = file;

    Elf32_Shdr* sstrtab = get_shdr_by_name_32(elf_header, ".strtab");
    Elf32_Shdr* ssymtab = get_shdr_by_name_32(elf_header, ".symtab");
    if (!ssymtab || !sstrtab) {
        errno = NO_SYMBOL;
        return;
    }
    if (!single)
    {
        write(1, "\n", 1);
        IDENTIFIER_HEADER(file_name, strlen(file_name));
        write(1, "\n", 1);
    }
    print_symbols_32(
        elf_header,
        ssymtab,
        (void *)elf_header + sstrtab->sh_offset,
        (void *)elf_header + ssymtab->sh_offset
    );
}
