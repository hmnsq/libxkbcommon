/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

#ਜੇ HAVE_MMAP

#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/mman.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

bool
map_file(FILE *file, ਅੱਖਰ **string_ਬਾਹਰਲਾ, ਮਾਪ_t *ਮਾਪ_ਬਾਹਰਲਾ)
{
    ਬਣਤਰ stat stat_buf;
    ਪੂਰਨ fd;
    ਅੱਖਰ *string;

    /* Make sure to keep the errno on failure! */
    fd = fileno(file);
    ਜੇ (fd < 0)
        ਵਾਪਸ false;

    ਜੇ (fstat(fd, &stat_buf) != 0)
        ਵਾਪਸ false;

    string = mmap(ਨਿਰਾ, (ਮਾਪ_t)stat_buf.st_ਮਾਪ, PROT_READ, MAP_SHARED, fd, 0);
    ਜੇ (string == MAP_FAILED)
        ਵਾਪਸ false;

    *string_ਬਾਹਰਲਾ = string;
    *ਮਾਪ_ਬਾਹਰਲਾ = (ਮਾਪ_t)stat_buf.st_ਮਾਪ;
    ਵਾਪਸ true;
}

ਖਾਲੀ
unmap_file(ਅੱਖਰ *str, ਮਾਪ_t ਮਾਪ)
{
    munmap(str, ਮਾਪ);
}

#ਨਹੀਂਤਾਂ

bool
map_file(FILE *file, ਅੱਖਰ **string_ਬਾਹਰਲਾ, ਮਾਪ_t *ਮਾਪ_ਬਾਹਰਲਾ)
{
    ਲੰਮਾ ret;
    ਮਾਪ_t ret_s;
    ਅੱਖਰ *string;
    ਮਾਪ_t ਮਾਪ;

    /* Make sure to keep the errno on failure! */

    ret = fseek(file, 0, SEEK_END);
    ਜੇ (ret != 0)
        ਵਾਪਸ false;

    ret = ftell(file);
    ਜੇ (ret < 0)
        ਵਾਪਸ false;
    ਮਾਪ = (ਮਾਪ_t) ret;

    ret = fseek(file, 0, SEEK_SET);
    ਜੇ (ret < 0)
        ਵਾਪਸ false;

    string = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਮਾਪ);
    ਜੇ (!string)
        ਵਾਪਸ false;

    ret_s = fread(string, 1, ਮਾਪ, file);
    ਜੇ (ret_s < ਮਾਪ) {
        ਮੁਕਤ(string);
        ਵਾਪਸ false;
    }

    *string_ਬਾਹਰਲਾ = string;
    *ਮਾਪ_ਬਾਹਰਲਾ = ਮਾਪ;
    ਵਾਪਸ true;
}

ਖਾਲੀ
unmap_file(ਅੱਖਰ *str, ਮਾਪ_t ਮਾਪ)
{
    ਮੁਕਤ(str);
}

#ਜੇ_ਅੰਤ

/* Open a file and ensure it is a regular file.
 * Returns NULL in case of error. */
FILE*
open_file(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
    ਜੇ (!ਰਸਤਾ)
        ਵਾਪਸ ਨਿਰਾ;

    ਪੂਰਨ fd = open(ਰਸਤਾ, O_RDONLY);
    ਜੇ (fd < 0)
        ਵਾਪਸ ਨਿਰਾ;

    ਬਣਤਰ stat stat_buf;
    ਪੂਰਨ err = fstat(fd, &stat_buf);

    ਜੇ (err != 0 || !S_ISREG(stat_buf.st_ਬਹੁਲਕ)) {
        close(fd);
        ਵਾਪਸ ਨਿਰਾ;
    }

    /* While unlikely to happen, if `fp` is NULL then we must close the file
     * descriptor. This is poorly documented and missing from the man page; see instead:
     * https://www.ibm.com/docs/en/i/7.3?topic=functions-fdopen-associates-stream-file-descriptor)
     * Confirmed by Peter to be necessary in glibc. */
    FILE *fp = fdopen(fd, "rb");
    ਜੇ (fp == ਨਿਰਾ) {
        close(fd);
    }

    ਵਾਪਸ fp;
}

// ASCII lower-case map.
ਸਥਿਰ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਹੇਠਲਾ_map[] = {
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
ਅੱਖਰ
ਮੰਜ਼ਿਲ_ਹੇਠਲਾ(ਅੱਖਰ c)
{
    ਵਾਪਸ (ਅੱਖਰ) ਹੇਠਲਾ_map[(ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) c];
}

// ASCII strcasecmp (to avoid locale issues).
ਪੂਰਨ
istrcmp(ਅਚਲ ਅੱਖਰ *a, ਅਚਲ ਅੱਖਰ *b)
{
    ਲਈ (ਮਾਪ_t i = 0; ; i++) {
        ਜੇ (ਮੰਜ਼ਿਲ_ਹੇਠਲਾ(a[i]) != ਮੰਜ਼ਿਲ_ਹੇਠਲਾ(b[i]))
            ਵਾਪਸ (ਪੂਰਨ) ਮੰਜ਼ਿਲ_ਹੇਠਲਾ(a[i]) - (ਪੂਰਨ) ਮੰਜ਼ਿਲ_ਹੇਠਲਾ(b[i]);
        ਜੇ (!a[i])
            ਰੋਕੋ;
    }
    ਵਾਪਸ 0;
}

// ASCII strncasecmp (to avoid locale issues).
ਪੂਰਨ
istrncmp(ਅਚਲ ਅੱਖਰ *a, ਅਚਲ ਅੱਖਰ *b, ਮਾਪ_t n)
{
    ਲਈ (ਮਾਪ_t i = 0; i < n; i++) {
        ਜੇ (ਮੰਜ਼ਿਲ_ਹੇਠਲਾ(a[i]) != ਮੰਜ਼ਿਲ_ਹੇਠਲਾ(b[i]))
            ਵਾਪਸ (ਪੂਰਨ) ਮੰਜ਼ਿਲ_ਹੇਠਲਾ(a[i]) - (ਪੂਰਨ) ਮੰਜ਼ਿਲ_ਹੇਠਲਾ(b[i]);
        ਜੇ (!a[i])
            ਰੋਕੋ;
    }
    ਵਾਪਸ 0;
}

#ਜੇ !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
ਪੂਰਨ
asprintf(ਅੱਖਰ **strp, ਅਚਲ ਅੱਖਰ *fmt, ...)
{
    ਪੂਰਨ ret;
    va_list ap;
    va_ਸ਼ੁਰੂ(ap, fmt);
    ret = vasprintf(strp, fmt, ap);
    va_ਸਿਰਾ(ap);
    ਵਾਪਸ ret;
}

# ਜੇ !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
ਪੂਰਨ
vasprintf(ਅੱਖਰ **strp, ਅਚਲ ਅੱਖਰ *fmt, va_list ap)
{
    ਪੂਰਨ ret;
    ਅੱਖਰ *buf;
    va_list ap_copy;

    /*
     * The value of the va_list parameter is undefined after the call to
     * vsnprintf() returns: pass a copy to make sure "ap" remains valid.
     */
    va_copy(ap_copy, ap);
    ret = vsnprintf(ਨਿਰਾ, 0, fmt, ap_copy);
    va_ਸਿਰਾ(ap_copy);

    ਜੇ (ret < 0)
        ਵਾਪਸ ret;

    ਜੇ (!(buf = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ret + 1)))
        ਵਾਪਸ -1;

    ਜੇ ((ret = vsnprintf(buf, ret + 1, fmt, ap)) < 0) {
        ਮੁਕਤ(buf);
        ਵਾਪਸ ret;
    }

    *strp = buf;
    ਵਾਪਸ ret;
}
# endif /* !HAVE_VASPRINTF */
#ਜੇ_ਅੰਤ /* !HAVE_ASPRINTF */
