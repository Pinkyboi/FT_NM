#ifndef __FT_NM_H__
# define __FT_NM_H__

#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#define ELF_ID 0x464c457f

void nm_64(void *file);
void nm_32(void *file);

#endif