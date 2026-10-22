/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <inttypes.h>
#ಅನ್ತರ್ಭಾವ <stdarg.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
# include <unistd.h>
#ಅನ್ಯಥಾ
/* Required on Windows where unistd.h doesn't exist */
# define R_OK    4               /* Test for read permission.  */
# define W_OK    2               /* Test for write permission.  */
# define X_OK    1               /* Test for execute permission.  */
# define F_OK    0               /* Test for existence.  */
#endif

#ifdef _WIN32
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
#ifdef _MSC_VER
ಪ್ರಕಾರನಿರ್ದೇಶ SSIZE_T ssize_t;
#endif
#endif

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
is_aligned(ಧ್ರುವ ಶೂನ್ಯ *restrict pointer, size_t byte_ಎಣಿಕೆ)
{
    ಪ್ರತ್ಯಾಗಮ (uintptr_t) pointer % byte_ಎಣಿಕೆ == 0;
}


#define ARRAY_SIZE(arr) ((ಪರಿಮಾಣ(arr) / ಪರಿಮಾಣ(*(arr))))

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* Round up @a so it's divisible by @b. */
#define ROUNDUP(a, b) (((a) + (b) - 1) / (b) * (b))

#define STRINGIFY(x) #x
#define STRINGIFY2(x) STRINGIFY(x)
#define CONCAT(x,y) x ## y
#define CONCAT2(x,y) CONCAT(x,y)

/* Check if a Unicode code point is valid in a string literal */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
is_valid_char(uint32_t cp)
{
    /* Currently we only check for NULL character, but this could be extended
     * in the future to further ASCII control characters. */
    ಪ್ರತ್ಯಾಗಮ cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
is_surrogate(uint32_t cp)
{
    ಪ್ರತ್ಯಾಗಮ (cp >= 0xd800 && cp <= 0xdfff);
}

ವರ್ಣ
to_lower(ವರ್ಣ c);

ಪೂರ್ಣಾಙ್ಕ
istrcmp(ಧ್ರುವ ವರ್ಣ *a, ಧ್ರುವ ವರ್ಣ *b);

ಪೂರ್ಣಾಙ್ಕ
istrncmp(ಧ್ರುವ ವರ್ಣ *a, ಧ್ರುವ ವರ್ಣ *b, size_t n);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
streq(ಧ್ರುವ ವರ್ಣ *s1, ಧ್ರುವ ವರ್ಣ *s2)
{
    ಪ್ರತಿಜ್ಞಾ(s1 && s2);
    ಪ್ರತ್ಯಾಗಮ strcmp(s1, s2) == 0;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
streq_null(ಧ್ರುವ ವರ್ಣ *s1, ಧ್ರುವ ವರ್ಣ *s2)
{
    ಯದಿ (s1 == ರಿಕ್ತ || s2 == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ s1 == s2;
    ಪ್ರತ್ಯಾಗಮ streq(s1, s2);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
streq_not_null(ಧ್ರುವ ವರ್ಣ *s1, ಧ್ರುವ ವರ್ಣ *s2)
{
    ಯದಿ (!s1 || !s2)
        ಪ್ರತ್ಯಾಗಮ false;
    ಪ್ರತ್ಯಾಗಮ streq(s1, s2);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
istreq(ಧ್ರುವ ವರ್ಣ *s1, ಧ್ರುವ ವರ್ಣ *s2)
{
    ಪ್ರತ್ಯಾಗಮ istrcmp(s1, s2) == 0;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
istrneq(ಧ್ರುವ ವರ್ಣ *s1, ಧ್ರುವ ವರ್ಣ *s2, size_t len)
{
    ಪ್ರತ್ಯಾಗಮ istrncmp(s1, s2, len) == 0;
}

#define istreq_prefix(s1, s2) istrneq(s1, s2, ಪರಿಮಾಣ(s1) - 1)

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ವರ್ಣ *
strcpy_safe(ವರ್ಣ *dest, size_t size, ಧ್ರುವ ವರ್ಣ *src) {
    ಯದಿ (!dest || !size || !src)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ limit = dest + size - 1;
    ಯಾವತ್ (dest < limit && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    ಪ್ರತ್ಯಾಗಮ *src ? ರಿಕ್ತ : dest;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ವರ್ಣ *
strdup_safe(ಧ್ರುವ ವರ್ಣ *s)
{
    ಪ್ರತ್ಯಾಗಮ s ? strdup(s) : ರಿಕ್ತ;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ size_t
strlen_safe(ಧ್ರುವ ವರ್ಣ *s)
{
    ಪ್ರತ್ಯಾಗಮ s ? strlen(s) : 0;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
isempty(ಧ್ರುವ ವರ್ಣ *s)
{
    ಪ್ರತ್ಯಾಗಮ s == ರಿಕ್ತ || s[0] == '\0';
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ ವರ್ಣ *
strnull(ಧ್ರುವ ವರ್ಣ *s)
{
    ಪ್ರತ್ಯಾಗಮ s ? s : "(null)";
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ ವರ್ಣ *
strempty(ಧ್ರುವ ವರ್ಣ *s)
{
    ಪ್ರತ್ಯಾಗಮ s ? s : "";
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ *
memdup(ಧ್ರುವ ಶೂನ್ಯ *mem, size_t nmemb, size_t size)
{
    ಶೂನ್ಯ *p = calloc(nmemb, size);
    ಯದಿ (p)
        memcpy(p, mem, nmemb * size);
    ಪ್ರತ್ಯಾಗಮ p;
}

#ಯದಿ !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ವರ್ಣ *
strndup(ಧ್ರುವ ವರ್ಣ *s, size_t n)
{
    size_t slen = strlen(s);
    size_t len = MIN(slen, n);
    ವರ್ಣ *p = malloc(len + 1);
    ಯದಿ (!p)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    memcpy(p, s, len);
    p[len] = '\0';
    ಪ್ರತ್ಯಾಗಮ p;
}
#endif

/* ctype.h is locale-dependent and has other oddities. */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
is_ascii(ವರ್ಣ ch)
{
    ಪ್ರತ್ಯಾಗಮ (ch & ~0x7f) == 0;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
is_space(ವರ್ಣ ch)
{
    ಪ್ರತ್ಯಾಗಮ ch == ' ' || (ch >= '\t' && ch <= '\r');
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
is_alpha(ವರ್ಣ ch)
{
    ಪ್ರತ್ಯಾಗಮ (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
is_digit(ವರ್ಣ ch)
{
    ಪ್ರತ್ಯಾಗಮ ch >= '0' && ch <= '9';
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
is_alnum(ವರ್ಣ ch)
{
    ಪ್ರತ್ಯಾಗಮ is_alpha(ch) || is_digit(ch);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
is_xdigit(ವರ್ಣ ch)
{
    ಪ್ರತ್ಯಾಗಮ
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
is_graph(ವರ್ಣ ch)
{
    /* See table in ascii(7). */
    ಪ್ರತ್ಯಾಗಮ ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned ಪೂರ್ಣಾಙ್ಕ
msb_pos(uint32_t mask)
{
    unsigned ಪೂರ್ಣಾಙ್ಕ pos = 0;
    ಯಾವತ್ (mask) {
        pos++;
        mask >>= 1u;
    }
    ಪ್ರತ್ಯಾಗಮ pos;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
one_bit_set(uint32_t x)
{
    ಪ್ರತ್ಯಾಗಮ x && (x & (x - 1)) == 0;
}

bool
map_file(FILE *file, ವರ್ಣ **string_out, size_t *size_out);

ಶೂನ್ಯ
unmap_file(ವರ್ಣ *string, size_t size);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
check_eaccess(ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ mode)
{
#ಯದಿ defined(HAVE_EACCESS)
    ಯದಿ (eaccess(path, mode) != 0)
        ಪ್ರತ್ಯಾಗಮ false;
#ನೋಚೇದ್ಯದಿ defined(HAVE_EUIDACCESS)
    ಯದಿ (euidaccess(path, mode) != 0)
        ಪ್ರತ್ಯಾಗಮ false;
#endif

    ಪ್ರತ್ಯಾಗಮ true;
}

FILE*
open_file(ಧ್ರುವ ವರ್ಣ *path);

#ಯದಿ defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#ನೋಚೇದ್ಯದಿ defined(HAVE___SECURE_GETENV)
# define secure_getenv __secure_getenv
#ಅನ್ಯಥಾ
# define secure_getenv getenv
#endif

#ಯದಿ defined(HAVE___BUILTIN_EXPECT)
# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#ಅನ್ಯಥಾ
# define likely(x)   (x)
# define unlikely(x) (x)
#endif

/* Compiler Attributes */

/* Private functions only exposed in tests. */
#ifdef ENABLE_PRIVATE_APIS
# ಯದಿ defined(__GNUC__) && !defined(__CYGWIN__)
#  define XKB_EXPORT_PRIVATE __attribute__((visibility("default")))
# elif defined(_WIN32)
#  define XKB_EXPORT_PRIVATE __declspec(dllexport)
# ಅನ್ಯಥಾ
#  define XKB_EXPORT_PRIVATE
# endif
#ಅನ್ಯಥಾ
# define XKB_EXPORT_PRIVATE
#endif

#ಯದಿ defined(__MINGW32__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__MINGW_PRINTF_FORMAT, x, y)))
#ನೋಚೇದ್ಯದಿ defined(__GNUC__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__printf__, x, y)))
#ಅನ್ಯಥಾ
# define ATTR_PRINTF(x,y)
#endif

#ಯದಿ defined(__GNUC__)
# define ATTR_NORETURN __attribute__((__noreturn__))
#ಅನ್ಯಥಾ
# define ATTR_NORETURN
#endif /* GNUC  */

#ಯದಿ defined(__GNUC__)
#define ATTR_MALLOC  __attribute__((__malloc__))
#ಅನ್ಯಥಾ
#define ATTR_MALLOC
#endif

#ಯದಿ defined(__GNUC__)
# define ATTR_NULL_SENTINEL __attribute__((__sentinel__))
#ಅನ್ಯಥಾ
# define ATTR_NULL_SENTINEL
#endif

#ಯದಿ defined(__GNUC__)
#define ATTR_PACKED  __attribute__((__packed__))
#ಅನ್ಯಥಾ
#define ATTR_PACKED
#endif

#ಯದಿ defined(HAVE_ATTR_COUNTED_BY) && HAVE_ATTR_COUNTED_BY
# define ATTR_COUNTED_BY(ಎಣಿಕೆ) __attribute__((counted_by(ಎಣಿಕೆ)))
#ಅನ್ಯಥಾ
# define ATTR_COUNTED_BY(ಎಣಿಕೆ)
#endif

#ಯದಿ !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
XKB_EXPORT_PRIVATE ಪೂರ್ಣಾಙ್ಕ asprintf(ವರ್ಣ **strp, ಧ್ರುವ ವರ್ಣ *fmt, ...) ATTR_PRINTF(2, 3);
# ಯದಿ !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_PRIVATE ಪೂರ್ಣಾಙ್ಕ vasprintf(ವರ್ಣ **strp, ಧ್ರುವ ವರ್ಣ *fmt, va_list ap);
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
ATTR_PRINTF(3, 4)
snprintf_safe(ವರ್ಣ *buf, size_t sz, ಧ್ರುವ ವರ್ಣ *format, ...)
{
    va_list ap;
    ಪೂರ್ಣಾಙ್ಕ rc;

    va_start(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    va_end(ap);

    ಪ್ರತ್ಯಾಗಮ rc >= 0 && (size_t)rc < sz;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ವರ್ಣ *
ATTR_PRINTF(1, 0)
vasprintf_safe(ಧ್ರುವ ವರ್ಣ *fmt, va_list args)
{
    ವರ್ಣ *str;
    ಪೂರ್ಣಾಙ್ಕ len;

    len = vasprintf(&str, fmt, args);

    ಯದಿ (len == -1)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ str;
}

/**
 * A version of asprintf that returns the allocated string or NULL on error.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ವರ್ಣ *
ATTR_PRINTF(1, 2)
asprintf_safe(ಧ್ರುವ ವರ್ಣ *fmt, ...)
{
    va_list args;
    ವರ್ಣ *str;

    va_start(args, fmt);
    str = vasprintf_safe(fmt, args);
    va_end(args);

    ಪ್ರತ್ಯಾಗಮ str;
}

#ifdef NDEBUG
    #ಯದಿ defined(__GNUC__) || defined(__clang__)
        #define PANIC_UNREACHABLE() __builtin_unreachable()
    #ನೋಚೇದ್ಯದಿ defined(_MSC_VER)
        #define PANIC_UNREACHABLE() __assume(0)
    #ಅನ್ಯಥಾ
        #define PANIC_UNREACHABLE() abort()
    #endif
#ಅನ್ಯಥಾ
    #define PANIC_UNREACHABLE() \
        ಕುರು { \
            fprintf(stderr, "Critical Error: Reached unreachable line in %s at %d\n", __FILE__, __LINE__); \
            abort(); \
        } ಯಾವತ್ (0)
#endif

#ಯದಿ defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#ನೋಚೇದ್ಯದಿ defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#ನೋಚೇದ್ಯದಿ defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#ಅನ್ಯಥಾ
#  define NOINLINE
#endif
