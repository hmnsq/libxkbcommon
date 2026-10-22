/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <inttypes.h>
#অন্তর্ভাব <stdarg.h>
#অন্তর্ভাব <stdbool.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
# include <unistd.h>
#অন্যথা
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
প্রকারলেখ SSIZE_T ssize_t;
#endif
#endif

স্থির অন্তঃরেখ bool
is_aligned(ধ্রুবক শূন্য *restrict pointer, আকার_t byte_গুনতি)
{
    ফেরত (uintptr_t) pointer % byte_গুনতি == 0;
}


#define ARRAY_আকার(arr) ((আকার(arr) / আকার(*(arr))))

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* Round up @a so it's divisible by @b. */
#define ROUNDUP(a, b) (((a) + (b) - 1) / (b) * (b))

#define STRINGIFY(x) #x
#define STRINGIFY2(x) STRINGIFY(x)
#define CONCAT(x,y) x ## y
#define CONCAT2(x,y) CONCAT(x,y)

/* Check if a Unicode code point is valid in a string literal */
স্থির অন্তঃরেখ bool
is_valid_char(uint32_t cp)
{
    /* Currently we only check for NULL character, but this could be extended
     * in the future to further ASCII control characters. */
    ফেরত cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
স্থির অন্তঃরেখ bool
is_surrogate(uint32_t cp)
{
    ফেরত (cp >= 0xd800 && cp <= 0xdfff);
}

অক্ষর
to_lower(অক্ষর c);

পূর্ণসংখ্যা
istrcmp(ধ্রুবক অক্ষর *a, ধ্রুবক অক্ষর *b);

পূর্ণসংখ্যা
istrncmp(ধ্রুবক অক্ষর *a, ধ্রুবক অক্ষর *b, আকার_t n);

স্থির অন্তঃরেখ bool
streq(ধ্রুবক অক্ষর *s1, ধ্রুবক অক্ষর *s2)
{
    প্রতিজ্ঞা(s1 && s2);
    ফেরত strcmp(s1, s2) == 0;
}

স্থির অন্তঃরেখ bool
streq_null(ধ্রুবক অক্ষর *s1, ধ্রুবক অক্ষর *s2)
{
    যদি (s1 == ফাঁকা || s2 == ফাঁকা)
        ফেরত s1 == s2;
    ফেরত streq(s1, s2);
}

স্থির অন্তঃরেখ bool
streq_not_null(ধ্রুবক অক্ষর *s1, ধ্রুবক অক্ষর *s2)
{
    যদি (!s1 || !s2)
        ফেরত false;
    ফেরত streq(s1, s2);
}

স্থির অন্তঃরেখ bool
istreq(ধ্রুবক অক্ষর *s1, ধ্রুবক অক্ষর *s2)
{
    ফেরত istrcmp(s1, s2) == 0;
}

স্থির অন্তঃরেখ bool
istrneq(ধ্রুবক অক্ষর *s1, ধ্রুবক অক্ষর *s2, আকার_t len)
{
    ফেরত istrncmp(s1, s2, len) == 0;
}

#define istreq_prefix(s1, s2) istrneq(s1, s2, আকার(s1) - 1)

স্থির অন্তঃরেখ অক্ষর *
strcpy_safe(অক্ষর *dest, আকার_t আকার_2, ধ্রুবক অক্ষর *src) {
    যদি (!dest || !আকার_2 || !src)
        ফেরত ফাঁকা;
    ধ্রুবক অক্ষর * ধ্রুবক limit = dest + আকার_2 - 1;
    যখন (dest < limit && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    ফেরত *src ? ফাঁকা : dest;
}

স্থির অন্তঃরেখ অক্ষর *
strdup_safe(ধ্রুবক অক্ষর *s)
{
    ফেরত s ? strdup(s) : ফাঁকা;
}

স্থির অন্তঃরেখ আকার_t
strlen_safe(ধ্রুবক অক্ষর *s)
{
    ফেরত s ? strlen(s) : 0;
}

স্থির অন্তঃরেখ bool
isempty(ধ্রুবক অক্ষর *s)
{
    ফেরত s == ফাঁকা || s[0] == '\0';
}

স্থির অন্তঃরেখ ধ্রুবক অক্ষর *
strnull(ধ্রুবক অক্ষর *s)
{
    ফেরত s ? s : "(null)";
}

স্থির অন্তঃরেখ ধ্রুবক অক্ষর *
strempty(ধ্রুবক অক্ষর *s)
{
    ফেরত s ? s : "";
}

স্থির অন্তঃরেখ শূন্য *
memdup(ধ্রুবক শূন্য *mem, আকার_t nmemb, আকার_t আকার_2)
{
    শূন্য *p = calloc(nmemb, আকার_2);
    যদি (p)
        memcpy(p, mem, nmemb * আকার_2);
    ফেরত p;
}

#যদি !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
স্থির অন্তঃরেখ অক্ষর *
strndup(ধ্রুবক অক্ষর *s, আকার_t n)
{
    আকার_t slen = strlen(s);
    আকার_t len = MIN(slen, n);
    অক্ষর *p = malloc(len + 1);
    যদি (!p)
        ফেরত ফাঁকা;
    memcpy(p, s, len);
    p[len] = '\0';
    ফেরত p;
}
#endif

/* ctype.h is locale-dependent and has other oddities. */
স্থির অন্তঃরেখ bool
is_ascii(অক্ষর ch)
{
    ফেরত (ch & ~0x7f) == 0;
}

স্থির অন্তঃরেখ bool
is_space(অক্ষর ch)
{
    ফেরত ch == ' ' || (ch >= '\t' && ch <= '\r');
}

স্থির অন্তঃরেখ bool
is_alpha(অক্ষর ch)
{
    ফেরত (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

স্থির অন্তঃরেখ bool
is_digit(অক্ষর ch)
{
    ফেরত ch >= '0' && ch <= '9';
}

স্থির অন্তঃরেখ bool
is_alnum(অক্ষর ch)
{
    ফেরত is_alpha(ch) || is_digit(ch);
}

স্থির অন্তঃরেখ bool
is_xdigit(অক্ষর ch)
{
    ফেরত
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

স্থির অন্তঃরেখ bool
is_graph(অক্ষর ch)
{
    /* See table in ascii(7). */
    ফেরত ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
স্থির অন্তঃরেখ unsigned পূর্ণসংখ্যা
msb_pos(uint32_t mask)
{
    unsigned পূর্ণসংখ্যা pos = 0;
    যখন (mask) {
        pos++;
        mask >>= 1u;
    }
    ফেরত pos;
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা
one_bit_set(uint32_t x)
{
    ফেরত x && (x & (x - 1)) == 0;
}

bool
map_file(FILE *file, অক্ষর **string_out, আকার_t *আকার_out);

শূন্য
unmap_file(অক্ষর *string, আকার_t আকার_2);

স্থির অন্তঃরেখ bool
check_eaccess(ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা mode)
{
#যদি defined(HAVE_EACCESS)
    যদি (eaccess(path, mode) != 0)
        ফেরত false;
#নতুবা_যদি defined(HAVE_EUIDACCESS)
    যদি (euidaccess(path, mode) != 0)
        ফেরত false;
#endif

    ফেরত true;
}

FILE*
open_file(ধ্রুবক অক্ষর *path);

#যদি defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#নতুবা_যদি defined(HAVE___SECURE_GETENV)
# define secure_getenv __secure_getenv
#অন্যথা
# define secure_getenv getenv
#endif

#যদি defined(HAVE___BUILTIN_EXPECT)
# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#অন্যথা
# define likely(x)   (x)
# define unlikely(x) (x)
#endif

/* Compiler Attributes */

/* Private functions only exposed in tests. */
#ifdef ENABLE_PRIVATE_APIS
# যদি defined(__GNUC__) && !defined(__CYGWIN__)
#  define XKB_EXPORT_PRIVATE __attribute__((visibility("default")))
# elif defined(_WIN32)
#  define XKB_EXPORT_PRIVATE __declspec(dllexport)
# অন্যথা
#  define XKB_EXPORT_PRIVATE
# endif
#অন্যথা
# define XKB_EXPORT_PRIVATE
#endif

#যদি defined(__MINGW32__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__MINGW_PRINTF_FORMAT, x, y)))
#নতুবা_যদি defined(__GNUC__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__printf__, x, y)))
#অন্যথা
# define ATTR_PRINTF(x,y)
#endif

#যদি defined(__GNUC__)
# define ATTR_NORETURN __attribute__((__noreturn__))
#অন্যথা
# define ATTR_NORETURN
#endif /* GNUC  */

#যদি defined(__GNUC__)
#define ATTR_MALLOC  __attribute__((__malloc__))
#অন্যথা
#define ATTR_MALLOC
#endif

#যদি defined(__GNUC__)
# define ATTR_NULL_SENTINEL __attribute__((__sentinel__))
#অন্যথা
# define ATTR_NULL_SENTINEL
#endif

#যদি defined(__GNUC__)
#define ATTR_PACKED  __attribute__((__packed__))
#অন্যথা
#define ATTR_PACKED
#endif

#যদি defined(HAVE_ATTR_COUNTED_BY) && HAVE_ATTR_COUNTED_BY
# define ATTR_COUNTED_BY(গুনতি) __attribute__((counted_by(গুনতি)))
#অন্যথা
# define ATTR_COUNTED_BY(গুনতি)
#endif

#যদি !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
XKB_EXPORT_PRIVATE পূর্ণসংখ্যা asprintf(অক্ষর **strp, ধ্রুবক অক্ষর *fmt, ...) ATTR_PRINTF(2, 3);
# যদি !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_PRIVATE পূর্ণসংখ্যা vasprintf(অক্ষর **strp, ধ্রুবক অক্ষর *fmt, va_list ap);
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */

স্থির অন্তঃরেখ bool
ATTR_PRINTF(3, 4)
snprintf_safe(অক্ষর *buf, আকার_t sz, ধ্রুবক অক্ষর *format, ...)
{
    va_list ap;
    পূর্ণসংখ্যা rc;

    va_start(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    va_end(ap);

    ফেরত rc >= 0 && (আকার_t)rc < sz;
}

স্থির অন্তঃরেখ অক্ষর *
ATTR_PRINTF(1, 0)
vasprintf_safe(ধ্রুবক অক্ষর *fmt, va_list args)
{
    অক্ষর *str;
    পূর্ণসংখ্যা len;

    len = vasprintf(&str, fmt, args);

    যদি (len == -1)
        ফেরত ফাঁকা;

    ফেরত str;
}

/**
 * A version of asprintf that returns the allocated string or NULL on error.
 */
স্থির অন্তঃরেখ অক্ষর *
ATTR_PRINTF(1, 2)
asprintf_safe(ধ্রুবক অক্ষর *fmt, ...)
{
    va_list args;
    অক্ষর *str;

    va_start(args, fmt);
    str = vasprintf_safe(fmt, args);
    va_end(args);

    ফেরত str;
}

#ifdef NDEBUG
    #যদি defined(__GNUC__) || defined(__clang__)
        #define PANIC_UNREACHABLE() __builtin_unreachable()
    #নতুবা_যদি defined(_MSC_VER)
        #define PANIC_UNREACHABLE() __assume(0)
    #অন্যথা
        #define PANIC_UNREACHABLE() abort()
    #endif
#অন্যথা
    #define PANIC_UNREACHABLE() \
        করো { \
            fprintf(stderr, "Critical Error: Reached unreachable line in %s at %d\n", __FILE__, __LINE__); \
            abort(); \
        } যখন (0)
#endif

#যদি defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#নতুবা_যদি defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#নতুবা_যদি defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#অন্যথা
#  define NOINLINE
#endif
