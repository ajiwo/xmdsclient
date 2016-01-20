#ifndef XMDS_UTIL_H
#define XMDS_UTIL_H
#include <stddef.h>

char *str_duplicate(const char *src);
char *str_between(const char *src, size_t *olen, const char *start, const char *end);
int str_raw2hex(char *dst, const unsigned char *src, size_t slen);
void file_md5sum(char dst[32], const char *path);

#endif
