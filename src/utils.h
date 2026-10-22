/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <inttypes.h>
#અન્તર્ભાવ <stdarg.h>
#અન્તર્ભાવ <stdbool.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
# include <unistd.h>
#નહીંતર
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
પ્રકારવ્યાખ્યા SSIZE_T ssize_t;
#endif
#endif

સ્થિર અંતઃરેખ bool
is_aligned(અચલ શૂન્ય *restrict pointer, size_t byte_ગણતરી)
{
    પાછા (uintptr_t) pointer % byte_ગણતરી == 0;
}


#define ARRAY_SIZE(arr) ((કદ(arr) / કદ(*(arr))))

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* Round up @a so it's divisible by @b. */
#define ROUNDUP(a, b) (((a) + (b) - 1) / (b) * (b))

#define STRINGIFY(x) #x
#define STRINGIFY2(x) STRINGIFY(x)
#define CONCAT(x,y) x ## y
#define CONCAT2(x,y) CONCAT(x,y)

/* Check if a Unicode code point is valid in a string literal */
સ્થિર અંતઃરેખ bool
is_valid_char(uint32_t cp)
{
    /* Currently we only check for NULL character, but this could be extended
     * in the future to further ASCII control characters. */
    પાછા cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
સ્થિર અંતઃરેખ bool
is_surrogate(uint32_t cp)
{
    પાછા (cp >= 0xd800 && cp <= 0xdfff);
}

અક્ષર
to_lower(અક્ષર c);

પૂર્ણાંક
istrcmp(અચલ અક્ષર *a, અચલ અક્ષર *b);

પૂર્ણાંક
istrncmp(અચલ અક્ષર *a, અચલ અક્ષર *b, size_t n);

સ્થિર અંતઃરેખ bool
streq(અચલ અક્ષર *s1, અચલ અક્ષર *s2)
{
    પ્રતિજ્ઞા(s1 && s2);
    પાછા strcmp(s1, s2) == 0;
}

સ્થિર અંતઃરેખ bool
streq_null(અચલ અક્ષર *s1, અચલ અક્ષર *s2)
{
    જો (s1 == ખાલી || s2 == ખાલી)
        પાછા s1 == s2;
    પાછા streq(s1, s2);
}

સ્થિર અંતઃરેખ bool
streq_not_null(અચલ અક્ષર *s1, અચલ અક્ષર *s2)
{
    જો (!s1 || !s2)
        પાછા false;
    પાછા streq(s1, s2);
}

સ્થિર અંતઃરેખ bool
istreq(અચલ અક્ષર *s1, અચલ અક્ષર *s2)
{
    પાછા istrcmp(s1, s2) == 0;
}

સ્થિર અંતઃરેખ bool
istrneq(અચલ અક્ષર *s1, અચલ અક્ષર *s2, size_t len)
{
    પાછા istrncmp(s1, s2, len) == 0;
}

#define istreq_prefix(s1, s2) istrneq(s1, s2, કદ(s1) - 1)

સ્થિર અંતઃરેખ અક્ષર *
strcpy_safe(અક્ષર *dest, size_t size, અચલ અક્ષર *src) {
    જો (!dest || !size || !src)
        પાછા ખાલી;
    અચલ અક્ષર * અચલ limit = dest + size - 1;
    જ્યારે (dest < limit && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    પાછા *src ? ખાલી : dest;
}

સ્થિર અંતઃરેખ અક્ષર *
strdup_safe(અચલ અક્ષર *s)
{
    પાછા s ? strdup(s) : ખાલી;
}

સ્થિર અંતઃરેખ size_t
strlen_safe(અચલ અક્ષર *s)
{
    પાછા s ? strlen(s) : 0;
}

સ્થિર અંતઃરેખ bool
isempty(અચલ અક્ષર *s)
{
    પાછા s == ખાલી || s[0] == '\0';
}

સ્થિર અંતઃરેખ અચલ અક્ષર *
strnull(અચલ અક્ષર *s)
{
    પાછા s ? s : "(null)";
}

સ્થિર અંતઃરેખ અચલ અક્ષર *
strempty(અચલ અક્ષર *s)
{
    પાછા s ? s : "";
}

સ્થિર અંતઃરેખ શૂન્ય *
memdup(અચલ શૂન્ય *mem, size_t nmemb, size_t size)
{
    શૂન્ય *p = calloc(nmemb, size);
    જો (p)
        memcpy(p, mem, nmemb * size);
    પાછા p;
}

#જો !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
સ્થિર અંતઃરેખ અક્ષર *
strndup(અચલ અક્ષર *s, size_t n)
{
    size_t slen = strlen(s);
    size_t len = MIN(slen, n);
    અક્ષર *p = malloc(len + 1);
    જો (!p)
        પાછા ખાલી;
    memcpy(p, s, len);
    p[len] = '\0';
    પાછા p;
}
#endif

/* ctype.h is locale-dependent and has other oddities. */
સ્થિર અંતઃરેખ bool
is_ascii(અક્ષર ch)
{
    પાછા (ch & ~0x7f) == 0;
}

સ્થિર અંતઃરેખ bool
is_space(અક્ષર ch)
{
    પાછા ch == ' ' || (ch >= '\t' && ch <= '\r');
}

સ્થિર અંતઃરેખ bool
is_alpha(અક્ષર ch)
{
    પાછા (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

સ્થિર અંતઃરેખ bool
is_digit(અક્ષર ch)
{
    પાછા ch >= '0' && ch <= '9';
}

સ્થિર અંતઃરેખ bool
is_alnum(અક્ષર ch)
{
    પાછા is_alpha(ch) || is_digit(ch);
}

સ્થિર અંતઃરેખ bool
is_xdigit(અક્ષર ch)
{
    પાછા
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

સ્થિર અંતઃરેખ bool
is_graph(અક્ષર ch)
{
    /* See table in ascii(7). */
    પાછા ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
સ્થિર અંતઃરેખ unsigned પૂર્ણાંક
msb_pos(uint32_t mask)
{
    unsigned પૂર્ણાંક pos = 0;
    જ્યારે (mask) {
        pos++;
        mask >>= 1u;
    }
    પાછા pos;
}

સ્થિર અંતઃરેખ પૂર્ણાંક
one_bit_set(uint32_t x)
{
    પાછા x && (x & (x - 1)) == 0;
}

bool
map_file(FILE *file, અક્ષર **string_out, size_t *size_out);

શૂન્ય
unmap_file(અક્ષર *string, size_t size);

સ્થિર અંતઃરેખ bool
check_eaccess(અચલ અક્ષર *path, પૂર્ણાંક mode)
{
#જો defined(HAVE_EACCESS)
    જો (eaccess(path, mode) != 0)
        પાછા false;
#અથવાજો defined(HAVE_EUIDACCESS)
    જો (euidaccess(path, mode) != 0)
        પાછા false;
#endif

    પાછા true;
}

FILE*
open_file(અચલ અક્ષર *path);

#જો defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#અથવાજો defined(HAVE___SECURE_GETENV)
# define secure_getenv __secure_getenv
#નહીંતર
# define secure_getenv getenv
#endif

#જો defined(HAVE___BUILTIN_EXPECT)
# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#નહીંતર
# define likely(x)   (x)
# define unlikely(x) (x)
#endif

/* Compiler Attributes */

/* Private functions only exposed in tests. */
#ifdef ENABLE_PRIVATE_APIS
# જો defined(__GNUC__) && !defined(__CYGWIN__)
#  define XKB_EXPORT_PRIVATE __attribute__((visibility("default")))
# elif defined(_WIN32)
#  define XKB_EXPORT_PRIVATE __declspec(dllexport)
# નહીંતર
#  define XKB_EXPORT_PRIVATE
# endif
#નહીંતર
# define XKB_EXPORT_PRIVATE
#endif

#જો defined(__MINGW32__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__MINGW_PRINTF_FORMAT, x, y)))
#અથવાજો defined(__GNUC__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__printf__, x, y)))
#નહીંતર
# define ATTR_PRINTF(x,y)
#endif

#જો defined(__GNUC__)
# define ATTR_NORETURN __attribute__((__noreturn__))
#નહીંતર
# define ATTR_NORETURN
#endif /* GNUC  */

#જો defined(__GNUC__)
#define ATTR_MALLOC  __attribute__((__malloc__))
#નહીંતર
#define ATTR_MALLOC
#endif

#જો defined(__GNUC__)
# define ATTR_NULL_SENTINEL __attribute__((__sentinel__))
#નહીંતર
# define ATTR_NULL_SENTINEL
#endif

#જો defined(__GNUC__)
#define ATTR_PACKED  __attribute__((__packed__))
#નહીંતર
#define ATTR_PACKED
#endif

#જો defined(HAVE_ATTR_COUNTED_BY) && HAVE_ATTR_COUNTED_BY
# define ATTR_COUNTED_BY(ગણતરી) __attribute__((counted_by(ગણતરી)))
#નહીંતર
# define ATTR_COUNTED_BY(ગણતરી)
#endif

#જો !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
XKB_EXPORT_PRIVATE પૂર્ણાંક asprintf(અક્ષર **strp, અચલ અક્ષર *fmt, ...) ATTR_PRINTF(2, 3);
# જો !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_PRIVATE પૂર્ણાંક vasprintf(અક્ષર **strp, અચલ અક્ષર *fmt, va_list ap);
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */

સ્થિર અંતઃરેખ bool
ATTR_PRINTF(3, 4)
snprintf_safe(અક્ષર *buf, size_t sz, અચલ અક્ષર *format, ...)
{
    va_list ap;
    પૂર્ણાંક rc;

    va_start(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    va_end(ap);

    પાછા rc >= 0 && (size_t)rc < sz;
}

સ્થિર અંતઃરેખ અક્ષર *
ATTR_PRINTF(1, 0)
vasprintf_safe(અચલ અક્ષર *fmt, va_list args)
{
    અક્ષર *str;
    પૂર્ણાંક len;

    len = vasprintf(&str, fmt, args);

    જો (len == -1)
        પાછા ખાલી;

    પાછા str;
}

/**
 * A version of asprintf that returns the allocated string or NULL on error.
 */
સ્થિર અંતઃરેખ અક્ષર *
ATTR_PRINTF(1, 2)
asprintf_safe(અચલ અક્ષર *fmt, ...)
{
    va_list args;
    અક્ષર *str;

    va_start(args, fmt);
    str = vasprintf_safe(fmt, args);
    va_end(args);

    પાછા str;
}

#ifdef NDEBUG
    #જો defined(__GNUC__) || defined(__clang__)
        #define PANIC_UNREACHABLE() __builtin_unreachable()
    #અથવાજો defined(_MSC_VER)
        #define PANIC_UNREACHABLE() __assume(0)
    #નહીંતર
        #define PANIC_UNREACHABLE() abort()
    #endif
#નહીંતર
    #define PANIC_UNREACHABLE() \
        કરો { \
            fprintf(stderr, "Critical Error: Reached unreachable line in %s at %d\n", __FILE__, __LINE__); \
            abort(); \
        } જ્યારે (0)
#endif

#જો defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#અથવાજો defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#અથવાજો defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#નહીંતર
#  define NOINLINE
#endif
