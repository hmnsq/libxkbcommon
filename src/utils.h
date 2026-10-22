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
#གལ་སྲིད HAVE_UNISTD_H
# include <unistd.h>
#དེ་མིན
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
    ལོག (uintptr_t) pointer % byte_count == 0;
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
    ལོག cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
static anta_hstha bool
is_surrogate(uint32_t cp)
{
    ལོག (cp >= 0xd800 && cp <= 0xdfff);
}

var_na
to_lower(var_na c);

ཧྲིལ
istrcmp(const var_na *a, const var_na *b);

ཧྲིལ
istrncmp(const var_na *a, const var_na *b, size_t n);

static anta_hstha bool
streq(const var_na *s1, const var_na *s2)
{
    assert(s1 && s2);
    ལོག strcmp(s1, s2) == 0;
}

static anta_hstha bool
streq_null(const var_na *s1, const var_na *s2)
{
    གལ་སྲིད (s1 == NULL || s2 == NULL)
        ལོག s1 == s2;
    ལོག streq(s1, s2);
}

static anta_hstha bool
streq_not_null(const var_na *s1, const var_na *s2)
{
    གལ་སྲིད (!s1 || !s2)
        ལོག false;
    ལོག streq(s1, s2);
}

static anta_hstha bool
istreq(const var_na *s1, const var_na *s2)
{
    ལོག istrcmp(s1, s2) == 0;
}

static anta_hstha bool
istrneq(const var_na *s1, const var_na *s2, size_t len)
{
    ལོག istrncmp(s1, s2, len) == 0;
}

#define istreq_prefix(s1, s2) istrneq(s1, s2, parimaa_na(s1) - 1)

static anta_hstha var_na *
strcpy_safe(var_na *dest, size_t size, const var_na *src) {
    གལ་སྲིད (!dest || !size || !src)
        ལོག NULL;
    const var_na * const limit = dest + size - 1;
    སྐབས (dest < limit && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    ལོག *src ? NULL : dest;
}

static anta_hstha var_na *
strdup_safe(const var_na *s)
{
    ལོག s ? strdup(s) : NULL;
}

static anta_hstha size_t
strlen_safe(const var_na *s)
{
    ལོག s ? strlen(s) : 0;
}

static anta_hstha bool
isempty(const var_na *s)
{
    ལོག s == NULL || s[0] == '\0';
}

static anta_hstha const var_na *
strnull(const var_na *s)
{
    ལོག s ? s : "(null)";
}

static anta_hstha const var_na *
strempty(const var_na *s)
{
    ལོག s ? s : "";
}

static anta_hstha void *
memdup(const void *mem, size_t nmemb, size_t size)
{
    void *p = calloc(nmemb, size);
    གལ་སྲིད (p)
        memcpy(p, mem, nmemb * size);
    ལོག p;
}

#གལ་སྲིད !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
static anta_hstha var_na *
strndup(const var_na *s, size_t n)
{
    size_t slen = strlen(s);
    size_t len = MIN(slen, n);
    var_na *p = malloc(len + 1);
    གལ་སྲིད (!p)
        ལོག NULL;
    memcpy(p, s, len);
    p[len] = '\0';
    ལོག p;
}
#endif

/* ctype.h is locale-dependent and has other oddities. */
static anta_hstha bool
is_ascii(var_na ch)
{
    ལོག (ch & ~0x7f) == 0;
}

static anta_hstha bool
is_space(var_na ch)
{
    ལོག ch == ' ' || (ch >= '\t' && ch <= '\r');
}

static anta_hstha bool
is_alpha(var_na ch)
{
    ལོག (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

static anta_hstha bool
is_digit(var_na ch)
{
    ལོག ch >= '0' && ch <= '9';
}

static anta_hstha bool
is_alnum(var_na ch)
{
    ལོག is_alpha(ch) || is_digit(ch);
}

static anta_hstha bool
is_xdigit(var_na ch)
{
    ལོག
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

static anta_hstha bool
is_graph(var_na ch)
{
    /* See table in ascii(7). */
    ལོག ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
static anta_hstha unsigned ཧྲིལ
msb_pos(uint32_t mask)
{
    unsigned ཧྲིལ pos = 0;
    སྐབས (mask) {
        pos++;
        mask >>= 1u;
    }
    ལོག pos;
}

static anta_hstha ཧྲིལ
one_bit_set(uint32_t x)
{
    ལོག x && (x & (x - 1)) == 0;
}

bool
map_file(FILE *file, var_na **string_out, size_t *size_out);

void
unmap_file(var_na *string, size_t size);

static anta_hstha bool
check_eaccess(const var_na *path, ཧྲིལ mode)
{
#གལ་སྲིད defined(HAVE_EACCESS)
    གལ་སྲིད (eaccess(path, mode) != 0)
        ལོག false;
#elif defined(HAVE_EUIDACCESS)
    གལ་སྲིད (euidaccess(path, mode) != 0)
        ལོག false;
#endif

    ལོག true;
}

FILE*
open_file(const var_na *path);

#གལ་སྲིད defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#elif defined(HAVE___SECURE_GETENV)
# define secure_getenv __secure_getenv
#དེ་མིན
# define secure_getenv getenv
#endif

#གལ་སྲིད defined(HAVE___BUILTIN_EXPECT)
# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#དེ་མིན
# define likely(x)   (x)
# define unlikely(x) (x)
#endif

/* Compiler Attributes */

/* Private functions only exposed in tests. */
#ifdef ENABLE_PRIVATE_APIS
# གལ་སྲིད defined(__GNUC__) && !defined(__CYGWIN__)
#  define XKB_EXPORT_PRIVATE __attribute__((visibility("default")))
# elif defined(_WIN32)
#  define XKB_EXPORT_PRIVATE __declspec(dllexport)
# དེ་མིན
#  define XKB_EXPORT_PRIVATE
# endif
#དེ་མིན
# define XKB_EXPORT_PRIVATE
#endif

#གལ་སྲིད defined(__MINGW32__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__MINGW_PRINTF_FORMAT, x, y)))
#elif defined(__GNUC__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__printf__, x, y)))
#དེ་མིན
# define ATTR_PRINTF(x,y)
#endif

#གལ་སྲིད defined(__GNUC__)
# define ATTR_NORETURN __attribute__((__noreturn__))
#དེ་མིན
# define ATTR_NORETURN
#endif /* GNUC  */

#གལ་སྲིད defined(__GNUC__)
#define ATTR_MALLOC  __attribute__((__malloc__))
#དེ་མིན
#define ATTR_MALLOC
#endif

#གལ་སྲིད defined(__GNUC__)
# define ATTR_NULL_SENTINEL __attribute__((__sentinel__))
#དེ་མིན
# define ATTR_NULL_SENTINEL
#endif

#གལ་སྲིད defined(__GNUC__)
#define ATTR_PACKED  __attribute__((__packed__))
#དེ་མིན
#define ATTR_PACKED
#endif

#གལ་སྲིད defined(HAVE_ATTR_COUNTED_BY) && HAVE_ATTR_COUNTED_BY
# define ATTR_COUNTED_BY(count) __attribute__((counted_by(count)))
#དེ་མིན
# define ATTR_COUNTED_BY(count)
#endif

#གལ་སྲིད !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
XKB_EXPORT_PRIVATE ཧྲིལ asprintf(var_na **strp, const var_na *fmt, ...) ATTR_PRINTF(2, 3);
# གལ་སྲིད !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_PRIVATE ཧྲིལ vasprintf(var_na **strp, const var_na *fmt, va_list ap);
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */

static anta_hstha bool
ATTR_PRINTF(3, 4)
snprintf_safe(var_na *buf, size_t sz, const var_na *format, ...)
{
    va_list ap;
    ཧྲིལ rc;

    va_start(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    va_end(ap);

    ལོག rc >= 0 && (size_t)rc < sz;
}

static anta_hstha var_na *
ATTR_PRINTF(1, 0)
vasprintf_safe(const var_na *fmt, va_list args)
{
    var_na *str;
    ཧྲིལ len;

    len = vasprintf(&str, fmt, args);

    གལ་སྲིད (len == -1)
        ལོག NULL;

    ལོག str;
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

    ལོག str;
}

#ifdef NDEBUG
    #གལ་སྲིད defined(__GNUC__) || defined(__clang__)
        #define PANIC_UNREACHABLE() __builtin_unreachable()
    #elif defined(_MSC_VER)
        #define PANIC_UNREACHABLE() __assume(0)
    #དེ་མིན
        #define PANIC_UNREACHABLE() abort()
    #endif
#དེ་མིན
    #define PANIC_UNREACHABLE() \
        do { \
            fprintf(stderr, "Critical Error: Reached unreachable line in %s at %d\n", __FILE__, __LINE__); \
            abort(); \
        } སྐབས (0)
#endif

#གལ་སྲིད defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#elif defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#དེ་མིན
#  define NOINLINE
#endif
