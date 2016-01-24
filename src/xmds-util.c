#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "xmds-util.h"
#include "mbedtls-md5.h"


char *str_duplicate(const char *src) {
    size_t len;
    char *dst;

    for(len = 0; src[len] != '\0'; len++);

    dst = malloc(len + 1);
    dst[len] = '\0';
    return memcpy(dst, src, len);
}

char *str_between(const char *src, size_t *olen, const char *start, const char *end) {
    char *opening, *closing;
    char *dst;
    int start_found, end_found;
    size_t start_offset, end_offset;

    size_t output_len;
    size_t iter;

    start_found = 0;
    end_found = 0;
    start_offset = 0;
    end_offset = 0;
    opening = NULL;
    closing = NULL;
    dst = NULL;


    opening = strstr(src, start);
    if(opening != NULL) {
        start_found = 1;
        opening = opening + strlen(start);
        start_offset = opening - src;
    }
    closing = strstr(src, end);
    if(closing != NULL) {
        end_found = 1;
        end_offset = closing - src;
    }
    if(!start_found || !end_found) return NULL;

    output_len = end_offset - start_offset;
    dst = malloc(output_len + 1);

    for(iter = 0; iter < output_len; iter++)
        dst[iter] = src[start_offset + iter];

    dst[output_len] = '\0';
    *olen = output_len;

    return dst;
}

int str_raw2hex(char *dst, const unsigned char *src, size_t slen) {
    size_t i;
    const char *hexmap = "0123456789abcdef";

    if(slen) {
        for(i = 0; i < slen; i++) {
            dst[ i * 2 ] = hexmap[src[i] >> 4];
            dst[ i * 2 + 1] = hexmap[src[i] & 0x0F];
        }
        dst[ i * 2 ] = '\0';
    }
    return slen;
}

void file_md5sum(char dst[32], const char *path) {
    FILE *file;
    size_t len;
    unsigned char *data;
    unsigned char raw_md5sum[16];

    memset(dst, '\0', sizeof(char));
    file = fopen(path, "r");
    if(file) {
        /* get file size for malloc arg */
        fseek(file, 0, SEEK_END);
        len = ftell(file);
        fseek(file, 0, SEEK_SET);

        data = (unsigned char *) malloc(len);

        if(fread(data, 1, len, file) == len) {
            mbedtls_md5(data, len, raw_md5sum);
            str_raw2hex(dst, raw_md5sum, 16);
        }
        free(data);
        fclose(file);
    }

}

unsigned short digitlen(long n) {
    short len = 1;

    if(n < 0) {
        n = -n;
        len++;
    }

    while((n /= 10) > 0) len++;

    return len;
}
