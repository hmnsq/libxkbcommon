/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#include "config.h"

#include <sys/stat.h>
#include <fcntl.h>
#include "utils.h"

#якщо HAVE_MMAP

#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>

bool
map_file(FILE *file, char **string_out, size_t *size_out)
{
    struct stat stat_buf;
    ціле fd;
    char *string;

    /* Make sure to keep the errno on failure! */
    fd = fileno(file);
    якщо (fd < 0)
        повернути false;

    якщо (fstat(fd, &stat_buf) != 0)
        повернути false;

    string = mmap(NULL, (size_t)stat_buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    якщо (string == MAP_FAILED)
        повернути false;

    *string_out = string;
    *size_out = (size_t)stat_buf.st_size;
    повернути true;
}

void
unmap_file(char *str, size_t size)
{
    munmap(str, size);
}

#інакше

bool
map_file(FILE *file, char **string_out, size_t *size_out)
{
    long ret;
    size_t ret_s;
    char *string;
    size_t size;

    /* Make sure to keep the errno on failure! */

    ret = fseek(file, 0, SEEK_END);
    якщо (ret != 0)
        повернути false;

    ret = ftell(file);
    якщо (ret < 0)
        повернути false;
    size = (size_t) ret;

    ret = fseek(file, 0, SEEK_SET);
    якщо (ret < 0)
        повернути false;

    string = malloc(size);
    якщо (!string)
        повернути false;

    ret_s = fread(string, 1, size, file);
    якщо (ret_s < size) {
        free(string);
        повернути false;
    }

    *string_out = string;
    *size_out = size;
    повернути true;
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
    якщо (!path)
        повернути NULL;

    ціле fd = open(path, O_RDONLY);
    якщо (fd < 0)
        повернути NULL;

    struct stat stat_buf;
    ціле err = fstat(fd, &stat_buf);

    якщо (err != 0 || !S_ISREG(stat_buf.st_mode)) {
        close(fd);
        повернути NULL;
    }

    /* While unlikely to happen, if `fp` is NULL then we must close the file
     * descriptor. This is poorly documented and missing from the man page; see instead:
     * https://www.ibm.com/docs/en/i/7.3?topic=functions-fdopen-associates-stream-file-descriptor)
     * Confirmed by Peter to be necessary in glibc. */
    FILE *fp = fdopen(fd, "rb");
    якщо (fp == NULL) {
        close(fd);
    }

    повернути fp;
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
    повернути (char) lower_map[(unsigned char) c];
}

// ASCII strcasecmp (to avoid locale issues).
ціле
istrcmp(const char *a, const char *b)
{
    для (size_t i = 0; ; i++) {
        якщо (to_lower(a[i]) != to_lower(b[i]))
            повернути (ціле) to_lower(a[i]) - (ціле) to_lower(b[i]);
        якщо (!a[i])
            break;
    }
    повернути 0;
}

// ASCII strncasecmp (to avoid locale issues).
ціле
istrncmp(const char *a, const char *b, size_t n)
{
    для (size_t i = 0; i < n; i++) {
        якщо (to_lower(a[i]) != to_lower(b[i]))
            повернути (ціле) to_lower(a[i]) - (ціле) to_lower(b[i]);
        якщо (!a[i])
            break;
    }
    повернути 0;
}

#якщо !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
ціле
asprintf(char **strp, const char *fmt, ...)
{
    ціле ret;
    va_list ap;
    va_start(ap, fmt);
    ret = vasprintf(strp, fmt, ap);
    va_end(ap);
    повернути ret;
}

# якщо !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
ціле
vasprintf(char **strp, const char *fmt, va_list ap)
{
    ціле ret;
    char *buf;
    va_list ap_copy;

    /*
     * The value of the va_list parameter is undefined after the call to
     * vsnprintf() returns: pass a copy to make sure "ap" remains valid.
     */
    va_copy(ap_copy, ap);
    ret = vsnprintf(NULL, 0, fmt, ap_copy);
    va_end(ap_copy);

    якщо (ret < 0)
        повернути ret;

    якщо (!(buf = malloc(ret + 1)))
        повернути -1;

    якщо ((ret = vsnprintf(buf, ret + 1, fmt, ap)) < 0) {
        free(buf);
        повернути ret;
    }

    *strp = buf;
    повернути ret;
}
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */
