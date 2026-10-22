/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <errno.h>
#ضمن <inttypes.h>
#ضمن <stdarg.h>
#ضمن <stdbool.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>
#אם HAVE_UNISTD_H
# include <unistd.h>
#אחרת
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
تعريفنوع SSIZE_T ssize_t;
#endif
#endif

ساكن مضمن bool
is_aligned(ثابت فراغ *restrict pointer, size_t byte_count)
{
    החזר (uintptr_t) pointer % byte_count == 0;
}


#define ARRAY_SIZE(arr) ((حجم(arr) / حجم(*(arr))))

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* Round up @a so it's divisible by @b. */
#define ROUNDUP(a, b) (((a) + (b) - 1) / (b) * (b))

#define STRINGIFY(x) #x
#define STRINGIFY2(x) STRINGIFY(x)
#define CONCAT(x,y) x ## y
#define CONCAT2(x,y) CONCAT(x,y)

/* Check if a Unicode code point is valid in a string literal */
ساكن مضمن bool
is_valid_char(uint32_t cp)
{
    /* Currently we only check for NULL character, but this could be extended
     * in the future to further ASCII control characters. */
    החזר cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
ساكن مضمن bool
is_surrogate(uint32_t cp)
{
    החזר (cp >= 0xd800 && cp <= 0xdfff);
}

حرف
to_lower(حرف c);

صحيح
istrcmp(ثابت حرف *a, ثابت حرف *b);

صحيح
istrncmp(ثابت حرف *a, ثابت حرف *b, size_t n);

ساكن مضمن bool
streq(ثابت حرف *s1, ثابت حرف *s2)
{
    أكد(s1 && s2);
    החזר strcmp(s1, s2) == 0;
}

ساكن مضمن bool
streq_null(ثابت حرف *s1, ثابت حرف *s2)
{
    אם (s1 == عدم || s2 == عدم)
        החזר s1 == s2;
    החזר streq(s1, s2);
}

ساكن مضمن bool
streq_not_null(ثابت حرف *s1, ثابت حرف *s2)
{
    אם (!s1 || !s2)
        החזר false;
    החזר streq(s1, s2);
}

ساكن مضمن bool
istreq(ثابت حرف *s1, ثابت حرف *s2)
{
    החזר istrcmp(s1, s2) == 0;
}

ساكن مضمن bool
istrneq(ثابت حرف *s1, ثابت حرف *s2, size_t len)
{
    החזר istrncmp(s1, s2, len) == 0;
}

#define istreq_prefix(s1, s2) istrneq(s1, s2, حجم(s1) - 1)

ساكن مضمن حرف *
strcpy_safe(حرف *dest, size_t size, ثابت حرف *src) {
    אם (!dest || !size || !src)
        החזר عدم;
    ثابت حرف * ثابت limit = dest + size - 1;
    כלעוד (dest < limit && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    החזר *src ? عدم : dest;
}

ساكن مضمن حرف *
strdup_safe(ثابت حرف *s)
{
    החזר s ? strdup(s) : عدم;
}

ساكن مضمن size_t
strlen_safe(ثابت حرف *s)
{
    החזר s ? strlen(s) : 0;
}

ساكن مضمن bool
isempty(ثابت حرف *s)
{
    החזר s == عدم || s[0] == '\0';
}

ساكن مضمن ثابت حرف *
strnull(ثابت حرف *s)
{
    החזר s ? s : "(null)";
}

ساكن مضمن ثابت حرف *
strempty(ثابت حرف *s)
{
    החזר s ? s : "";
}

ساكن مضمن فراغ *
memdup(ثابت فراغ *mem, size_t nmemb, size_t size)
{
    فراغ *p = calloc(nmemb, size);
    אם (p)
        memcpy(p, mem, nmemb * size);
    החזר p;
}

#אם !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
ساكن مضمن حرف *
strndup(ثابت حرف *s, size_t n)
{
    size_t slen = strlen(s);
    size_t len = MIN(slen, n);
    حرف *p = malloc(len + 1);
    אם (!p)
        החזר عدم;
    memcpy(p, s, len);
    p[len] = '\0';
    החזר p;
}
#endif

/* ctype.h is locale-dependent and has other oddities. */
ساكن مضمن bool
is_ascii(حرف ch)
{
    החזר (ch & ~0x7f) == 0;
}

ساكن مضمن bool
is_space(حرف ch)
{
    החזר ch == ' ' || (ch >= '\t' && ch <= '\r');
}

ساكن مضمن bool
is_alpha(حرف ch)
{
    החזר (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

ساكن مضمن bool
is_digit(حرف ch)
{
    החזר ch >= '0' && ch <= '9';
}

ساكن مضمن bool
is_alnum(حرف ch)
{
    החזר is_alpha(ch) || is_digit(ch);
}

ساكن مضمن bool
is_xdigit(حرف ch)
{
    החזר
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

ساكن مضمن bool
is_graph(حرف ch)
{
    /* See table in ascii(7). */
    החזר ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
ساكن مضمن unsigned صحيح
msb_pos(uint32_t mask)
{
    unsigned صحيح pos = 0;
    כלעוד (mask) {
        pos++;
        mask >>= 1u;
    }
    החזר pos;
}

ساكن مضمن صحيح
one_bit_set(uint32_t x)
{
    החזר x && (x & (x - 1)) == 0;
}

bool
map_file(FILE *file, حرف **string_out, size_t *size_out);

فراغ
unmap_file(حرف *string, size_t size);

ساكن مضمن bool
check_eaccess(ثابت حرف *path, صحيح mode)
{
#אם defined(HAVE_EACCESS)
    אם (eaccess(path, mode) != 0)
        החזר false;
#وإلاإذا defined(HAVE_EUIDACCESS)
    אם (euidaccess(path, mode) != 0)
        החזר false;
#endif

    החזר true;
}

FILE*
open_file(ثابت حرف *path);

#אם defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#وإلاإذا defined(HAVE___SECURE_GETENV)
# define secure_getenv __secure_getenv
#אחרת
# define secure_getenv getenv
#endif

#אם defined(HAVE___BUILTIN_EXPECT)
# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#אחרת
# define likely(x)   (x)
# define unlikely(x) (x)
#endif

/* Compiler Attributes */

/* Private functions only exposed in tests. */
#ifdef ENABLE_PRIVATE_APIS
# אם defined(__GNUC__) && !defined(__CYGWIN__)
#  define XKB_EXPORT_PRIVATE __attribute__((visibility("default")))
# elif defined(_WIN32)
#  define XKB_EXPORT_PRIVATE __declspec(dllexport)
# אחרת
#  define XKB_EXPORT_PRIVATE
# endif
#אחרת
# define XKB_EXPORT_PRIVATE
#endif

#אם defined(__MINGW32__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__MINGW_PRINTF_FORMAT, x, y)))
#وإلاإذا defined(__GNUC__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__printf__, x, y)))
#אחרת
# define ATTR_PRINTF(x,y)
#endif

#אם defined(__GNUC__)
# define ATTR_NORETURN __attribute__((__noreturn__))
#אחרת
# define ATTR_NORETURN
#endif /* GNUC  */

#אם defined(__GNUC__)
#define ATTR_MALLOC  __attribute__((__malloc__))
#אחרת
#define ATTR_MALLOC
#endif

#אם defined(__GNUC__)
# define ATTR_NULL_SENTINEL __attribute__((__sentinel__))
#אחרת
# define ATTR_NULL_SENTINEL
#endif

#אם defined(__GNUC__)
#define ATTR_PACKED  __attribute__((__packed__))
#אחרת
#define ATTR_PACKED
#endif

#אם defined(HAVE_ATTR_COUNTED_BY) && HAVE_ATTR_COUNTED_BY
# define ATTR_COUNTED_BY(count) __attribute__((counted_by(count)))
#אחרת
# define ATTR_COUNTED_BY(count)
#endif

#אם !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
XKB_EXPORT_PRIVATE صحيح asprintf(حرف **strp, ثابت حرف *fmt, ...) ATTR_PRINTF(2, 3);
# אם !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_PRIVATE صحيح vasprintf(حرف **strp, ثابت حرف *fmt, va_list ap);
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */

ساكن مضمن bool
ATTR_PRINTF(3, 4)
snprintf_safe(حرف *buf, size_t sz, ثابت حرف *format, ...)
{
    va_list ap;
    صحيح rc;

    va_start(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    va_end(ap);

    החזר rc >= 0 && (size_t)rc < sz;
}

ساكن مضمن حرف *
ATTR_PRINTF(1, 0)
vasprintf_safe(ثابت حرف *fmt, va_list args)
{
    حرف *str;
    صحيح len;

    len = vasprintf(&str, fmt, args);

    אם (len == -1)
        החזר عدم;

    החזר str;
}

/**
 * A version of asprintf that returns the allocated string or NULL on error.
 */
ساكن مضمن حرف *
ATTR_PRINTF(1, 2)
asprintf_safe(ثابت حرف *fmt, ...)
{
    va_list args;
    حرف *str;

    va_start(args, fmt);
    str = vasprintf_safe(fmt, args);
    va_end(args);

    החזר str;
}

#ifdef NDEBUG
    #אם defined(__GNUC__) || defined(__clang__)
        #define PANIC_UNREACHABLE() __builtin_unreachable()
    #وإلاإذا defined(_MSC_VER)
        #define PANIC_UNREACHABLE() __assume(0)
    #אחרת
        #define PANIC_UNREACHABLE() abort()
    #endif
#אחרת
    #define PANIC_UNREACHABLE() \
        עשה { \
            fprintf(stderr, "Critical Error: Reached unreachable line in %s at %d\n", __FILE__, __LINE__); \
            abort(); \
        } כלעוד (0)
#endif

#אם defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#وإلاإذا defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#وإلاإذا defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#אחרת
#  define NOINLINE
#endif
