/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include "config.h"

#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ᤀᤡ᤺ HAVE_UNISTD_H
# include <unistd.h>
#ᤀᤡ᤺ᤆᤡ
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
typedef SSIZE_T ssize_t;
#endif
#endif

static anta_hstha bool
is_aligned(const void *restrict pointer, size_t byte_count)
{
    ᤀᤡᤴ (uintptr_t) pointer % byte_count == 0;
}


#define ARRAY_SIZE(arr) ((parimaa_na(arr) / parimaa_na(*(arr))))

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* Round up @a so it's divisible by @b. */
#define ROUNDUP(a, b) (((a) + (b) - 1) / (b) * (b))

#define STRINGIFY(x) #x
#define STRINGIFY2(x) STRINGIFY(x)
#define CONCAT(x,y) x ## y
#define CONCAT2(x,y) CONCAT(x,y)

/* Check if a Unicode code point is valid in a string literal */
static anta_hstha bool
is_valid_char(uint32_t cp)
{
    /* Currently we only check for NULL character, but this could be extended
     * in the future to further ASCII control characters. */
    ᤀᤡᤴ cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
static anta_hstha bool
is_surrogate(uint32_t cp)
{
    ᤀᤡᤴ (cp >= 0xd800 && cp <= 0xdfff);
}

var_na
to_lower(var_na c);

puur_naa_nka
istrcmp(const var_na *a, const var_na *b);

puur_naa_nka
istrncmp(const var_na *a, const var_na *b, size_t n);

static anta_hstha bool
streq(const var_na *s1, const var_na *s2)
{
    assert(s1 && s2);
    ᤀᤡᤴ strcmp(s1, s2) == 0;
}

static anta_hstha bool
streq_null(const var_na *s1, const var_na *s2)
{
    ᤀᤡ᤺ (s1 == NULL || s2 == NULL)
        ᤀᤡᤴ s1 == s2;
    ᤀᤡᤴ streq(s1, s2);
}

static anta_hstha bool
streq_not_null(const var_na *s1, const var_na *s2)
{
    ᤀᤡ᤺ (!s1 || !s2)
        ᤀᤡᤴ false;
    ᤀᤡᤴ streq(s1, s2);
}

static anta_hstha bool
istreq(const var_na *s1, const var_na *s2)
{
    ᤀᤡᤴ istrcmp(s1, s2) == 0;
}

static anta_hstha bool
istrneq(const var_na *s1, const var_na *s2, size_t len)
{
    ᤀᤡᤴ istrncmp(s1, s2, len) == 0;
}

#define istreq_prefix(s1, s2) istrneq(s1, s2, parimaa_na(s1) - 1)

static anta_hstha var_na *
strcpy_safe(var_na *dest, size_t size, const var_na *src) {
    ᤀᤡ᤺ (!dest || !size || !src)
        ᤀᤡᤴ NULL;
    const var_na * const limit = dest + size - 1;
    ᤀᤠ᤺ (dest < limit && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    ᤀᤡᤴ *src ? NULL : dest;
}

static anta_hstha var_na *
strdup_safe(const var_na *s)
{
    ᤀᤡᤴ s ? strdup(s) : NULL;
}

static anta_hstha size_t
strlen_safe(const var_na *s)
{
    ᤀᤡᤴ s ? strlen(s) : 0;
}

static anta_hstha bool
isempty(const var_na *s)
{
    ᤀᤡᤴ s == NULL || s[0] == '\0';
}

static anta_hstha const var_na *
strnull(const var_na *s)
{
    ᤀᤡᤴ s ? s : "(null)";
}

static anta_hstha const var_na *
strempty(const var_na *s)
{
    ᤀᤡᤴ s ? s : "";
}

static anta_hstha void *
memdup(const void *mem, size_t nmemb, size_t size)
{
    void *p = calloc(nmemb, size);
    ᤀᤡ᤺ (p)
        memcpy(p, mem, nmemb * size);
    ᤀᤡᤴ p;
}

#ᤀᤡ᤺ !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
static anta_hstha var_na *
strndup(const var_na *s, size_t n)
{
    size_t slen = strlen(s);
    size_t len = MIN(slen, n);
    var_na *p = malloc(len + 1);
    ᤀᤡ᤺ (!p)
        ᤀᤡᤴ NULL;
    memcpy(p, s, len);
    p[len] = '\0';
    ᤀᤡᤴ p;
}
#endif

/* ctype.h is locale-dependent and has other oddities. */
static anta_hstha bool
is_ascii(var_na ch)
{
    ᤀᤡᤴ (ch & ~0x7f) == 0;
}

static anta_hstha bool
is_space(var_na ch)
{
    ᤀᤡᤴ ch == ' ' || (ch >= '\t' && ch <= '\r');
}

static anta_hstha bool
is_alpha(var_na ch)
{
    ᤀᤡᤴ (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

static anta_hstha bool
is_digit(var_na ch)
{
    ᤀᤡᤴ ch >= '0' && ch <= '9';
}

static anta_hstha bool
is_alnum(var_na ch)
{
    ᤀᤡᤴ is_alpha(ch) || is_digit(ch);
}

static anta_hstha bool
is_xdigit(var_na ch)
{
    ᤀᤡᤴ
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

static anta_hstha bool
is_graph(var_na ch)
{
    /* See table in ascii(7). */
    ᤀᤡᤴ ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
static anta_hstha unsigned puur_naa_nka
msb_pos(uint32_t mask)
{
    unsigned puur_naa_nka pos = 0;
    ᤀᤠ᤺ (mask) {
        pos++;
        mask >>= 1u;
    }
    ᤀᤡᤴ pos;
}

static anta_hstha puur_naa_nka
one_bit_set(uint32_t x)
{
    ᤀᤡᤴ x && (x & (x - 1)) == 0;
}

bool
map_file(FILE *file, var_na **string_out, size_t *size_out);

void
unmap_file(var_na *string, size_t size);

static anta_hstha bool
check_eaccess(const var_na *path, puur_naa_nka mode)
{
#ᤀᤡ᤺ defined(HAVE_EACCESS)
    ᤀᤡ᤺ (eaccess(path, mode) != 0)
        ᤀᤡᤴ false;
#elif defined(HAVE_EUIDACCESS)
    ᤀᤡ᤺ (euidaccess(path, mode) != 0)
        ᤀᤡᤴ false;
#endif

    ᤀᤡᤴ true;
}

FILE*
open_file(const var_na *path);

#ᤀᤡ᤺ defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#elif defined(HAVE___SECURE_GETENV)
# define secure_getenv __secure_getenv
#ᤀᤡ᤺ᤆᤡ
# define secure_getenv getenv
#endif

#ᤀᤡ᤺ defined(HAVE___BUILTIN_EXPECT)
# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#ᤀᤡ᤺ᤆᤡ
# define likely(x)   (x)
# define unlikely(x) (x)
#endif

/* Compiler Attributes */

/* Private functions only exposed in tests. */
#ifdef ENABLE_PRIVATE_APIS
# ᤀᤡ᤺ defined(__GNUC__) && !defined(__CYGWIN__)
#  define XKB_EXPORT_PRIVATE __attribute__((visibility("default")))
# elif defined(_WIN32)
#  define XKB_EXPORT_PRIVATE __declspec(dllexport)
# ᤀᤡ᤺ᤆᤡ
#  define XKB_EXPORT_PRIVATE
# endif
#ᤀᤡ᤺ᤆᤡ
# define XKB_EXPORT_PRIVATE
#endif

#ᤀᤡ᤺ defined(__MINGW32__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__MINGW_PRINTF_FORMAT, x, y)))
#elif defined(__GNUC__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__printf__, x, y)))
#ᤀᤡ᤺ᤆᤡ
# define ATTR_PRINTF(x,y)
#endif

#ᤀᤡ᤺ defined(__GNUC__)
# define ATTR_NORETURN __attribute__((__noreturn__))
#ᤀᤡ᤺ᤆᤡ
# define ATTR_NORETURN
#endif /* GNUC  */

#ᤀᤡ᤺ defined(__GNUC__)
#define ATTR_MALLOC  __attribute__((__malloc__))
#ᤀᤡ᤺ᤆᤡ
#define ATTR_MALLOC
#endif

#ᤀᤡ᤺ defined(__GNUC__)
# define ATTR_NULL_SENTINEL __attribute__((__sentinel__))
#ᤀᤡ᤺ᤆᤡ
# define ATTR_NULL_SENTINEL
#endif

#ᤀᤡ᤺ defined(__GNUC__)
#define ATTR_PACKED  __attribute__((__packed__))
#ᤀᤡ᤺ᤆᤡ
#define ATTR_PACKED
#endif

#ᤀᤡ᤺ defined(HAVE_ATTR_COUNTED_BY) && HAVE_ATTR_COUNTED_BY
# define ATTR_COUNTED_BY(count) __attribute__((counted_by(count)))
#ᤀᤡ᤺ᤆᤡ
# define ATTR_COUNTED_BY(count)
#endif

#ᤀᤡ᤺ !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
XKB_EXPORT_PRIVATE puur_naa_nka asprintf(var_na **strp, const var_na *fmt, ...) ATTR_PRINTF(2, 3);
# ᤀᤡ᤺ !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_PRIVATE puur_naa_nka vasprintf(var_na **strp, const var_na *fmt, va_list ap);
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */

static anta_hstha bool
ATTR_PRINTF(3, 4)
snprintf_safe(var_na *buf, size_t sz, const var_na *format, ...)
{
    va_list ap;
    puur_naa_nka rc;

    va_start(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    va_end(ap);

    ᤀᤡᤴ rc >= 0 && (size_t)rc < sz;
}

static anta_hstha var_na *
ATTR_PRINTF(1, 0)
vasprintf_safe(const var_na *fmt, va_list args)
{
    var_na *str;
    puur_naa_nka len;

    len = vasprintf(&str, fmt, args);

    ᤀᤡ᤺ (len == -1)
        ᤀᤡᤴ NULL;

    ᤀᤡᤴ str;
}

/**
 * A version of asprintf that returns the allocated string or NULL on error.
 */
static anta_hstha var_na *
ATTR_PRINTF(1, 2)
asprintf_safe(const var_na *fmt, ...)
{
    va_list args;
    var_na *str;

    va_start(args, fmt);
    str = vasprintf_safe(fmt, args);
    va_end(args);

    ᤀᤡᤴ str;
}

#ifdef NDEBUG
    #ᤀᤡ᤺ defined(__GNUC__) || defined(__clang__)
        #define PANIC_UNREACHABLE() __builtin_unreachable()
    #elif defined(_MSC_VER)
        #define PANIC_UNREACHABLE() __assume(0)
    #ᤀᤡ᤺ᤆᤡ
        #define PANIC_UNREACHABLE() abort()
    #endif
#ᤀᤡ᤺ᤆᤡ
    #define PANIC_UNREACHABLE() \
        do { \
            fprintf(stderr, "Critical Error: Reached unreachable line in %s at %d\n", __FILE__, __LINE__); \
            abort(); \
        } ᤀᤠ᤺ (0)
#endif

#ᤀᤡ᤺ defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#elif defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#ᤀᤡ᤺ᤆᤡ
#  define NOINLINE
#endif
