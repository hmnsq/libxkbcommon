/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <inttypes.h>
#ಅನ್ತರ್ಭಾವ <stdarg.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
# include <unistd.h>
#ಇಲ್ಲದಿದ್ದರೆ
/* Required on Windows where unistd.h doesn't exist */
# define R_OK    4               /* Test for read permission.  */
# define W_OK    2               /* Test for write permission.  */
# define X_OK    1               /* Test for execute permission.  */
# define F_OK    0               /* Test for existence.  */
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
# include <direct.h>
# include <io.h>
# include <BaseTsd.h>
# ifndef S_ISDIR
#  define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
# endif
# ifndef S_ISREG
#  define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
# endif
# ifndef STDIN_FILENO
#  define STDIN_FILENO 0
# endif
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _MSC_VER
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ SSIZE_T ssize_t;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_aligned(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಸೀಮಿತ ಸೂಚಕ, ಗಾತ್ರ_t byte_ಎಣಿಕೆ)
{
    ಹಿಂತಿರುಗಿ (uintptr_t) ಸೂಚಕ % byte_ಎಣಿಕೆ == 0;
}


#ವ್ಯಾಖ್ಯಾನಿಸು ARRAY_ಗಾತ್ರ(arr) ((ಗಾತ್ರ(arr) / ಗಾತ್ರ(*(arr))))

#ವ್ಯಾಖ್ಯಾನಿಸು MIN(a, b) ((a) < (b) ? (a) : (b))
#ವ್ಯಾಖ್ಯಾನಿಸು MAX(a, b) ((a) > (b) ? (a) : (b))

/* Round up @a so it's divisible by @b. */
#ವ್ಯಾಖ್ಯಾನಿಸು ROUNDUP(a, b) (((a) + (b) - 1) / (b) * (b))

#ವ್ಯಾಖ್ಯಾನಿಸು STRINGIFY(x) #x
#ವ್ಯಾಖ್ಯಾನಿಸು STRINGIFY2(x) STRINGIFY(x)
#ವ್ಯಾಖ್ಯಾನಿಸು CONCAT(x,y) x ## y
#ವ್ಯಾಖ್ಯಾನಿಸು CONCAT2(x,y) CONCAT(x,y)

/* Check if a Unicode code point is valid in a string literal */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_valid_char(uint32_t cp)
{
    /* Currently we only check for NULL character, but this could be extended
     * in the future to further ASCII control characters. */
    ಹಿಂತಿರುಗಿ cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_surrogate(uint32_t cp)
{
    ಹಿಂತಿರುಗಿ (cp >= 0xd800 && cp <= 0xdfff);
}

ಅಕ್ಷರ
ಗಮ್ಯ_ಅಧೋ(ಅಕ್ಷರ c);

ಪೂರ್ಣಾಂಕ
istrcmp(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *a, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *b);

ಪೂರ್ಣಾಂಕ
istrncmp(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *a, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *b, ಗಾತ್ರ_t n);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
streq(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s1, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s2)
{
    ಪ್ರತಿಜ್ಞಾ(s1 && s2);
    ಹಿಂತಿರುಗಿ strcmp(s1, s2) == 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
streq_null(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s1, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s2)
{
    ಆದರೆ (s1 == ಖಾಲಿ || s2 == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ s1 == s2;
    ಹಿಂತಿರುಗಿ streq(s1, s2);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
streq_not_null(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s1, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s2)
{
    ಆದರೆ (!s1 || !s2)
        ಹಿಂತಿರುಗಿ false;
    ಹಿಂತಿರುಗಿ streq(s1, s2);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
istreq(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s1, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s2)
{
    ಹಿಂತಿರುಗಿ istrcmp(s1, s2) == 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
istrneq(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s1, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s2, ಗಾತ್ರ_t len)
{
    ಹಿಂತಿರುಗಿ istrncmp(s1, s2, len) == 0;
}

#ವ್ಯಾಖ್ಯಾನಿಸು istreq_ಪೂರ್ವಪ್ರತ್ಯಯ(s1, s2) istrneq(s1, s2, ಗಾತ್ರ(s1) - 1)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಅಕ್ಷರ *
strcpy_ಸುರಕ್ಷಿತ(ಅಕ್ಷರ *dest, ಗಾತ್ರ_t ಗಾತ್ರ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *src) {
    ಆದರೆ (!dest || !ಗಾತ್ರ_2 || !src)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ ಎಲ್ಲೆ = dest + ಗಾತ್ರ_2 - 1;
    ಆಗ (dest < ಎಲ್ಲೆ && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    ಹಿಂತಿರುಗಿ *src ? ಖಾಲಿ : dest;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಅಕ್ಷರ *
strdup_ಸುರಕ್ಷಿತ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
    ಹಿಂತಿರುಗಿ s ? strdup(s) : ಖಾಲಿ;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಗಾತ್ರ_t
strlen_ಸುರಕ್ಷಿತ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
    ಹಿಂತಿರುಗಿ s ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(s) : 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
isempty(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
    ಹಿಂತಿರುಗಿ s == ಖಾಲಿ || s[0] == '\0';
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
strnull(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
    ಹಿಂತಿರುಗಿ s ? s : "(null)";
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
strempty(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
    ಹಿಂತಿರುಗಿ s ? s : "";
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ *
memdup(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *mem, ಗಾತ್ರ_t nmemb, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ಶೂನ್ಯ *p = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(nmemb, ಗಾತ್ರ_2);
    ಆದರೆ (p)
        ಸ್ಮರಣೆ_ನಕಲು(p, mem, nmemb * ಗಾತ್ರ_2);
    ಹಿಂತಿರುಗಿ p;
}

#ಆದರೆ !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಅಕ್ಷರ *
strndup(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, ಗಾತ್ರ_t n)
{
    ಗಾತ್ರ_t slen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(s);
    ಗಾತ್ರ_t len = MIN(slen, n);
    ಅಕ್ಷರ *p = ಸ್ಮರಣೆ_ಕೊಡು(len + 1);
    ಆದರೆ (!p)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    ಸ್ಮರಣೆ_ನಕಲು(p, s, len);
    p[len] = '\0';
    ಹಿಂತಿರುಗಿ p;
}
#ಆದರೆ_ಅಂತ್ಯ

/* ctype.h is locale-dependent and has other oddities. */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_ascii(ಅಕ್ಷರ ch)
{
    ಹಿಂತಿರುಗಿ (ch & ~0x7f) == 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_space(ಅಕ್ಷರ ch)
{
    ಹಿಂತಿರುಗಿ ch == ' ' || (ch >= '\t' && ch <= '\r');
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_alpha(ಅಕ್ಷರ ch)
{
    ಹಿಂತಿರುಗಿ (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_digit(ಅಕ್ಷರ ch)
{
    ಹಿಂತಿರುಗಿ ch >= '0' && ch <= '9';
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_alnum(ಅಕ್ಷರ ch)
{
    ಹಿಂತಿರುಗಿ is_alpha(ch) || is_digit(ch);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_xdigit(ಅಕ್ಷರ ch)
{
    ಹಿಂತಿರುಗಿ
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_ನಕ್ಷೆ(ಅಕ್ಷರ ch)
{
    /* See table in ascii(7). */
    ಹಿಂತಿರುಗಿ ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ
msb_pos(uint32_t ಮುಖವಾಡ)
{
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ pos = 0;
    ಆಗ (ಮುಖವಾಡ) {
        pos++;
        ಮುಖವಾಡ >>= 1u;
    }
    ಹಿಂತಿರುಗಿ pos;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
one_ಬಿಟ್_ಗಣ(uint32_t x)
{
    ಹಿಂತಿರುಗಿ x && (x & (x - 1)) == 0;
}

bool
map_file(FILE *file, ಅಕ್ಷರ **string_ಹೊರಹೋಗು, ಗಾತ್ರ_t *ಗಾತ್ರ_ಹೊರಹೋಗು);

ಶೂನ್ಯ
unmap_file(ಅಕ್ಷರ *string, ಗಾತ್ರ_t ಗಾತ್ರ_2);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
check_eaccess(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ)
{
#ಆದರೆ defined(HAVE_EACCESS)
    ಆದರೆ (eaccess(ಮಾರ್ಗ, ಬಹುಲಕ) != 0)
        ಹಿಂತಿರುಗಿ false;
#ಅಥವಾಆದರೆ defined(HAVE_EUIDACCESS)
    ಆದರೆ (euidaccess(ಮಾರ್ಗ, ಬಹುಲಕ) != 0)
        ಹಿಂತಿರುಗಿ false;
#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ true;
}

FILE*
open_file(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ);

#ಆದರೆ defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#ಅಥವಾಆದರೆ defined(HAVE___SECURE_GETENV)
# define secure_getenv __secure_getenv
#ಇಲ್ಲದಿದ್ದರೆ
# define secure_getenv getenv
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(HAVE___BUILTIN_EXPECT)
# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#ಇಲ್ಲದಿದ್ದರೆ
# define likely(x)   (x)
# define unlikely(x) (x)
#ಆದರೆ_ಅಂತ್ಯ

/* Compiler Attributes */

/* Private functions only exposed in tests. */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_ಖಾಸಗಿ_APIS
# ಆದರೆ defined(__GNUC__) && !defined(__CYGWIN__)
#  define XKB_EXPORT_ಖಾಸಗಿ __attribute__((visibility("default")))
# elif defined(_WIN32)
#  define XKB_EXPORT_ಖಾಸಗಿ __declspec(dllexport)
# ಇಲ್ಲದಿದ್ದರೆ
#  define XKB_EXPORT_ಖಾಸಗಿ
# endif
#ಇಲ್ಲದಿದ್ದರೆ
# define XKB_EXPORT_ಖಾಸಗಿ
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(__MINGW32__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__MINGW_PRINTF_FORMAT, x, y)))
#ಅಥವಾಆದರೆ defined(__GNUC__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__printf__, x, y)))
#ಇಲ್ಲದಿದ್ದರೆ
# define ATTR_PRINTF(x,y)
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(__GNUC__)
# define ATTR_NORETURN __attribute__((__noreturn__))
#ಇಲ್ಲದಿದ್ದರೆ
# define ATTR_NORETURN
#ಆದರೆ_ಅಂತ್ಯ /* GNUC  */

#ಆದರೆ defined(__GNUC__)
#ವ್ಯಾಖ್ಯಾನಿಸು ATTR_MALLOC  __attribute__((__malloc__))
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು ATTR_MALLOC
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(__GNUC__)
# define ATTR_NULL_SENTINEL __attribute__((__sentinel__))
#ಇಲ್ಲದಿದ್ದರೆ
# define ATTR_NULL_SENTINEL
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(__GNUC__)
#ವ್ಯಾಖ್ಯಾನಿಸು ATTR_PACKED  __attribute__((__packed__))
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು ATTR_PACKED
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(HAVE_ATTR_COUNTED_BY) && HAVE_ATTR_COUNTED_BY
# define ATTR_COUNTED_BY(ಎಣಿಕೆ_2) __attribute__((counted_by(ಎಣಿಕೆ_2)))
#ಇಲ್ಲದಿದ್ದರೆ
# define ATTR_COUNTED_BY(ಎಣಿಕೆ_2)
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
XKB_EXPORT_ಖಾಸಗಿ ಪೂರ್ಣಾಂಕ asprintf(ಅಕ್ಷರ **strp, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...) ATTR_PRINTF(2, 3);
# ಆದರೆ !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_ಖಾಸಗಿ ಪೂರ್ಣಾಂಕ vasprintf(ಅಕ್ಷರ **strp, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, va_list ap);
# endif /* !HAVE_VASPRINTF */
#ಆದರೆ_ಅಂತ್ಯ /* !HAVE_ASPRINTF */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ATTR_PRINTF(3, 4)
snprintf_ಸುರಕ್ಷಿತ(ಅಕ್ಷರ *buf, ಗಾತ್ರ_t sz, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format, ...)
{
    va_list ap;
    ಪೂರ್ಣಾಂಕ rc;

    va_ಆರಂಭ(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    va_ಕೊನೆ(ap);

    ಹಿಂತಿರುಗಿ rc >= 0 && (ಗಾತ್ರ_t)rc < sz;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಅಕ್ಷರ *
ATTR_PRINTF(1, 0)
vasprintf_ಸುರಕ್ಷಿತ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, va_list ವಾದಗಳು)
{
    ಅಕ್ಷರ *str;
    ಪೂರ್ಣಾಂಕ len;

    len = vasprintf(&str, fmt, ವಾದಗಳು);

    ಆದರೆ (len == -1)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ str;
}

/**
 * A version of asprintf that returns the allocated string or NULL on error.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಅಕ್ಷರ *
ATTR_PRINTF(1, 2)
asprintf_ಸುರಕ್ಷಿತ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...)
{
    va_list ವಾದಗಳು;
    ಅಕ್ಷರ *str;

    va_ಆರಂಭ(ವಾದಗಳು, fmt);
    str = vasprintf_ಸುರಕ್ಷಿತ(fmt, ವಾದಗಳು);
    va_ಕೊನೆ(ವಾದಗಳು);

    ಹಿಂತಿರುಗಿ str;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NDEBUG
    #ಆದರೆ defined(__GNUC__) || defined(__clang__)
        #ವ್ಯಾಖ್ಯಾನಿಸು PANIC_UNREACHABLE() __builtin_unreachable()
    #ಅಥವಾಆದರೆ defined(_MSC_VER)
        #ವ್ಯಾಖ್ಯಾನಿಸು PANIC_UNREACHABLE() __assume(0)
    #ಇಲ್ಲದಿದ್ದರೆ
        #ವ್ಯಾಖ್ಯಾನಿಸು PANIC_UNREACHABLE() abort()
    #ಆದರೆ_ಅಂತ್ಯ
#ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು PANIC_UNREACHABLE() \
        ಮಾಡು { \
            fprintf(stderr, "Critical Error: Reached unreachable line in %s at %d\n", __FILE__, __LINE__); \
            abort(); \
        } ಆಗ (0)
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#ಅಥವಾಆದರೆ defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#ಅಥವಾಆದರೆ defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#ಇಲ್ಲದಿದ್ದರೆ
#  define NOINLINE
#ಆದರೆ_ಅಂತ್ಯ
