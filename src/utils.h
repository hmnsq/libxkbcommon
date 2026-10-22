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
#nếu HAVE_UNISTD_H
# include <unistd.h>
#không_thì
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

static inline bool
is_aligned(const void *restrict pointer, size_t byte_count)
{
    trả_về (uintptr_t) pointer % byte_count == 0;
}


#define ARRAY_SIZE(arr) ((sizeof(arr) / sizeof(*(arr))))

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* Round up @a so it's divisible by @b. */
#define ROUNDUP(a, b) (((a) + (b) - 1) / (b) * (b))

#define STRINGIFY(x) #x
#define STRINGIFY2(x) STRINGIFY(x)
#define CONCAT(x,y) x ## y
#define CONCAT2(x,y) CONCAT(x,y)

/* Check if a Unicode code point is valid in a string literal */
static inline bool
is_valid_char(uint32_t cp)
{
    /* Currently we only check for NULL character, but this could be extended
     * in the future to further ASCII control characters. */
    trả_về cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
static inline bool
is_surrogate(uint32_t cp)
{
    trả_về (cp >= 0xd800 && cp <= 0xdfff);
}

char
to_lower(char c);

số_nguyên
istrcmp(const char *a, const char *b);

số_nguyên
istrncmp(const char *a, const char *b, size_t n);

static inline bool
streq(const char *s1, const char *s2)
{
    assert(s1 && s2);
    trả_về strcmp(s1, s2) == 0;
}

static inline bool
streq_null(const char *s1, const char *s2)
{
    nếu (s1 == NULL || s2 == NULL)
        trả_về s1 == s2;
    trả_về streq(s1, s2);
}

static inline bool
streq_not_null(const char *s1, const char *s2)
{
    nếu (!s1 || !s2)
        trả_về false;
    trả_về streq(s1, s2);
}

static inline bool
istreq(const char *s1, const char *s2)
{
    trả_về istrcmp(s1, s2) == 0;
}

static inline bool
istrneq(const char *s1, const char *s2, size_t len)
{
    trả_về istrncmp(s1, s2, len) == 0;
}

#define istreq_prefix(s1, s2) istrneq(s1, s2, sizeof(s1) - 1)

static inline char *
strcpy_safe(char *dest, size_t size, const char *src) {
    nếu (!dest || !size || !src)
        trả_về NULL;
    const char * const limit = dest + size - 1;
    trong_khi (dest < limit && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    trả_về *src ? NULL : dest;
}

static inline char *
strdup_safe(const char *s)
{
    trả_về s ? strdup(s) : NULL;
}

static inline size_t
strlen_safe(const char *s)
{
    trả_về s ? strlen(s) : 0;
}

static inline bool
isempty(const char *s)
{
    trả_về s == NULL || s[0] == '\0';
}

static inline const char *
strnull(const char *s)
{
    trả_về s ? s : "(null)";
}

static inline const char *
strempty(const char *s)
{
    trả_về s ? s : "";
}

static inline void *
memdup(const void *mem, size_t nmemb, size_t size)
{
    void *p = calloc(nmemb, size);
    nếu (p)
        memcpy(p, mem, nmemb * size);
    trả_về p;
}

#nếu !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
static inline char *
strndup(const char *s, size_t n)
{
    size_t slen = strlen(s);
    size_t len = MIN(slen, n);
    char *p = malloc(len + 1);
    nếu (!p)
        trả_về NULL;
    memcpy(p, s, len);
    p[len] = '\0';
    trả_về p;
}
#endif

/* ctype.h is locale-dependent and has other oddities. */
static inline bool
is_ascii(char ch)
{
    trả_về (ch & ~0x7f) == 0;
}

static inline bool
is_space(char ch)
{
    trả_về ch == ' ' || (ch >= '\t' && ch <= '\r');
}

static inline bool
is_alpha(char ch)
{
    trả_về (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

static inline bool
is_digit(char ch)
{
    trả_về ch >= '0' && ch <= '9';
}

static inline bool
is_alnum(char ch)
{
    trả_về is_alpha(ch) || is_digit(ch);
}

static inline bool
is_xdigit(char ch)
{
    trả_về
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

static inline bool
is_graph(char ch)
{
    /* See table in ascii(7). */
    trả_về ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
static inline unsigned số_nguyên
msb_pos(uint32_t mask)
{
    unsigned số_nguyên pos = 0;
    trong_khi (mask) {
        pos++;
        mask >>= 1u;
    }
    trả_về pos;
}

static inline số_nguyên
one_bit_set(uint32_t x)
{
    trả_về x && (x & (x - 1)) == 0;
}

bool
map_file(FILE *file, char **string_out, size_t *size_out);

void
unmap_file(char *string, size_t size);

static inline bool
check_eaccess(const char *path, số_nguyên mode)
{
#nếu defined(HAVE_EACCESS)
    nếu (eaccess(path, mode) != 0)
        trả_về false;
#elif defined(HAVE_EUIDACCESS)
    nếu (euidaccess(path, mode) != 0)
        trả_về false;
#endif

    trả_về true;
}

FILE*
open_file(const char *path);

#nếu defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#elif defined(HAVE___SECURE_GETENV)
# define secure_getenv __secure_getenv
#không_thì
# define secure_getenv getenv
#endif

#nếu defined(HAVE___BUILTIN_EXPECT)
# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#không_thì
# define likely(x)   (x)
# define unlikely(x) (x)
#endif

/* Compiler Attributes */

/* Private functions only exposed in tests. */
#ifdef ENABLE_PRIVATE_APIS
# nếu defined(__GNUC__) && !defined(__CYGWIN__)
#  define XKB_EXPORT_PRIVATE __attribute__((visibility("default")))
# elif defined(_WIN32)
#  define XKB_EXPORT_PRIVATE __declspec(dllexport)
# không_thì
#  define XKB_EXPORT_PRIVATE
# endif
#không_thì
# define XKB_EXPORT_PRIVATE
#endif

#nếu defined(__MINGW32__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__MINGW_PRINTF_FORMAT, x, y)))
#elif defined(__GNUC__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__printf__, x, y)))
#không_thì
# define ATTR_PRINTF(x,y)
#endif

#nếu defined(__GNUC__)
# define ATTR_NORETURN __attribute__((__noreturn__))
#không_thì
# define ATTR_NORETURN
#endif /* GNUC  */

#nếu defined(__GNUC__)
#define ATTR_MALLOC  __attribute__((__malloc__))
#không_thì
#define ATTR_MALLOC
#endif

#nếu defined(__GNUC__)
# define ATTR_NULL_SENTINEL __attribute__((__sentinel__))
#không_thì
# define ATTR_NULL_SENTINEL
#endif

#nếu defined(__GNUC__)
#define ATTR_PACKED  __attribute__((__packed__))
#không_thì
#define ATTR_PACKED
#endif

#nếu defined(HAVE_ATTR_COUNTED_BY) && HAVE_ATTR_COUNTED_BY
# define ATTR_COUNTED_BY(count) __attribute__((counted_by(count)))
#không_thì
# define ATTR_COUNTED_BY(count)
#endif

#nếu !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
XKB_EXPORT_PRIVATE số_nguyên asprintf(char **strp, const char *fmt, ...) ATTR_PRINTF(2, 3);
# nếu !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_PRIVATE số_nguyên vasprintf(char **strp, const char *fmt, va_list ap);
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */

static inline bool
ATTR_PRINTF(3, 4)
snprintf_safe(char *buf, size_t sz, const char *format, ...)
{
    va_list ap;
    số_nguyên rc;

    va_start(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    va_end(ap);

    trả_về rc >= 0 && (size_t)rc < sz;
}

static inline char *
ATTR_PRINTF(1, 0)
vasprintf_safe(const char *fmt, va_list args)
{
    char *str;
    số_nguyên len;

    len = vasprintf(&str, fmt, args);

    nếu (len == -1)
        trả_về NULL;

    trả_về str;
}

/**
 * A version of asprintf that returns the allocated string or NULL on error.
 */
static inline char *
ATTR_PRINTF(1, 2)
asprintf_safe(const char *fmt, ...)
{
    va_list args;
    char *str;

    va_start(args, fmt);
    str = vasprintf_safe(fmt, args);
    va_end(args);

    trả_về str;
}

#ifdef NDEBUG
    #nếu defined(__GNUC__) || defined(__clang__)
        #define PANIC_UNREACHABLE() __builtin_unreachable()
    #elif defined(_MSC_VER)
        #define PANIC_UNREACHABLE() __assume(0)
    #không_thì
        #define PANIC_UNREACHABLE() abort()
    #endif
#không_thì
    #define PANIC_UNREACHABLE() \
        do { \
            fprintf(stderr, "Critical Error: Reached unreachable line in %s at %d\n", __FILE__, __LINE__); \
            abort(); \
        } trong_khi (0)
#endif

#nếu defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#elif defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#không_thì
#  define NOINLINE
#endif
