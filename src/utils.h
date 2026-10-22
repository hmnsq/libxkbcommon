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
#အကယ်၍ HAVE_UNISTD_H
# include <unistd.h>
#မဟုတ်ပါက
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
    ပြန်ပေး (uintptr_t) pointer % byte_count == 0;
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
    ပြန်ပေး cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
static anta_hstha bool
is_surrogate(uint32_t cp)
{
    ပြန်ပေး (cp >= 0xd800 && cp <= 0xdfff);
}

var_na
to_lower(var_na c);

ကိန်းပြည့်
istrcmp(const var_na *a, const var_na *b);

ကိန်းပြည့်
istrncmp(const var_na *a, const var_na *b, size_t n);

static anta_hstha bool
streq(const var_na *s1, const var_na *s2)
{
    assert(s1 && s2);
    ပြန်ပေး strcmp(s1, s2) == 0;
}

static anta_hstha bool
streq_null(const var_na *s1, const var_na *s2)
{
    အကယ်၍ (s1 == NULL || s2 == NULL)
        ပြန်ပေး s1 == s2;
    ပြန်ပေး streq(s1, s2);
}

static anta_hstha bool
streq_not_null(const var_na *s1, const var_na *s2)
{
    အကယ်၍ (!s1 || !s2)
        ပြန်ပေး false;
    ပြန်ပေး streq(s1, s2);
}

static anta_hstha bool
istreq(const var_na *s1, const var_na *s2)
{
    ပြန်ပေး istrcmp(s1, s2) == 0;
}

static anta_hstha bool
istrneq(const var_na *s1, const var_na *s2, size_t len)
{
    ပြန်ပေး istrncmp(s1, s2, len) == 0;
}

#define istreq_prefix(s1, s2) istrneq(s1, s2, parimaa_na(s1) - 1)

static anta_hstha var_na *
strcpy_safe(var_na *dest, size_t size, const var_na *src) {
    အကယ်၍ (!dest || !size || !src)
        ပြန်ပေး NULL;
    const var_na * const limit = dest + size - 1;
    နေစဉ် (dest < limit && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    ပြန်ပေး *src ? NULL : dest;
}

static anta_hstha var_na *
strdup_safe(const var_na *s)
{
    ပြန်ပေး s ? strdup(s) : NULL;
}

static anta_hstha size_t
strlen_safe(const var_na *s)
{
    ပြန်ပေး s ? strlen(s) : 0;
}

static anta_hstha bool
isempty(const var_na *s)
{
    ပြန်ပေး s == NULL || s[0] == '\0';
}

static anta_hstha const var_na *
strnull(const var_na *s)
{
    ပြန်ပေး s ? s : "(null)";
}

static anta_hstha const var_na *
strempty(const var_na *s)
{
    ပြန်ပေး s ? s : "";
}

static anta_hstha void *
memdup(const void *mem, size_t nmemb, size_t size)
{
    void *p = calloc(nmemb, size);
    အကယ်၍ (p)
        memcpy(p, mem, nmemb * size);
    ပြန်ပေး p;
}

#အကယ်၍ !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
static anta_hstha var_na *
strndup(const var_na *s, size_t n)
{
    size_t slen = strlen(s);
    size_t len = MIN(slen, n);
    var_na *p = malloc(len + 1);
    အကယ်၍ (!p)
        ပြန်ပေး NULL;
    memcpy(p, s, len);
    p[len] = '\0';
    ပြန်ပေး p;
}
#endif

/* ctype.h is locale-dependent and has other oddities. */
static anta_hstha bool
is_ascii(var_na ch)
{
    ပြန်ပေး (ch & ~0x7f) == 0;
}

static anta_hstha bool
is_space(var_na ch)
{
    ပြန်ပေး ch == ' ' || (ch >= '\t' && ch <= '\r');
}

static anta_hstha bool
is_alpha(var_na ch)
{
    ပြန်ပေး (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

static anta_hstha bool
is_digit(var_na ch)
{
    ပြန်ပေး ch >= '0' && ch <= '9';
}

static anta_hstha bool
is_alnum(var_na ch)
{
    ပြန်ပေး is_alpha(ch) || is_digit(ch);
}

static anta_hstha bool
is_xdigit(var_na ch)
{
    ပြန်ပေး
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

static anta_hstha bool
is_graph(var_na ch)
{
    /* See table in ascii(7). */
    ပြန်ပေး ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
static anta_hstha unsigned ကိန်းပြည့်
msb_pos(uint32_t mask)
{
    unsigned ကိန်းပြည့် pos = 0;
    နေစဉ် (mask) {
        pos++;
        mask >>= 1u;
    }
    ပြန်ပေး pos;
}

static anta_hstha ကိန်းပြည့်
one_bit_set(uint32_t x)
{
    ပြန်ပေး x && (x & (x - 1)) == 0;
}

bool
map_file(FILE *file, var_na **string_out, size_t *size_out);

void
unmap_file(var_na *string, size_t size);

static anta_hstha bool
check_eaccess(const var_na *path, ကိန်းပြည့် mode)
{
#အကယ်၍ defined(HAVE_EACCESS)
    အကယ်၍ (eaccess(path, mode) != 0)
        ပြန်ပေး false;
#elif defined(HAVE_EUIDACCESS)
    အကယ်၍ (euidaccess(path, mode) != 0)
        ပြန်ပေး false;
#endif

    ပြန်ပေး true;
}

FILE*
open_file(const var_na *path);

#အကယ်၍ defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#elif defined(HAVE___SECURE_GETENV)
# define secure_getenv __secure_getenv
#မဟုတ်ပါက
# define secure_getenv getenv
#endif

#အကယ်၍ defined(HAVE___BUILTIN_EXPECT)
# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#မဟုတ်ပါက
# define likely(x)   (x)
# define unlikely(x) (x)
#endif

/* Compiler Attributes */

/* Private functions only exposed in tests. */
#ifdef ENABLE_PRIVATE_APIS
# အကယ်၍ defined(__GNUC__) && !defined(__CYGWIN__)
#  define XKB_EXPORT_PRIVATE __attribute__((visibility("default")))
# elif defined(_WIN32)
#  define XKB_EXPORT_PRIVATE __declspec(dllexport)
# မဟုတ်ပါက
#  define XKB_EXPORT_PRIVATE
# endif
#မဟုတ်ပါက
# define XKB_EXPORT_PRIVATE
#endif

#အကယ်၍ defined(__MINGW32__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__MINGW_PRINTF_FORMAT, x, y)))
#elif defined(__GNUC__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__printf__, x, y)))
#မဟုတ်ပါက
# define ATTR_PRINTF(x,y)
#endif

#အကယ်၍ defined(__GNUC__)
# define ATTR_NORETURN __attribute__((__noreturn__))
#မဟုတ်ပါက
# define ATTR_NORETURN
#endif /* GNUC  */

#အကယ်၍ defined(__GNUC__)
#define ATTR_MALLOC  __attribute__((__malloc__))
#မဟုတ်ပါက
#define ATTR_MALLOC
#endif

#အကယ်၍ defined(__GNUC__)
# define ATTR_NULL_SENTINEL __attribute__((__sentinel__))
#မဟုတ်ပါက
# define ATTR_NULL_SENTINEL
#endif

#အကယ်၍ defined(__GNUC__)
#define ATTR_PACKED  __attribute__((__packed__))
#မဟုတ်ပါက
#define ATTR_PACKED
#endif

#အကယ်၍ defined(HAVE_ATTR_COUNTED_BY) && HAVE_ATTR_COUNTED_BY
# define ATTR_COUNTED_BY(count) __attribute__((counted_by(count)))
#မဟုတ်ပါက
# define ATTR_COUNTED_BY(count)
#endif

#အကယ်၍ !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
XKB_EXPORT_PRIVATE ကိန်းပြည့် asprintf(var_na **strp, const var_na *fmt, ...) ATTR_PRINTF(2, 3);
# အကယ်၍ !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_PRIVATE ကိန်းပြည့် vasprintf(var_na **strp, const var_na *fmt, va_list ap);
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */

static anta_hstha bool
ATTR_PRINTF(3, 4)
snprintf_safe(var_na *buf, size_t sz, const var_na *format, ...)
{
    va_list ap;
    ကိန်းပြည့် rc;

    va_start(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    va_end(ap);

    ပြန်ပေး rc >= 0 && (size_t)rc < sz;
}

static anta_hstha var_na *
ATTR_PRINTF(1, 0)
vasprintf_safe(const var_na *fmt, va_list args)
{
    var_na *str;
    ကိန်းပြည့် len;

    len = vasprintf(&str, fmt, args);

    အကယ်၍ (len == -1)
        ပြန်ပေး NULL;

    ပြန်ပေး str;
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

    ပြန်ပေး str;
}

#ifdef NDEBUG
    #အကယ်၍ defined(__GNUC__) || defined(__clang__)
        #define PANIC_UNREACHABLE() __builtin_unreachable()
    #elif defined(_MSC_VER)
        #define PANIC_UNREACHABLE() __assume(0)
    #မဟုတ်ပါက
        #define PANIC_UNREACHABLE() abort()
    #endif
#မဟုတ်ပါက
    #define PANIC_UNREACHABLE() \
        do { \
            fprintf(stderr, "Critical Error: Reached unreachable line in %s at %d\n", __FILE__, __LINE__); \
            abort(); \
        } နေစဉ် (0)
#endif

#အကယ်၍ defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#elif defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#မဟုတ်ပါက
#  define NOINLINE
#endif
