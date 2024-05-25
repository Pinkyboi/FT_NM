#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <elf.h>
#include "ft_nm.h"


#define ELF_ID 0x464c457f

int error(int return_code, char *target, char *message)
{
    if (target)
        printf("%s", target);
    if (message)
        printf(": %s\n", message);
    else if (message)
        printf("%s\n", message);
    return return_code;
}

int open_file(char *file_name, struct stat *st)
{
    int rt;
    int fd;

    fd = open(file_name, O_RDONLY);
    if (fd == -1)
    {
        if (errno == EACCES)
            return error(-1, file_name, "Permission denied.");
        return error(-1, file_name, "No such file or directory.");
    }
    rt = fstat(fd, st);
    if (rt == -1)
        return error(-1, file_name, "Error encountered while checking file stats.");
    if (S_ISDIR(st->st_mode))
        return error(-1, file_name, "Is a directory.");
    return fd;
}


int main(int argc, char **argv)
{
    void *file;
    struct stat st;
    int fd;

    if (argc != 2)
    {
        printf("Usage: ./ft_nm <file>\n");
        return 1;
    }

    fd = open_file(argv[1], &st);
    if (fd == -1)
        return errno;
    file = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file == MAP_FAILED)
    {
        close(fd);
        return errno;
    }

    if (memcmp(file, "\x7f""ELF", 4) == 0)
    {
        if (((unsigned char *)file)[EI_CLASS] == ELFCLASS64)
            nm_64(file);
        else if (((unsigned char *)file)[EI_CLASS] == ELFCLASS32)
            nm_32(file);
    }
    else
    {
        printf("Not a valid ELF file\n");
    }

    munmap(file, st.st_size);
    close(fd);
    return 0;
}
