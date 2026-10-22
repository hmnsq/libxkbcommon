/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ "utils.h"

#ಯದಿ HAVE_MMAP

#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <sys/mman.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

bool
map_file(FILE *file, ವರ್ಣ **string_out, size_t *size_out)
{
    ಸಂರಚನಾ stat stat_buf;
    ಪೂರ್ಣಾಙ್ಕ fd;
    ವರ್ಣ *string;

    /* Make sure to keep the errno on failure! */
    fd = fileno(file);
    ಯದಿ (fd < 0)
        ಪ್ರತ್ಯಾಗಮ false;

    ಯದಿ (fstat(fd, &stat_buf) != 0)
        ಪ್ರತ್ಯಾಗಮ false;

    string = mmap(ರಿಕ್ತ, (size_t)stat_buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    ಯದಿ (string == MAP_FAILED)
        ಪ್ರತ್ಯಾಗಮ false;

    *string_out = string;
    *size_out = (size_t)stat_buf.st_size;
    ಪ್ರತ್ಯಾಗಮ true;
}

ಶೂನ್ಯ
unmap_file(ವರ್ಣ *str, size_t size)
{
    munmap(str, size);
}

#ಅನ್ಯಥಾ

bool
map_file(FILE *file, ವರ್ಣ **string_out, size_t *size_out)
{
    long ret;
    size_t ret_s;
    ವರ್ಣ *string;
    size_t size;

    /* Make sure to keep the errno on failure! */

    ret = fseek(file, 0, SEEK_END);
    ಯದಿ (ret != 0)
        ಪ್ರತ್ಯಾಗಮ false;

    ret = ftell(file);
    ಯದಿ (ret < 0)
        ಪ್ರತ್ಯಾಗಮ false;
    size = (size_t) ret;

    ret = fseek(file, 0, SEEK_SET);
    ಯದಿ (ret < 0)
        ಪ್ರತ್ಯಾಗಮ false;

    string = malloc(size);
    ಯದಿ (!string)
        ಪ್ರತ್ಯಾಗಮ false;

    ret_s = fread(string, 1, size, file);
    ಯದಿ (ret_s < size) {
        ಮುಕ್ತ(string);
        ಪ್ರತ್ಯಾಗಮ false;
    }

    *string_out = string;
    *size_out = size;
    ಪ್ರತ್ಯಾಗಮ true;
}

ಶೂನ್ಯ
unmap_file(ವರ್ಣ *str, size_t size)
{
    ಮುಕ್ತ(str);
}

#endif

/* Open a file and ensure it is a regular file.
 * Returns NULL in case of error. */
FILE*
open_file(ಧ್ರುವ ವರ್ಣ *path)
{
    ಯದಿ (!path)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಪೂರ್ಣಾಙ್ಕ fd = open(path, O_RDONLY);
    ಯದಿ (fd < 0)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಸಂರಚನಾ stat stat_buf;
    ಪೂರ್ಣಾಙ್ಕ err = fstat(fd, &stat_buf);

    ಯದಿ (err != 0 || !S_ISREG(stat_buf.st_mode)) {
        close(fd);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    /* While unlikely to happen, if `fp` is NULL then we must close the file
     * descriptor. This is poorly documented and missing from the man page; see instead:
     * https://www.ibm.com/docs/en/i/7.3?topic=functions-fdopen-associates-stream-file-descriptor)
     * Confirmed by Peter to be necessary in glibc. */
    FILE *fp = fdopen(fd, "rb");
    ಯದಿ (fp == ರಿಕ್ತ) {
        close(fd);
    }

    ಪ್ರತ್ಯಾಗಮ fp;
}

// ASCII lower-case map.
ಸ್ಥಿರ ಧ್ರುವ unsigned ವರ್ಣ lower_map[] = {
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
ವರ್ಣ
to_lower(ವರ್ಣ c)
{
    ಪ್ರತ್ಯಾಗಮ (ವರ್ಣ) lower_map[(unsigned ವರ್ಣ) c];
}

// ASCII strcasecmp (to avoid locale issues).
ಪೂರ್ಣಾಙ್ಕ
istrcmp(ಧ್ರುವ ವರ್ಣ *a, ಧ್ರುವ ವರ್ಣ *b)
{
    ಪ್ರತಿ (size_t i = 0; ; i++) {
        ಯದಿ (to_lower(a[i]) != to_lower(b[i]))
            ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) to_lower(a[i]) - (ಪೂರ್ಣಾಙ್ಕ) to_lower(b[i]);
        ಯದಿ (!a[i])
            ಭಙ್ಗ;
    }
    ಪ್ರತ್ಯಾಗಮ 0;
}

// ASCII strncasecmp (to avoid locale issues).
ಪೂರ್ಣಾಙ್ಕ
istrncmp(ಧ್ರುವ ವರ್ಣ *a, ಧ್ರುವ ವರ್ಣ *b, size_t n)
{
    ಪ್ರತಿ (size_t i = 0; i < n; i++) {
        ಯದಿ (to_lower(a[i]) != to_lower(b[i]))
            ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) to_lower(a[i]) - (ಪೂರ್ಣಾಙ್ಕ) to_lower(b[i]);
        ಯದಿ (!a[i])
            ಭಙ್ಗ;
    }
    ಪ್ರತ್ಯಾಗಮ 0;
}

#ಯದಿ !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
ಪೂರ್ಣಾಙ್ಕ
asprintf(ವರ್ಣ **strp, ಧ್ರುವ ವರ್ಣ *fmt, ...)
{
    ಪೂರ್ಣಾಙ್ಕ ret;
    va_list ap;
    va_start(ap, fmt);
    ret = vasprintf(strp, fmt, ap);
    va_end(ap);
    ಪ್ರತ್ಯಾಗಮ ret;
}

# ಯದಿ !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
ಪೂರ್ಣಾಙ್ಕ
vasprintf(ವರ್ಣ **strp, ಧ್ರುವ ವರ್ಣ *fmt, va_list ap)
{
    ಪೂರ್ಣಾಙ್ಕ ret;
    ವರ್ಣ *buf;
    va_list ap_copy;

    /*
     * The value of the va_list parameter is undefined after the call to
     * vsnprintf() returns: pass a copy to make sure "ap" remains valid.
     */
    va_copy(ap_copy, ap);
    ret = vsnprintf(ರಿಕ್ತ, 0, fmt, ap_copy);
    va_end(ap_copy);

    ಯದಿ (ret < 0)
        ಪ್ರತ್ಯಾಗಮ ret;

    ಯದಿ (!(buf = malloc(ret + 1)))
        ಪ್ರತ್ಯಾಗಮ -1;

    ಯದಿ ((ret = vsnprintf(buf, ret + 1, fmt, ap)) < 0) {
        ಮುಕ್ತ(buf);
        ಪ್ರತ್ಯಾಗಮ ret;
    }

    *strp = buf;
    ಪ್ರತ್ಯಾಗಮ ret;
}
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */
