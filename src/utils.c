/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ "utils.h"

#ಆದರೆ HAVE_MMAP

#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <sys/mman.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

bool
map_file(FILE *file, ಅಕ್ಷರ **string_ಹೊರಹೋಗು, ಗಾತ್ರ_t *ಗಾತ್ರ_ಹೊರಹೋಗು)
{
    ರಚನೆ stat stat_buf;
    ಪೂರ್ಣಾಂಕ fd;
    ಅಕ್ಷರ *string;

    /* Make sure to keep the errno on failure! */
    fd = fileno(file);
    ಆದರೆ (fd < 0)
        ಹಿಂತಿರುಗಿ false;

    ಆದರೆ (fstat(fd, &stat_buf) != 0)
        ಹಿಂತಿರುಗಿ false;

    string = mmap(ಖಾಲಿ, (ಗಾತ್ರ_t)stat_buf.st_ಗಾತ್ರ, PROT_READ, MAP_SHARED, fd, 0);
    ಆದರೆ (string == MAP_FAILED)
        ಹಿಂತಿರುಗಿ false;

    *string_ಹೊರಹೋಗು = string;
    *ಗಾತ್ರ_ಹೊರಹೋಗು = (ಗಾತ್ರ_t)stat_buf.st_ಗಾತ್ರ;
    ಹಿಂತಿರುಗಿ true;
}

ಶೂನ್ಯ
unmap_file(ಅಕ್ಷರ *str, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    munmap(str, ಗಾತ್ರ_2);
}

#ಇಲ್ಲದಿದ್ದರೆ

bool
map_file(FILE *file, ಅಕ್ಷರ **string_ಹೊರಹೋಗು, ಗಾತ್ರ_t *ಗಾತ್ರ_ಹೊರಹೋಗು)
{
    ದೀರ್ಘ ret;
    ಗಾತ್ರ_t ret_s;
    ಅಕ್ಷರ *string;
    ಗಾತ್ರ_t ಗಾತ್ರ_2;

    /* Make sure to keep the errno on failure! */

    ret = fseek(file, 0, SEEK_END);
    ಆದರೆ (ret != 0)
        ಹಿಂತಿರುಗಿ false;

    ret = ftell(file);
    ಆದರೆ (ret < 0)
        ಹಿಂತಿರುಗಿ false;
    ಗಾತ್ರ_2 = (ಗಾತ್ರ_t) ret;

    ret = fseek(file, 0, SEEK_SET);
    ಆದರೆ (ret < 0)
        ಹಿಂತಿರುಗಿ false;

    string = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ_2);
    ಆದರೆ (!string)
        ಹಿಂತಿರುಗಿ false;

    ret_s = fread(string, 1, ಗಾತ್ರ_2, file);
    ಆದರೆ (ret_s < ಗಾತ್ರ_2) {
        ಬಿಡುಗಡೆ(string);
        ಹಿಂತಿರುಗಿ false;
    }

    *string_ಹೊರಹೋಗು = string;
    *ಗಾತ್ರ_ಹೊರಹೋಗು = ಗಾತ್ರ_2;
    ಹಿಂತಿರುಗಿ true;
}

ಶೂನ್ಯ
unmap_file(ಅಕ್ಷರ *str, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ಬಿಡುಗಡೆ(str);
}

#ಆದರೆ_ಅಂತ್ಯ

/* Open a file and ensure it is a regular file.
 * Returns NULL in case of error. */
FILE*
open_file(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
    ಆದರೆ (!ಮಾರ್ಗ)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಪೂರ್ಣಾಂಕ fd = open(ಮಾರ್ಗ, O_RDONLY);
    ಆದರೆ (fd < 0)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ರಚನೆ stat stat_buf;
    ಪೂರ್ಣಾಂಕ err = fstat(fd, &stat_buf);

    ಆದರೆ (err != 0 || !S_ISREG(stat_buf.st_ಬಹುಲಕ)) {
        close(fd);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    /* While unlikely to happen, if `fp` is NULL then we must close the file
     * descriptor. This is poorly documented and missing from the man page; see instead:
     * https://www.ibm.com/docs/en/i/7.3?topic=functions-fdopen-associates-stream-file-descriptor)
     * Confirmed by Peter to be necessary in glibc. */
    FILE *fp = fdopen(fd, "rb");
    ಆದರೆ (fp == ಖಾಲಿ) {
        close(fd);
    }

    ಹಿಂತಿರುಗಿ fp;
}

// ASCII lower-case map.
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಅಧೋ_map[] = {
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
ಅಕ್ಷರ
ಗಮ್ಯ_ಅಧೋ(ಅಕ್ಷರ c)
{
    ಹಿಂತಿರುಗಿ (ಅಕ್ಷರ) ಅಧೋ_map[(ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) c];
}

// ASCII strcasecmp (to avoid locale issues).
ಪೂರ್ಣಾಂಕ
istrcmp(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *a, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *b)
{
    ಗಾಗಿ (ಗಾತ್ರ_t i = 0; ; i++) {
        ಆದರೆ (ಗಮ್ಯ_ಅಧೋ(a[i]) != ಗಮ್ಯ_ಅಧೋ(b[i]))
            ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) ಗಮ್ಯ_ಅಧೋ(a[i]) - (ಪೂರ್ಣಾಂಕ) ಗಮ್ಯ_ಅಧೋ(b[i]);
        ಆದರೆ (!a[i])
            ನಿಲ್ಲಿಸು;
    }
    ಹಿಂತಿರುಗಿ 0;
}

// ASCII strncasecmp (to avoid locale issues).
ಪೂರ್ಣಾಂಕ
istrncmp(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *a, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *b, ಗಾತ್ರ_t n)
{
    ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < n; i++) {
        ಆದರೆ (ಗಮ್ಯ_ಅಧೋ(a[i]) != ಗಮ್ಯ_ಅಧೋ(b[i]))
            ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) ಗಮ್ಯ_ಅಧೋ(a[i]) - (ಪೂರ್ಣಾಂಕ) ಗಮ್ಯ_ಅಧೋ(b[i]);
        ಆದರೆ (!a[i])
            ನಿಲ್ಲಿಸು;
    }
    ಹಿಂತಿರುಗಿ 0;
}

#ಆದರೆ !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
ಪೂರ್ಣಾಂಕ
asprintf(ಅಕ್ಷರ **strp, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...)
{
    ಪೂರ್ಣಾಂಕ ret;
    va_list ap;
    va_ಆರಂಭ(ap, fmt);
    ret = vasprintf(strp, fmt, ap);
    va_ಕೊನೆ(ap);
    ಹಿಂತಿರುಗಿ ret;
}

# ಆದರೆ !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
ಪೂರ್ಣಾಂಕ
vasprintf(ಅಕ್ಷರ **strp, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, va_list ap)
{
    ಪೂರ್ಣಾಂಕ ret;
    ಅಕ್ಷರ *buf;
    va_list ap_copy;

    /*
     * The value of the va_list parameter is undefined after the call to
     * vsnprintf() returns: pass a copy to make sure "ap" remains valid.
     */
    va_copy(ap_copy, ap);
    ret = vsnprintf(ಖಾಲಿ, 0, fmt, ap_copy);
    va_ಕೊನೆ(ap_copy);

    ಆದರೆ (ret < 0)
        ಹಿಂತಿರುಗಿ ret;

    ಆದರೆ (!(buf = ಸ್ಮರಣೆ_ಕೊಡು(ret + 1)))
        ಹಿಂತಿರುಗಿ -1;

    ಆದರೆ ((ret = vsnprintf(buf, ret + 1, fmt, ap)) < 0) {
        ಬಿಡುಗಡೆ(buf);
        ಹಿಂತಿರುಗಿ ret;
    }

    *strp = buf;
    ಹಿಂತಿರುಗಿ ret;
}
# endif /* !HAVE_VASPRINTF */
#ಆದರೆ_ಅಂತ್ಯ /* !HAVE_ASPRINTF */
