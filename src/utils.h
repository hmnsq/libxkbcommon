/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <inttypes.h>
#அந்தர்பாவ <stdarg.h>
#அந்தர்பாவ <stdbool.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
# include <unistd.h>
#இல்லையெனில்
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
வகைவரை SSIZE_T ssize_t;
#endif
#endif

நிலையான உள்வரி bool
is_aligned(மாறிலி வெறுமை *restrict pointer, size_t byte_மொத்தம்)
{
    திரும்பு (uintptr_t) pointer % byte_மொத்தம் == 0;
}


#define ARRAY_SIZE(arr) ((அளவு(arr) / அளவு(*(arr))))

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* Round up @a so it's divisible by @b. */
#define ROUNDUP(a, b) (((a) + (b) - 1) / (b) * (b))

#define STRINGIFY(x) #x
#define STRINGIFY2(x) STRINGIFY(x)
#define CONCAT(x,y) x ## y
#define CONCAT2(x,y) CONCAT(x,y)

/* Check if a Unicode code point is valid in a string literal */
நிலையான உள்வரி bool
is_valid_char(uint32_t cp)
{
    /* Currently we only check for NULL character, but this could be extended
     * in the future to further ASCII control characters. */
    திரும்பு cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
நிலையான உள்வரி bool
is_surrogate(uint32_t cp)
{
    திரும்பு (cp >= 0xd800 && cp <= 0xdfff);
}

எழுத்து
to_lower(எழுத்து c);

முழு
istrcmp(மாறிலி எழுத்து *a, மாறிலி எழுத்து *b);

முழு
istrncmp(மாறிலி எழுத்து *a, மாறிலி எழுத்து *b, size_t n);

நிலையான உள்வரி bool
streq(மாறிலி எழுத்து *s1, மாறிலி எழுத்து *s2)
{
    ப்ரதிஜ்ஞா(s1 && s2);
    திரும்பு strcmp(s1, s2) == 0;
}

நிலையான உள்வரி bool
streq_null(மாறிலி எழுத்து *s1, மாறிலி எழுத்து *s2)
{
    எனில் (s1 == வெற்று || s2 == வெற்று)
        திரும்பு s1 == s2;
    திரும்பு streq(s1, s2);
}

நிலையான உள்வரி bool
streq_not_null(மாறிலி எழுத்து *s1, மாறிலி எழுத்து *s2)
{
    எனில் (!s1 || !s2)
        திரும்பு false;
    திரும்பு streq(s1, s2);
}

நிலையான உள்வரி bool
istreq(மாறிலி எழுத்து *s1, மாறிலி எழுத்து *s2)
{
    திரும்பு istrcmp(s1, s2) == 0;
}

நிலையான உள்வரி bool
istrneq(மாறிலி எழுத்து *s1, மாறிலி எழுத்து *s2, size_t len)
{
    திரும்பு istrncmp(s1, s2, len) == 0;
}

#define istreq_prefix(s1, s2) istrneq(s1, s2, அளவு(s1) - 1)

நிலையான உள்வரி எழுத்து *
strcpy_safe(எழுத்து *dest, size_t size, மாறிலி எழுத்து *src) {
    எனில் (!dest || !size || !src)
        திரும்பு வெற்று;
    மாறிலி எழுத்து * மாறிலி limit = dest + size - 1;
    போது (dest < limit && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    திரும்பு *src ? வெற்று : dest;
}

நிலையான உள்வரி எழுத்து *
strdup_safe(மாறிலி எழுத்து *s)
{
    திரும்பு s ? strdup(s) : வெற்று;
}

நிலையான உள்வரி size_t
strlen_safe(மாறிலி எழுத்து *s)
{
    திரும்பு s ? strlen(s) : 0;
}

நிலையான உள்வரி bool
isempty(மாறிலி எழுத்து *s)
{
    திரும்பு s == வெற்று || s[0] == '\0';
}

நிலையான உள்வரி மாறிலி எழுத்து *
strnull(மாறிலி எழுத்து *s)
{
    திரும்பு s ? s : "(null)";
}

நிலையான உள்வரி மாறிலி எழுத்து *
strempty(மாறிலி எழுத்து *s)
{
    திரும்பு s ? s : "";
}

நிலையான உள்வரி வெறுமை *
memdup(மாறிலி வெறுமை *mem, size_t nmemb, size_t size)
{
    வெறுமை *p = calloc(nmemb, size);
    எனில் (p)
        memcpy(p, mem, nmemb * size);
    திரும்பு p;
}

#எனில் !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
நிலையான உள்வரி எழுத்து *
strndup(மாறிலி எழுத்து *s, size_t n)
{
    size_t slen = strlen(s);
    size_t len = MIN(slen, n);
    எழுத்து *p = malloc(len + 1);
    எனில் (!p)
        திரும்பு வெற்று;
    memcpy(p, s, len);
    p[len] = '\0';
    திரும்பு p;
}
#endif

/* ctype.h is locale-dependent and has other oddities. */
நிலையான உள்வரி bool
is_ascii(எழுத்து ch)
{
    திரும்பு (ch & ~0x7f) == 0;
}

நிலையான உள்வரி bool
is_space(எழுத்து ch)
{
    திரும்பு ch == ' ' || (ch >= '\t' && ch <= '\r');
}

நிலையான உள்வரி bool
is_alpha(எழுத்து ch)
{
    திரும்பு (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

நிலையான உள்வரி bool
is_digit(எழுத்து ch)
{
    திரும்பு ch >= '0' && ch <= '9';
}

நிலையான உள்வரி bool
is_alnum(எழுத்து ch)
{
    திரும்பு is_alpha(ch) || is_digit(ch);
}

நிலையான உள்வரி bool
is_xdigit(எழுத்து ch)
{
    திரும்பு
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

நிலையான உள்வரி bool
is_graph(எழுத்து ch)
{
    /* See table in ascii(7). */
    திரும்பு ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
நிலையான உள்வரி unsigned முழு
msb_pos(uint32_t mask)
{
    unsigned முழு pos = 0;
    போது (mask) {
        pos++;
        mask >>= 1u;
    }
    திரும்பு pos;
}

நிலையான உள்வரி முழு
one_bit_set(uint32_t x)
{
    திரும்பு x && (x & (x - 1)) == 0;
}

bool
map_file(FILE *file, எழுத்து **string_out, size_t *size_out);

வெறுமை
unmap_file(எழுத்து *string, size_t size);

நிலையான உள்வரி bool
check_eaccess(மாறிலி எழுத்து *path, முழு mode)
{
#எனில் defined(HAVE_EACCESS)
    எனில் (eaccess(path, mode) != 0)
        திரும்பு false;
#அல்லதெனில் defined(HAVE_EUIDACCESS)
    எனில் (euidaccess(path, mode) != 0)
        திரும்பு false;
#endif

    திரும்பு true;
}

FILE*
open_file(மாறிலி எழுத்து *path);

#எனில் defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#அல்லதெனில் defined(HAVE___SECURE_GETENV)
# define secure_getenv __secure_getenv
#இல்லையெனில்
# define secure_getenv getenv
#endif

#எனில் defined(HAVE___BUILTIN_EXPECT)
# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#இல்லையெனில்
# define likely(x)   (x)
# define unlikely(x) (x)
#endif

/* Compiler Attributes */

/* Private functions only exposed in tests. */
#ifdef ENABLE_PRIVATE_APIS
# எனில் defined(__GNUC__) && !defined(__CYGWIN__)
#  define XKB_EXPORT_PRIVATE __attribute__((visibility("default")))
# elif defined(_WIN32)
#  define XKB_EXPORT_PRIVATE __declspec(dllexport)
# இல்லையெனில்
#  define XKB_EXPORT_PRIVATE
# endif
#இல்லையெனில்
# define XKB_EXPORT_PRIVATE
#endif

#எனில் defined(__MINGW32__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__MINGW_PRINTF_FORMAT, x, y)))
#அல்லதெனில் defined(__GNUC__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__printf__, x, y)))
#இல்லையெனில்
# define ATTR_PRINTF(x,y)
#endif

#எனில் defined(__GNUC__)
# define ATTR_NORETURN __attribute__((__noreturn__))
#இல்லையெனில்
# define ATTR_NORETURN
#endif /* GNUC  */

#எனில் defined(__GNUC__)
#define ATTR_MALLOC  __attribute__((__malloc__))
#இல்லையெனில்
#define ATTR_MALLOC
#endif

#எனில் defined(__GNUC__)
# define ATTR_NULL_SENTINEL __attribute__((__sentinel__))
#இல்லையெனில்
# define ATTR_NULL_SENTINEL
#endif

#எனில் defined(__GNUC__)
#define ATTR_PACKED  __attribute__((__packed__))
#இல்லையெனில்
#define ATTR_PACKED
#endif

#எனில் defined(HAVE_ATTR_COUNTED_BY) && HAVE_ATTR_COUNTED_BY
# define ATTR_COUNTED_BY(மொத்தம்) __attribute__((counted_by(மொத்தம்)))
#இல்லையெனில்
# define ATTR_COUNTED_BY(மொத்தம்)
#endif

#எனில் !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
XKB_EXPORT_PRIVATE முழு asprintf(எழுத்து **strp, மாறிலி எழுத்து *fmt, ...) ATTR_PRINTF(2, 3);
# எனில் !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_PRIVATE முழு vasprintf(எழுத்து **strp, மாறிலி எழுத்து *fmt, va_list ap);
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */

நிலையான உள்வரி bool
ATTR_PRINTF(3, 4)
snprintf_safe(எழுத்து *buf, size_t sz, மாறிலி எழுத்து *format, ...)
{
    va_list ap;
    முழு rc;

    va_start(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    va_end(ap);

    திரும்பு rc >= 0 && (size_t)rc < sz;
}

நிலையான உள்வரி எழுத்து *
ATTR_PRINTF(1, 0)
vasprintf_safe(மாறிலி எழுத்து *fmt, va_list args)
{
    எழுத்து *str;
    முழு len;

    len = vasprintf(&str, fmt, args);

    எனில் (len == -1)
        திரும்பு வெற்று;

    திரும்பு str;
}

/**
 * A version of asprintf that returns the allocated string or NULL on error.
 */
நிலையான உள்வரி எழுத்து *
ATTR_PRINTF(1, 2)
asprintf_safe(மாறிலி எழுத்து *fmt, ...)
{
    va_list args;
    எழுத்து *str;

    va_start(args, fmt);
    str = vasprintf_safe(fmt, args);
    va_end(args);

    திரும்பு str;
}

#ifdef NDEBUG
    #எனில் defined(__GNUC__) || defined(__clang__)
        #define PANIC_UNREACHABLE() __builtin_unreachable()
    #அல்லதெனில் defined(_MSC_VER)
        #define PANIC_UNREACHABLE() __assume(0)
    #இல்லையெனில்
        #define PANIC_UNREACHABLE() abort()
    #endif
#இல்லையெனில்
    #define PANIC_UNREACHABLE() \
        செய் { \
            fprintf(stderr, "Critical Error: Reached unreachable line in %s at %d\n", __FILE__, __LINE__); \
            abort(); \
        } போது (0)
#endif

#எனில் defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#அல்லதெனில் defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#அல்லதெனில் defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#இல்லையெனில்
#  define NOINLINE
#endif
