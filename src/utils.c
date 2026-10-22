/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#include "config.h"

#include <sys/stat.h>
#include <fcntl.h>
#include "utils.h"

#nếu HAVE_MMAP

#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>

bool
map_file(FILE *file, char **string_out, size_t *size_out)
{
    struct stat stat_buf;
    số_nguyên fd;
    char *string;

    /* Make sure to keep the errno on failure! */
    fd = fileno(file);
    nếu (fd < 0)
        trả_về false;

    nếu (fstat(fd, &stat_buf) != 0)
        trả_về false;

    string = mmap(NULL, (size_t)stat_buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    nếu (string == MAP_FAILED)
        trả_về false;

    *string_out = string;
    *size_out = (size_t)stat_buf.st_size;
    trả_về true;
}

void
unmap_file(char *str, size_t size)
{
    munmap(str, size);
}

#không_thì

bool
map_file(FILE *file, char **string_out, size_t *size_out)
{
    long ret;
    size_t ret_s;
    char *string;
    size_t size;

    /* Make sure to keep the errno on failure! */

    ret = fseek(file, 0, SEEK_END);
    nếu (ret != 0)
        trả_về false;

    ret = ftell(file);
    nếu (ret < 0)
        trả_về false;
    size = (size_t) ret;

    ret = fseek(file, 0, SEEK_SET);
    nếu (ret < 0)
        trả_về false;

    string = malloc(size);
    nếu (!string)
        trả_về false;

    ret_s = fread(string, 1, size, file);
    nếu (ret_s < size) {
        free(string);
        trả_về false;
    }

    *string_out = string;
    *size_out = size;
    trả_về true;
}

void
unmap_file(char *str, size_t size)
{
    free(str);
}

#endif

/* Open a file and ensure it is a regular file.
 * Returns NULL in case of error. */
FILE*
open_file(const char *path)
{
    nếu (!path)
        trả_về NULL;

    số_nguyên fd = open(path, O_RDONLY);
    nếu (fd < 0)
        trả_về NULL;

    struct stat stat_buf;
    số_nguyên err = fstat(fd, &stat_buf);

    nếu (err != 0 || !S_ISREG(stat_buf.st_mode)) {
        close(fd);
        trả_về NULL;
    }

    /* While unlikely to happen, if `fp` is NULL then we must close the file
     * descriptor. This is poorly documented and missing from the man page; see instead:
     * https://www.ibm.com/docs/en/i/7.3?topic=functions-fdopen-associates-stream-file-descriptor)
     * Confirmed by Peter to be necessary in glibc. */
    FILE *fp = fdopen(fd, "rb");
    nếu (fp == NULL) {
        close(fd);
    }

    trả_về fp;
}

// ASCII lower-case map.
static const unsigned char lower_map[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58,
    59, 60, 61, 62, 63, 64, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107,
    108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
    91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107,
    108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
    123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137,
    138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152,
    153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167,
    168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182,
    183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197,
    198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212,
    213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
    228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242,
    243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
};

// ASCII tolower (to avoid locale issues).
char
to_lower(char c)
{
    trả_về (char) lower_map[(unsigned char) c];
}

// ASCII strcasecmp (to avoid locale issues).
số_nguyên
istrcmp(const char *a, const char *b)
{
    cho (size_t i = 0; ; i++) {
        nếu (to_lower(a[i]) != to_lower(b[i]))
            trả_về (số_nguyên) to_lower(a[i]) - (số_nguyên) to_lower(b[i]);
        nếu (!a[i])
            break;
    }
    trả_về 0;
}

// ASCII strncasecmp (to avoid locale issues).
số_nguyên
istrncmp(const char *a, const char *b, size_t n)
{
    cho (size_t i = 0; i < n; i++) {
        nếu (to_lower(a[i]) != to_lower(b[i]))
            trả_về (số_nguyên) to_lower(a[i]) - (số_nguyên) to_lower(b[i]);
        nếu (!a[i])
            break;
    }
    trả_về 0;
}

#nếu !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
số_nguyên
asprintf(char **strp, const char *fmt, ...)
{
    số_nguyên ret;
    va_list ap;
    va_start(ap, fmt);
    ret = vasprintf(strp, fmt, ap);
    va_end(ap);
    trả_về ret;
}

# nếu !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
số_nguyên
vasprintf(char **strp, const char *fmt, va_list ap)
{
    số_nguyên ret;
    char *buf;
    va_list ap_copy;

    /*
     * The value of the va_list parameter is undefined after the call to
     * vsnprintf() returns: pass a copy to make sure "ap" remains valid.
     */
    va_copy(ap_copy, ap);
    ret = vsnprintf(NULL, 0, fmt, ap_copy);
    va_end(ap_copy);

    nếu (ret < 0)
        trả_về ret;

    nếu (!(buf = malloc(ret + 1)))
        trả_về -1;

    nếu ((ret = vsnprintf(buf, ret + 1, fmt, ap)) < 0) {
        free(buf);
        trả_về ret;
    }

    *strp = buf;
    trả_về ret;
}
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */
