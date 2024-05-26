
#include <stdlib.h>

long unsigned strlen(const char *buffer)
{
    int len = -1;
    while(buffer[++len]);
    return len;
}

int		strcmp(const char *s1, const char *s2)
{
	size_t i;

	i = 0;
	while ((s1[i] == s2[i]) && (s1[i] != '\0') && (s2[i] != '\0'))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int		memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i = 0;
	while (++i < n)
	{
		if (((unsigned char *)s1)[i] != ((unsigned char *)s2)[i])
			return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
	}
	return (0);
}

// void swap_pointers(void **p1, void **p2) {
//     void *temp = *p1;
//     *p1 = *p2;
//     *p2 = temp;
// }

