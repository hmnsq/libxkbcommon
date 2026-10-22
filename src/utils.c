/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ "utils.h"

#എങ്കിൽ HAVE_MMAP

#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <sys/mman.h>
#അന്തര്ഭാവ <sys/types.h>

bool
map_file(FILE *file, അക്ഷരം **string_out, size_t *size_out)
{
    ഘടന stat stat_buf;
    പൂർണ്ണസംഖ്യ fd;
    അക്ഷരം *string;

    /* Make sure to keep the errno on failure! */
    fd = fileno(file);
    എങ്കിൽ (fd < 0)
        തിരികെ false;

    എങ്കിൽ (fstat(fd, &stat_buf) != 0)
        തിരികെ false;

    string = mmap(ശൂന്യ, (size_t)stat_buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    എങ്കിൽ (string == MAP_FAILED)
        തിരികെ false;

    *string_out = string;
    *size_out = (size_t)stat_buf.st_size;
    തിരികെ true;
}

ശൂന്യം
unmap_file(അക്ഷരം *str, size_t size)
{
    munmap(str, size);
}

#അല്ലെങ്കിൽ

bool
map_file(FILE *file, അക്ഷരം **string_out, size_t *size_out)
{
    long ret;
    size_t ret_s;
    അക്ഷരം *string;
    size_t size;

    /* Make sure to keep the errno on failure! */

    ret = fseek(file, 0, SEEK_END);
    എങ്കിൽ (ret != 0)
        തിരികെ false;

    ret = ftell(file);
    എങ്കിൽ (ret < 0)
        തിരികെ false;
    size = (size_t) ret;

    ret = fseek(file, 0, SEEK_SET);
    എങ്കിൽ (ret < 0)
        തിരികെ false;

    string = malloc(size);
    എങ്കിൽ (!string)
        തിരികെ false;

    ret_s = fread(string, 1, size, file);
    എങ്കിൽ (ret_s < size) {
        സ്വതന്ത്രം(string);
        തിരികെ false;
    }

    *string_out = string;
    *size_out = size;
    തിരികെ true;
}

ശൂന്യം
unmap_file(അക്ഷരം *str, size_t size)
{
    സ്വതന്ത്രം(str);
}

#endif

/* Open a file and ensure it is a regular file.
 * Returns NULL in case of error. */
FILE*
open_file(സ്ഥിരം അക്ഷരം *path)
{
    എങ്കിൽ (!path)
        തിരികെ ശൂന്യ;

    പൂർണ്ണസംഖ്യ fd = open(path, O_RDONLY);
    എങ്കിൽ (fd < 0)
        തിരികെ ശൂന്യ;

    ഘടന stat stat_buf;
    പൂർണ്ണസംഖ്യ err = fstat(fd, &stat_buf);

    എങ്കിൽ (err != 0 || !S_ISREG(stat_buf.st_mode)) {
        close(fd);
        തിരികെ ശൂന്യ;
    }

    /* While unlikely to happen, if `fp` is NULL then we must close the file
     * descriptor. This is poorly documented and missing from the man page; see instead:
     * https://www.ibm.com/docs/en/i/7.3?topic=functions-fdopen-associates-stream-file-descriptor)
     * Confirmed by Peter to be necessary in glibc. */
    FILE *fp = fdopen(fd, "rb");
    എങ്കിൽ (fp == ശൂന്യ) {
        close(fd);
    }

    തിരികെ fp;
}

// ASCII lower-case map.
സ്ഥിതം സ്ഥിരം unsigned അക്ഷരം lower_map[] = {
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
അക്ഷരം
to_lower(അക്ഷരം c)
{
    തിരികെ (അക്ഷരം) lower_map[(unsigned അക്ഷരം) c];
}

// ASCII strcasecmp (to avoid locale issues).
പൂർണ്ണസംഖ്യ
istrcmp(സ്ഥിരം അക്ഷരം *a, സ്ഥിരം അക്ഷരം *b)
{
    വേണ്ടി (size_t i = 0; ; i++) {
        എങ്കിൽ (to_lower(a[i]) != to_lower(b[i]))
            തിരികെ (പൂർണ്ണസംഖ്യ) to_lower(a[i]) - (പൂർണ്ണസംഖ്യ) to_lower(b[i]);
        എങ്കിൽ (!a[i])
            നിർത്തുക;
    }
    തിരികെ 0;
}

// ASCII strncasecmp (to avoid locale issues).
പൂർണ്ണസംഖ്യ
istrncmp(സ്ഥിരം അക്ഷരം *a, സ്ഥിരം അക്ഷരം *b, size_t n)
{
    വേണ്ടി (size_t i = 0; i < n; i++) {
        എങ്കിൽ (to_lower(a[i]) != to_lower(b[i]))
            തിരികെ (പൂർണ്ണസംഖ്യ) to_lower(a[i]) - (പൂർണ്ണസംഖ്യ) to_lower(b[i]);
        എങ്കിൽ (!a[i])
            നിർത്തുക;
    }
    തിരികെ 0;
}

#എങ്കിൽ !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
പൂർണ്ണസംഖ്യ
asprintf(അക്ഷരം **strp, സ്ഥിരം അക്ഷരം *fmt, ...)
{
    പൂർണ്ണസംഖ്യ ret;
    va_list ap;
    va_start(ap, fmt);
    ret = vasprintf(strp, fmt, ap);
    va_end(ap);
    തിരികെ ret;
}

# എങ്കിൽ !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
പൂർണ്ണസംഖ്യ
vasprintf(അക്ഷരം **strp, സ്ഥിരം അക്ഷരം *fmt, va_list ap)
{
    പൂർണ്ണസംഖ്യ ret;
    അക്ഷരം *buf;
    va_list ap_copy;

    /*
     * The value of the va_list parameter is undefined after the call to
     * vsnprintf() returns: pass a copy to make sure "ap" remains valid.
     */
    va_copy(ap_copy, ap);
    ret = vsnprintf(ശൂന്യ, 0, fmt, ap_copy);
    va_end(ap_copy);

    എങ്കിൽ (ret < 0)
        തിരികെ ret;

    എങ്കിൽ (!(buf = malloc(ret + 1)))
        തിരികെ -1;

    എങ്കിൽ ((ret = vsnprintf(buf, ret + 1, fmt, ap)) < 0) {
        സ്വതന്ത്രം(buf);
        തിരികെ ret;
    }

    *strp = buf;
    തിരികെ ret;
}
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */
