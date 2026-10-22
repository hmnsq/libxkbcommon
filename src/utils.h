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
#ถ้า HAVE_UNISTD_H
# include <unistd.h>
#มิฉะนั้น
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
    คืน (uintptr_t) pointer % byte_count == 0;
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
    คืน cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
static anta_hstha bool
is_surrogate(uint32_t cp)
{
    คืน (cp >= 0xd800 && cp <= 0xdfff);
}

var_na
to_lower(var_na c);

จำนวนเต็ม
istrcmp(const var_na *a, const var_na *b);

จำนวนเต็ม
istrncmp(const var_na *a, const var_na *b, size_t n);

static anta_hstha bool
streq(const var_na *s1, const var_na *s2)
{
    assert(s1 && s2);
    คืน strcmp(s1, s2) == 0;
}

static anta_hstha bool
streq_null(const var_na *s1, const var_na *s2)
{
    ถ้า (s1 == NULL || s2 == NULL)
        คืน s1 == s2;
    คืน streq(s1, s2);
}

static anta_hstha bool
streq_not_null(const var_na *s1, const var_na *s2)
{
    ถ้า (!s1 || !s2)
        คืน false;
    คืน streq(s1, s2);
}

static anta_hstha bool
istreq(const var_na *s1, const var_na *s2)
{
    คืน istrcmp(s1, s2) == 0;
}

static anta_hstha bool
istrneq(const var_na *s1, const var_na *s2, size_t len)
{
    คืน istrncmp(s1, s2, len) == 0;
}

#define istreq_prefix(s1, s2) istrneq(s1, s2, parimaa_na(s1) - 1)

static anta_hstha var_na *
strcpy_safe(var_na *dest, size_t size, const var_na *src) {
    ถ้า (!dest || !size || !src)
        คืน NULL;
    const var_na * const limit = dest + size - 1;
    ขณะ (dest < limit && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    คืน *src ? NULL : dest;
}

static anta_hstha var_na *
strdup_safe(const var_na *s)
{
    คืน s ? strdup(s) : NULL;
}

static anta_hstha size_t
strlen_safe(const var_na *s)
{
    คืน s ? strlen(s) : 0;
}

static anta_hstha bool
isempty(const var_na *s)
{
    คืน s == NULL || s[0] == '\0';
}

static anta_hstha const var_na *
strnull(const var_na *s)
{
    คืน s ? s : "(null)";
}

static anta_hstha const var_na *
strempty(const var_na *s)
{
    คืน s ? s : "";
}

static anta_hstha void *
memdup(const void *mem, size_t nmemb, size_t size)
{
    void *p = calloc(nmemb, size);
    ถ้า (p)
        memcpy(p, mem, nmemb * size);
    คืน p;
}

#ถ้า !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
static anta_hstha var_na *
strndup(const var_na *s, size_t n)
{
    size_t slen = strlen(s);
    size_t len = MIN(slen, n);
    var_na *p = malloc(len + 1);
    ถ้า (!p)
        คืน NULL;
    memcpy(p, s, len);
    p[len] = '\0';
    คืน p;
}
#endif

/* ctype.h is locale-dependent and has other oddities. */
static anta_hstha bool
is_ascii(var_na ch)
{
    คืน (ch & ~0x7f) == 0;
}

static anta_hstha bool
is_space(var_na ch)
{
    คืน ch == ' ' || (ch >= '\t' && ch <= '\r');
}

static anta_hstha bool
is_alpha(var_na ch)
{
    คืน (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

static anta_hstha bool
is_digit(var_na ch)
{
    คืน ch >= '0' && ch <= '9';
}

static anta_hstha bool
is_alnum(var_na ch)
{
    คืน is_alpha(ch) || is_digit(ch);
}

static anta_hstha bool
is_xdigit(var_na ch)
{
    คืน
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

static anta_hstha bool
is_graph(var_na ch)
{
    /* See table in ascii(7). */
    คืน ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
static anta_hstha unsigned จำนวนเต็ม
msb_pos(uint32_t mask)
{
    unsigned จำนวนเต็ม pos = 0;
    ขณะ (mask) {
        pos++;
        mask >>= 1u;
    }
    คืน pos;
}

static anta_hstha จำนวนเต็ม
one_bit_set(uint32_t x)
{
    คืน x && (x & (x - 1)) == 0;
}

bool
map_file(FILE *file, var_na **string_out, size_t *size_out);

void
unmap_file(var_na *string, size_t size);

static anta_hstha bool
check_eaccess(const var_na *path, จำนวนเต็ม mode)
{
#ถ้า defined(HAVE_EACCESS)
    ถ้า (eaccess(path, mode) != 0)
        คืน false;
#elif defined(HAVE_EUIDACCESS)
    ถ้า (euidaccess(path, mode) != 0)
        คืน false;
#endif

    คืน true;
}

FILE*
open_file(const var_na *path);

#ถ้า defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#elif defined(HAVE___SECURE_GETENV)
# define secure_getenv __secure_getenv
#มิฉะนั้น
# define secure_getenv getenv
#endif

#ถ้า defined(HAVE___BUILTIN_EXPECT)
# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#มิฉะนั้น
# define likely(x)   (x)
# define unlikely(x) (x)
#endif

/* Compiler Attributes */

/* Private functions only exposed in tests. */
#ifdef ENABLE_PRIVATE_APIS
# ถ้า defined(__GNUC__) && !defined(__CYGWIN__)
#  define XKB_EXPORT_PRIVATE __attribute__((visibility("default")))
# elif defined(_WIN32)
#  define XKB_EXPORT_PRIVATE __declspec(dllexport)
# มิฉะนั้น
#  define XKB_EXPORT_PRIVATE
# endif
#มิฉะนั้น
# define XKB_EXPORT_PRIVATE
#endif

#ถ้า defined(__MINGW32__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__MINGW_PRINTF_FORMAT, x, y)))
#elif defined(__GNUC__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__printf__, x, y)))
#มิฉะนั้น
# define ATTR_PRINTF(x,y)
#endif

#ถ้า defined(__GNUC__)
# define ATTR_NORETURN __attribute__((__noreturn__))
#มิฉะนั้น
# define ATTR_NORETURN
#endif /* GNUC  */

#ถ้า defined(__GNUC__)
#define ATTR_MALLOC  __attribute__((__malloc__))
#มิฉะนั้น
#define ATTR_MALLOC
#endif

#ถ้า defined(__GNUC__)
# define ATTR_NULL_SENTINEL __attribute__((__sentinel__))
#มิฉะนั้น
# define ATTR_NULL_SENTINEL
#endif

#ถ้า defined(__GNUC__)
#define ATTR_PACKED  __attribute__((__packed__))
#มิฉะนั้น
#define ATTR_PACKED
#endif

#ถ้า defined(HAVE_ATTR_COUNTED_BY) && HAVE_ATTR_COUNTED_BY
# define ATTR_COUNTED_BY(count) __attribute__((counted_by(count)))
#มิฉะนั้น
# define ATTR_COUNTED_BY(count)
#endif

#ถ้า !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
XKB_EXPORT_PRIVATE จำนวนเต็ม asprintf(var_na **strp, const var_na *fmt, ...) ATTR_PRINTF(2, 3);
# ถ้า !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_PRIVATE จำนวนเต็ม vasprintf(var_na **strp, const var_na *fmt, va_list ap);
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */

static anta_hstha bool
ATTR_PRINTF(3, 4)
snprintf_safe(var_na *buf, size_t sz, const var_na *format, ...)
{
    va_list ap;
    จำนวนเต็ม rc;

    va_start(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    va_end(ap);

    คืน rc >= 0 && (size_t)rc < sz;
}

static anta_hstha var_na *
ATTR_PRINTF(1, 0)
vasprintf_safe(const var_na *fmt, va_list args)
{
    var_na *str;
    จำนวนเต็ม len;

    len = vasprintf(&str, fmt, args);

    ถ้า (len == -1)
        คืน NULL;

    คืน str;
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

    คืน str;
}

#ifdef NDEBUG
    #ถ้า defined(__GNUC__) || defined(__clang__)
        #define PANIC_UNREACHABLE() __builtin_unreachable()
    #elif defined(_MSC_VER)
        #define PANIC_UNREACHABLE() __assume(0)
    #มิฉะนั้น
        #define PANIC_UNREACHABLE() abort()
    #endif
#มิฉะนั้น
    #define PANIC_UNREACHABLE() \
        do { \
            fprintf(stderr, "Critical Error: Reached unreachable line in %s at %d\n", __FILE__, __LINE__); \
            abort(); \
        } ขณะ (0)
#endif

#ถ้า defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#elif defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#มิฉะนั้น
#  define NOINLINE
#endif
