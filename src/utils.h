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
প্রকারনির্দেশ SSIZE_T ssize_t;
#endif
#endif

স্থির অন্তঃস্থ bool
is_aligned(ধ্রুব শূন্য *restrict pointer, size_t byte_গুনতি)
{
    প্রত্যাগম (uintptr_t) pointer % byte_গুনতি == 0;
}


#define ARRAY_SIZE(arr) ((পরিমাণ(arr) / পরিমাণ(*(arr))))

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* Round up @a so it's divisible by @b. */
#define ROUNDUP(a, b) (((a) + (b) - 1) / (b) * (b))

#define STRINGIFY(x) #x
#define STRINGIFY2(x) STRINGIFY(x)
#define CONCAT(x,y) x ## y
#define CONCAT2(x,y) CONCAT(x,y)

/* Check if a Unicode code point is valid in a string literal */
স্থির অন্তঃস্থ bool
is_valid_char(uint32_t cp)
{
    /* Currently we only check for NULL character, but this could be extended
     * in the future to further ASCII control characters. */
    প্রত্যাগম cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
স্থির অন্তঃস্থ bool
is_surrogate(uint32_t cp)
{
    প্রত্যাগম (cp >= 0xd800 && cp <= 0xdfff);
}

বর্ণ
to_lower(বর্ণ c);

পূর্ণাঙ্ক
istrcmp(ধ্রুব বর্ণ *a, ধ্রুব বর্ণ *b);

পূর্ণাঙ্ক
istrncmp(ধ্রুব বর্ণ *a, ধ্রুব বর্ণ *b, size_t n);

স্থির অন্তঃস্থ bool
streq(ধ্রুব বর্ণ *s1, ধ্রুব বর্ণ *s2)
{
    প্রতিজ্ঞা(s1 && s2);
    প্রত্যাগম strcmp(s1, s2) == 0;
}

স্থির অন্তঃস্থ bool
streq_null(ধ্রুব বর্ণ *s1, ধ্রুব বর্ণ *s2)
{
    যদি (s1 == রিক্ত || s2 == রিক্ত)
        প্রত্যাগম s1 == s2;
    প্রত্যাগম streq(s1, s2);
}

স্থির অন্তঃস্থ bool
streq_not_null(ধ্রুব বর্ণ *s1, ধ্রুব বর্ণ *s2)
{
    যদি (!s1 || !s2)
        প্রত্যাগম false;
    প্রত্যাগম streq(s1, s2);
}

স্থির অন্তঃস্থ bool
istreq(ধ্রুব বর্ণ *s1, ধ্রুব বর্ণ *s2)
{
    প্রত্যাগম istrcmp(s1, s2) == 0;
}

স্থির অন্তঃস্থ bool
istrneq(ধ্রুব বর্ণ *s1, ধ্রুব বর্ণ *s2, size_t len)
{
    প্রত্যাগম istrncmp(s1, s2, len) == 0;
}

#define istreq_prefix(s1, s2) istrneq(s1, s2, পরিমাণ(s1) - 1)

স্থির অন্তঃস্থ বর্ণ *
strcpy_safe(বর্ণ *dest, size_t size, ধ্রুব বর্ণ *src) {
    যদি (!dest || !size || !src)
        প্রত্যাগম রিক্ত;
    ধ্রুব বর্ণ * ধ্রুব limit = dest + size - 1;
    যাবত্ (dest < limit && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    প্রত্যাগম *src ? রিক্ত : dest;
}

স্থির অন্তঃস্থ বর্ণ *
strdup_safe(ধ্রুব বর্ণ *s)
{
    প্রত্যাগম s ? strdup(s) : রিক্ত;
}

স্থির অন্তঃস্থ size_t
strlen_safe(ধ্রুব বর্ণ *s)
{
    প্রত্যাগম s ? strlen(s) : 0;
}

স্থির অন্তঃস্থ bool
isempty(ধ্রুব বর্ণ *s)
{
    প্রত্যাগম s == রিক্ত || s[0] == '\0';
}

স্থির অন্তঃস্থ ধ্রুব বর্ণ *
strnull(ধ্রুব বর্ণ *s)
{
    প্রত্যাগম s ? s : "(null)";
}

স্থির অন্তঃস্থ ধ্রুব বর্ণ *
strempty(ধ্রুব বর্ণ *s)
{
    প্রত্যাগম s ? s : "";
}

স্থির অন্তঃস্থ শূন্য *
memdup(ধ্রুব শূন্য *mem, size_t nmemb, size_t size)
{
    শূন্য *p = calloc(nmemb, size);
    যদি (p)
        memcpy(p, mem, nmemb * size);
    প্রত্যাগম p;
}

#যদি !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
স্থির অন্তঃস্থ বর্ণ *
strndup(ধ্রুব বর্ণ *s, size_t n)
{
    size_t slen = strlen(s);
    size_t len = MIN(slen, n);
    বর্ণ *p = malloc(len + 1);
    যদি (!p)
        প্রত্যাগম রিক্ত;
    memcpy(p, s, len);
    p[len] = '\0';
    প্রত্যাগম p;
}
#endif

/* ctype.h is locale-dependent and has other oddities. */
স্থির অন্তঃস্থ bool
is_ascii(বর্ণ ch)
{
    প্রত্যাগম (ch & ~0x7f) == 0;
}

স্থির অন্তঃস্থ bool
is_space(বর্ণ ch)
{
    প্রত্যাগম ch == ' ' || (ch >= '\t' && ch <= '\r');
}

স্থির অন্তঃস্থ bool
is_alpha(বর্ণ ch)
{
    প্রত্যাগম (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

স্থির অন্তঃস্থ bool
is_digit(বর্ণ ch)
{
    প্রত্যাগম ch >= '0' && ch <= '9';
}

স্থির অন্তঃস্থ bool
is_alnum(বর্ণ ch)
{
    প্রত্যাগম is_alpha(ch) || is_digit(ch);
}

স্থির অন্তঃস্থ bool
is_xdigit(বর্ণ ch)
{
    প্রত্যাগম
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

স্থির অন্তঃস্থ bool
is_graph(বর্ণ ch)
{
    /* See table in ascii(7). */
    প্রত্যাগম ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
স্থির অন্তঃস্থ unsigned পূর্ণাঙ্ক
msb_pos(uint32_t mask)
{
    unsigned পূর্ণাঙ্ক pos = 0;
    যাবত্ (mask) {
        pos++;
        mask >>= 1u;
    }
    প্রত্যাগম pos;
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক
one_bit_set(uint32_t x)
{
    প্রত্যাগম x && (x & (x - 1)) == 0;
}

bool
map_file(FILE *file, বর্ণ **string_out, size_t *size_out);

শূন্য
unmap_file(বর্ণ *string, size_t size);

স্থির অন্তঃস্থ bool
check_eaccess(ধ্রুব বর্ণ *path, পূর্ণাঙ্ক mode)
{
#যদি defined(HAVE_EACCESS)
    যদি (eaccess(path, mode) != 0)
        প্রত্যাগম false;
#নোচেদ্যদি defined(HAVE_EUIDACCESS)
    যদি (euidaccess(path, mode) != 0)
        প্রত্যাগম false;
#endif

    প্রত্যাগম true;
}

FILE*
open_file(ধ্রুব বর্ণ *path);

#যদি defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#নোচেদ্যদি defined(HAVE___SECURE_GETENV)
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
#নোচেদ্যদি defined(__GNUC__)
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
XKB_EXPORT_PRIVATE পূর্ণাঙ্ক asprintf(বর্ণ **strp, ধ্রুব বর্ণ *fmt, ...) ATTR_PRINTF(2, 3);
# যদি !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_PRIVATE পূর্ণাঙ্ক vasprintf(বর্ণ **strp, ধ্রুব বর্ণ *fmt, va_list ap);
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */

স্থির অন্তঃস্থ bool
ATTR_PRINTF(3, 4)
snprintf_safe(বর্ণ *buf, size_t sz, ধ্রুব বর্ণ *format, ...)
{
    va_list ap;
    পূর্ণাঙ্ক rc;

    va_start(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    va_end(ap);

    প্রত্যাগম rc >= 0 && (size_t)rc < sz;
}

স্থির অন্তঃস্থ বর্ণ *
ATTR_PRINTF(1, 0)
vasprintf_safe(ধ্রুব বর্ণ *fmt, va_list args)
{
    বর্ণ *str;
    পূর্ণাঙ্ক len;

    len = vasprintf(&str, fmt, args);

    যদি (len == -1)
        প্রত্যাগম রিক্ত;

    প্রত্যাগম str;
}

/**
 * A version of asprintf that returns the allocated string or NULL on error.
 */
স্থির অন্তঃস্থ বর্ণ *
ATTR_PRINTF(1, 2)
asprintf_safe(ধ্রুব বর্ণ *fmt, ...)
{
    va_list args;
    বর্ণ *str;

    va_start(args, fmt);
    str = vasprintf_safe(fmt, args);
    va_end(args);

    প্রত্যাগম str;
}

#ifdef NDEBUG
    #যদি defined(__GNUC__) || defined(__clang__)
        #define PANIC_UNREACHABLE() __builtin_unreachable()
    #নোচেদ্যদি defined(_MSC_VER)
        #define PANIC_UNREACHABLE() __assume(0)
    #অন্যথা
        #define PANIC_UNREACHABLE() abort()
    #endif
#অন্যথা
    #define PANIC_UNREACHABLE() \
        কুরু { \
            fprintf(stderr, "Critical Error: Reached unreachable line in %s at %d\n", __FILE__, __LINE__); \
            abort(); \
        } যাবত্ (0)
#endif

#যদি defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#নোচেদ্যদি defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#নোচেদ্যদি defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#অন্যথা
#  define NOINLINE
#endif
