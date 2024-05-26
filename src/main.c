#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <elf.h>
#include "ft_nm.h"


#define ELF_ID 0x464c457f

int open_file(char *file_name, struct stat *st) {
    int rt;
    int fd;

    fd = open(file_name, O_RDONLY);
    if (fd == -1)
        return -1;
    rt = fstat(fd, st);
    if (rt == -1)
        errno = EIO;
    if (S_ISDIR(st->st_mode))
        errno = EISDIR;
    return fd;
}

void handle_errors(char *file_name, t_bool single)
{
    if (errno == EINVAL)
        return;
    if (errno != NO_SYMBOL && errno != NOT_ELF && errno != EISDIR)
        perror((single) ? file_name : "");
    else
    {
        IDENTIFIER_HEADER(file_name, strlen(file_name));
        if (errno == NO_SYMBOL)
            write(1, "no symbols\n", 11);
        if (errno == NOT_ELF)
            write(1, "file format not recognized\n", 27);
    }
}

static void launch_nm(char *file_name, t_bool single)
{
    struct stat st;
    int fd;
    void *file;

    fd = open_file(file_name, &st);
    if (errno)
        return;
    file = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file == MAP_FAILED){
        close(fd);
        return;
    }
    if (memcmp(file, ELF_FORMAT, 4) == 0)
    {
        if (((unsigned char *)file)[EI_CLASS] == ELFCLASS64)
            nm_64(file, file_name, single);
        else if (((unsigned char *)file)[EI_CLASS] == ELFCLASS32)
            nm_32(file, file_name, single);
    }
    else
        errno = NOT_ELF;
    munmap(file, st.st_size);
    close(fd);    
}

int main(int argc, char **argv)
{
    char *nm_file;

    if (argc < 3)
    {
        nm_file = (argc == 1) ? DEFAULT_NM_FILE : argv[1];
        launch_nm(nm_file, TRUE);
        if (errno)
        {
            IDENTIFIER_HEADER(argv[0], strlen(argv[0]));
            handle_errors(nm_file, TRUE);
        }
    }
    else{
        for (int i = 1; i < argc; i++)
        {   
            launch_nm(argv[i], FALSE);
            if (errno)
            {
                IDENTIFIER_HEADER(argv[0], strlen(argv[0]));
                handle_errors(argv[i], FALSE);
            }
        }
    }
    return 0;
}
