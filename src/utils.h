/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#شامل "config.h"

#شامل <assert.h>
#شامل <errno.h>
#شامل <inttypes.h>
#شامل <stdarg.h>
#شامل <stdbool.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
# include <unistd.h>
#وگرنه
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
تعریفنوع SSIZE_T ssize_t;
#endif
#endif

ایستا درونخطی bool
is_aligned(ثابت تهی *restrict pointer, size_t byte_شمار)
{
    بازگشت (uintptr_t) pointer % byte_شمار == 0;
}


#define ARRAY_SIZE(arr) ((اندازه(arr) / اندازه(*(arr))))

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* Round up @a so it's divisible by @b. */
#define ROUNDUP(a, b) (((a) + (b) - 1) / (b) * (b))

#define STRINGIFY(x) #x
#define STRINGIFY2(x) STRINGIFY(x)
#define CONCAT(x,y) x ## y
#define CONCAT2(x,y) CONCAT(x,y)

/* Check if a Unicode code point is valid in a string literal */
ایستا درونخطی bool
is_valid_char(uint32_t cp)
{
    /* Currently we only check for NULL character, but this could be extended
     * in the future to further ASCII control characters. */
    بازگشت cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
ایستا درونخطی bool
is_surrogate(uint32_t cp)
{
    بازگشت (cp >= 0xd800 && cp <= 0xdfff);
}

نویسه
to_lower(نویسه c);

صحیح
istrcmp(ثابت نویسه *a, ثابت نویسه *b);

صحیح
istrncmp(ثابت نویسه *a, ثابت نویسه *b, size_t n);

ایستا درونخطی bool
streq(ثابت نویسه *s1, ثابت نویسه *s2)
{
    أكد(s1 && s2);
    بازگشت strcmp(s1, s2) == 0;
}

ایستا درونخطی bool
streq_null(ثابت نویسه *s1, ثابت نویسه *s2)
{
    اگر (s1 == پوچ || s2 == پوچ)
        بازگشت s1 == s2;
    بازگشت streq(s1, s2);
}

ایستا درونخطی bool
streq_not_null(ثابت نویسه *s1, ثابت نویسه *s2)
{
    اگر (!s1 || !s2)
        بازگشت false;
    بازگشت streq(s1, s2);
}

ایستا درونخطی bool
istreq(ثابت نویسه *s1, ثابت نویسه *s2)
{
    بازگشت istrcmp(s1, s2) == 0;
}

ایستا درونخطی bool
istrneq(ثابت نویسه *s1, ثابت نویسه *s2, size_t len)
{
    بازگشت istrncmp(s1, s2, len) == 0;
}

#define istreq_prefix(s1, s2) istrneq(s1, s2, اندازه(s1) - 1)

ایستا درونخطی نویسه *
strcpy_safe(نویسه *dest, size_t size, ثابت نویسه *src) {
    اگر (!dest || !size || !src)
        بازگشت پوچ;
    ثابت نویسه * ثابت limit = dest + size - 1;
    وقتی (dest < limit && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    بازگشت *src ? پوچ : dest;
}

ایستا درونخطی نویسه *
strdup_safe(ثابت نویسه *s)
{
    بازگشت s ? strdup(s) : پوچ;
}

ایستا درونخطی size_t
strlen_safe(ثابت نویسه *s)
{
    بازگشت s ? strlen(s) : 0;
}

ایستا درونخطی bool
isempty(ثابت نویسه *s)
{
    بازگشت s == پوچ || s[0] == '\0';
}

ایستا درونخطی ثابت نویسه *
strnull(ثابت نویسه *s)
{
    بازگشت s ? s : "(null)";
}

ایستا درونخطی ثابت نویسه *
strempty(ثابت نویسه *s)
{
    بازگشت s ? s : "";
}

ایستا درونخطی تهی *
memdup(ثابت تهی *mem, size_t nmemb, size_t size)
{
    تهی *p = calloc(nmemb, size);
    اگر (p)
        memcpy(p, mem, nmemb * size);
    بازگشت p;
}

#اگر !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
ایستا درونخطی نویسه *
strndup(ثابت نویسه *s, size_t n)
{
    size_t slen = strlen(s);
    size_t len = MIN(slen, n);
    نویسه *p = malloc(len + 1);
    اگر (!p)
        بازگشت پوچ;
    memcpy(p, s, len);
    p[len] = '\0';
    بازگشت p;
}
#endif

/* ctype.h is locale-dependent and has other oddities. */
ایستا درونخطی bool
is_ascii(نویسه ch)
{
    بازگشت (ch & ~0x7f) == 0;
}

ایستا درونخطی bool
is_space(نویسه ch)
{
    بازگشت ch == ' ' || (ch >= '\t' && ch <= '\r');
}

ایستا درونخطی bool
is_alpha(نویسه ch)
{
    بازگشت (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

ایستا درونخطی bool
is_digit(نویسه ch)
{
    بازگشت ch >= '0' && ch <= '9';
}

ایستا درونخطی bool
is_alnum(نویسه ch)
{
    بازگشت is_alpha(ch) || is_digit(ch);
}

ایستا درونخطی bool
is_xdigit(نویسه ch)
{
    بازگشت
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

ایستا درونخطی bool
is_graph(نویسه ch)
{
    /* See table in ascii(7). */
    بازگشت ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
ایستا درونخطی unsigned صحیح
msb_pos(uint32_t mask)
{
    unsigned صحیح pos = 0;
    وقتی (mask) {
        pos++;
        mask >>= 1u;
    }
    بازگشت pos;
}

ایستا درونخطی صحیح
one_bit_set(uint32_t x)
{
    بازگشت x && (x & (x - 1)) == 0;
}

bool
map_file(FILE *file, نویسه **string_out, size_t *size_out);

تهی
unmap_file(نویسه *string, size_t size);

ایستا درونخطی bool
check_eaccess(ثابت نویسه *path, صحیح mode)
{
#اگر defined(HAVE_EACCESS)
    اگر (eaccess(path, mode) != 0)
        بازگشت false;
#وگرنهاگر defined(HAVE_EUIDACCESS)
    اگر (euidaccess(path, mode) != 0)
        بازگشت false;
#endif

    بازگشت true;
}

FILE*
open_file(ثابت نویسه *path);

#اگر defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#وگرنهاگر defined(HAVE___SECURE_GETENV)
# define secure_getenv __secure_getenv
#وگرنه
# define secure_getenv getenv
#endif

#اگر defined(HAVE___BUILTIN_EXPECT)
# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#وگرنه
# define likely(x)   (x)
# define unlikely(x) (x)
#endif

/* Compiler Attributes */

/* Private functions only exposed in tests. */
#ifdef ENABLE_PRIVATE_APIS
# اگر defined(__GNUC__) && !defined(__CYGWIN__)
#  define XKB_EXPORT_PRIVATE __attribute__((visibility("default")))
# elif defined(_WIN32)
#  define XKB_EXPORT_PRIVATE __declspec(dllexport)
# وگرنه
#  define XKB_EXPORT_PRIVATE
# endif
#وگرنه
# define XKB_EXPORT_PRIVATE
#endif

#اگر defined(__MINGW32__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__MINGW_PRINTF_FORMAT, x, y)))
#وگرنهاگر defined(__GNUC__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__printf__, x, y)))
#وگرنه
# define ATTR_PRINTF(x,y)
#endif

#اگر defined(__GNUC__)
# define ATTR_NORETURN __attribute__((__noreturn__))
#وگرنه
# define ATTR_NORETURN
#endif /* GNUC  */

#اگر defined(__GNUC__)
#define ATTR_MALLOC  __attribute__((__malloc__))
#وگرنه
#define ATTR_MALLOC
#endif

#اگر defined(__GNUC__)
# define ATTR_NULL_SENTINEL __attribute__((__sentinel__))
#وگرنه
# define ATTR_NULL_SENTINEL
#endif

#اگر defined(__GNUC__)
#define ATTR_PACKED  __attribute__((__packed__))
#وگرنه
#define ATTR_PACKED
#endif

#اگر defined(HAVE_ATTR_COUNTED_BY) && HAVE_ATTR_COUNTED_BY
# define ATTR_COUNTED_BY(شمار) __attribute__((counted_by(شمار)))
#وگرنه
# define ATTR_COUNTED_BY(شمار)
#endif

#اگر !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
XKB_EXPORT_PRIVATE صحیح asprintf(نویسه **strp, ثابت نویسه *fmt, ...) ATTR_PRINTF(2, 3);
# اگر !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_PRIVATE صحیح vasprintf(نویسه **strp, ثابت نویسه *fmt, va_list ap);
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */

ایستا درونخطی bool
ATTR_PRINTF(3, 4)
snprintf_safe(نویسه *buf, size_t sz, ثابت نویسه *format, ...)
{
    va_list ap;
    صحیح rc;

    va_start(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    va_end(ap);

    بازگشت rc >= 0 && (size_t)rc < sz;
}

ایستا درونخطی نویسه *
ATTR_PRINTF(1, 0)
vasprintf_safe(ثابت نویسه *fmt, va_list args)
{
    نویسه *str;
    صحیح len;

    len = vasprintf(&str, fmt, args);

    اگر (len == -1)
        بازگشت پوچ;

    بازگشت str;
}

/**
 * A version of asprintf that returns the allocated string or NULL on error.
 */
ایستا درونخطی نویسه *
ATTR_PRINTF(1, 2)
asprintf_safe(ثابت نویسه *fmt, ...)
{
    va_list args;
    نویسه *str;

    va_start(args, fmt);
    str = vasprintf_safe(fmt, args);
    va_end(args);

    بازگشت str;
}

#ifdef NDEBUG
    #اگر defined(__GNUC__) || defined(__clang__)
        #define PANIC_UNREACHABLE() __builtin_unreachable()
    #وگرنهاگر defined(_MSC_VER)
        #define PANIC_UNREACHABLE() __assume(0)
    #وگرنه
        #define PANIC_UNREACHABLE() abort()
    #endif
#وگرنه
    #define PANIC_UNREACHABLE() \
        انجامبده { \
            fprintf(stderr, "Critical Error: Reached unreachable line in %s at %d\n", __FILE__, __LINE__); \
            abort(); \
        } وقتی (0)
#endif

#اگر defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#وگرنهاگر defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#وگرنهاگر defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#وگرنه
#  define NOINLINE
#endif
