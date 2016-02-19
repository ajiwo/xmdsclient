#ifndef XMDS_UTIL_H
#define XMDS_UTIL_H
#include <stddef.h>

#ifdef	__cplusplus
extern "C" {
#endif

size_t file_get_size(FILE *file);
unsigned char *file_get_contents(size_t *len, const char *path);

char *str_duplicate(const char *src);
char *str_between(const char *src, size_t *olen, const char *start, const char *end);
int str_raw2hex(char *dst, const unsigned char *src, size_t slen);
void file_md5sum(char dst[32], const char *path);
unsigned short digitlen(long n);
int str_tolower(char *src);
int str_toupper(char *src);

#ifdef	__cplusplus
}
#endif

#endif
