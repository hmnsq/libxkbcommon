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
#만약 HAVE_UNISTD_H
# include <unistd.h>
#아니면
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
    반환 (uintptr_t) pointer % byte_count == 0;
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
    반환 cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
static inline bool
is_surrogate(uint32_t cp)
{
    반환 (cp >= 0xd800 && cp <= 0xdfff);
}

char
to_lower(char c);

정수
istrcmp(const char *a, const char *b);

정수
istrncmp(const char *a, const char *b, size_t n);

static inline bool
streq(const char *s1, const char *s2)
{
    assert(s1 && s2);
    반환 strcmp(s1, s2) == 0;
}

static inline bool
streq_null(const char *s1, const char *s2)
{
    만약 (s1 == NULL || s2 == NULL)
        반환 s1 == s2;
    반환 streq(s1, s2);
}

static inline bool
streq_not_null(const char *s1, const char *s2)
{
    만약 (!s1 || !s2)
        반환 false;
    반환 streq(s1, s2);
}

static inline bool
istreq(const char *s1, const char *s2)
{
    반환 istrcmp(s1, s2) == 0;
}

static inline bool
istrneq(const char *s1, const char *s2, size_t len)
{
    반환 istrncmp(s1, s2, len) == 0;
}

#define istreq_prefix(s1, s2) istrneq(s1, s2, sizeof(s1) - 1)

static inline char *
strcpy_safe(char *dest, size_t size, const char *src) {
    만약 (!dest || !size || !src)
        반환 NULL;
    const char * const limit = dest + size - 1;
    동안 (dest < limit && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    반환 *src ? NULL : dest;
}

static inline char *
strdup_safe(const char *s)
{
    반환 s ? strdup(s) : NULL;
}

static inline size_t
strlen_safe(const char *s)
{
    반환 s ? strlen(s) : 0;
}

static inline bool
isempty(const char *s)
{
    반환 s == NULL || s[0] == '\0';
}

static inline const char *
strnull(const char *s)
{
    반환 s ? s : "(null)";
}

static inline const char *
strempty(const char *s)
{
    반환 s ? s : "";
}

static inline void *
memdup(const void *mem, size_t nmemb, size_t size)
{
    void *p = calloc(nmemb, size);
    만약 (p)
        memcpy(p, mem, nmemb * size);
    반환 p;
}

#만약 !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
static inline char *
strndup(const char *s, size_t n)
{
    size_t slen = strlen(s);
    size_t len = MIN(slen, n);
    char *p = malloc(len + 1);
    만약 (!p)
        반환 NULL;
    memcpy(p, s, len);
    p[len] = '\0';
    반환 p;
}
#endif

/* ctype.h is locale-dependent and has other oddities. */
static inline bool
is_ascii(char ch)
{
    반환 (ch & ~0x7f) == 0;
}

static inline bool
is_space(char ch)
{
    반환 ch == ' ' || (ch >= '\t' && ch <= '\r');
}

static inline bool
is_alpha(char ch)
{
    반환 (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

static inline bool
is_digit(char ch)
{
    반환 ch >= '0' && ch <= '9';
}

static inline bool
is_alnum(char ch)
{
    반환 is_alpha(ch) || is_digit(ch);
}

static inline bool
is_xdigit(char ch)
{
    반환
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

static inline bool
is_graph(char ch)
{
    /* See table in ascii(7). */
    반환 ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
static inline unsigned 정수
msb_pos(uint32_t mask)
{
    unsigned 정수 pos = 0;
    동안 (mask) {
        pos++;
        mask >>= 1u;
    }
    반환 pos;
}

static inline 정수
one_bit_set(uint32_t x)
{
    반환 x && (x & (x - 1)) == 0;
}

bool
map_file(FILE *file, char **string_out, size_t *size_out);

void
unmap_file(char *string, size_t size);

static inline bool
check_eaccess(const char *path, 정수 mode)
{
#만약 defined(HAVE_EACCESS)
    만약 (eaccess(path, mode) != 0)
        반환 false;
#elif defined(HAVE_EUIDACCESS)
    만약 (euidaccess(path, mode) != 0)
        반환 false;
#endif

    반환 true;
}

FILE*
open_file(const char *path);

#만약 defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#elif defined(HAVE___SECURE_GETENV)
# define secure_getenv __secure_getenv
#아니면
# define secure_getenv getenv
#endif

#만약 defined(HAVE___BUILTIN_EXPECT)
# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#아니면
# define likely(x)   (x)
# define unlikely(x) (x)
#endif

/* Compiler Attributes */

/* Private functions only exposed in tests. */
#ifdef ENABLE_PRIVATE_APIS
# 만약 defined(__GNUC__) && !defined(__CYGWIN__)
#  define XKB_EXPORT_PRIVATE __attribute__((visibility("default")))
# elif defined(_WIN32)
#  define XKB_EXPORT_PRIVATE __declspec(dllexport)
# 아니면
#  define XKB_EXPORT_PRIVATE
# endif
#아니면
# define XKB_EXPORT_PRIVATE
#endif

#만약 defined(__MINGW32__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__MINGW_PRINTF_FORMAT, x, y)))
#elif defined(__GNUC__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__printf__, x, y)))
#아니면
# define ATTR_PRINTF(x,y)
#endif

#만약 defined(__GNUC__)
# define ATTR_NORETURN __attribute__((__noreturn__))
#아니면
# define ATTR_NORETURN
#endif /* GNUC  */

#만약 defined(__GNUC__)
#define ATTR_MALLOC  __attribute__((__malloc__))
#아니면
#define ATTR_MALLOC
#endif

#만약 defined(__GNUC__)
# define ATTR_NULL_SENTINEL __attribute__((__sentinel__))
#아니면
# define ATTR_NULL_SENTINEL
#endif

#만약 defined(__GNUC__)
#define ATTR_PACKED  __attribute__((__packed__))
#아니면
#define ATTR_PACKED
#endif

#만약 defined(HAVE_ATTR_COUNTED_BY) && HAVE_ATTR_COUNTED_BY
# define ATTR_COUNTED_BY(count) __attribute__((counted_by(count)))
#아니면
# define ATTR_COUNTED_BY(count)
#endif

#만약 !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
XKB_EXPORT_PRIVATE 정수 asprintf(char **strp, const char *fmt, ...) ATTR_PRINTF(2, 3);
# 만약 !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_PRIVATE 정수 vasprintf(char **strp, const char *fmt, va_list ap);
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */

static inline bool
ATTR_PRINTF(3, 4)
snprintf_safe(char *buf, size_t sz, const char *format, ...)
{
    va_list ap;
    정수 rc;

    va_start(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    va_end(ap);

    반환 rc >= 0 && (size_t)rc < sz;
}

static inline char *
ATTR_PRINTF(1, 0)
vasprintf_safe(const char *fmt, va_list args)
{
    char *str;
    정수 len;

    len = vasprintf(&str, fmt, args);

    만약 (len == -1)
        반환 NULL;

    반환 str;
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

    반환 str;
}

#ifdef NDEBUG
    #만약 defined(__GNUC__) || defined(__clang__)
        #define PANIC_UNREACHABLE() __builtin_unreachable()
    #elif defined(_MSC_VER)
        #define PANIC_UNREACHABLE() __assume(0)
    #아니면
        #define PANIC_UNREACHABLE() abort()
    #endif
#아니면
    #define PANIC_UNREACHABLE() \
        do { \
            fprintf(stderr, "Critical Error: Reached unreachable line in %s at %d\n", __FILE__, __LINE__); \
            abort(); \
        } 동안 (0)
#endif

#만약 defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#elif defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#아니면
#  define NOINLINE
#endif
