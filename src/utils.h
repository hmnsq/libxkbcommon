/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <inttypes.h>
#അന്തര്ഭാവ <stdarg.h>
#അന്തര്ഭാവ <stdbool.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
# include <unistd.h>
#അല്ലെങ്കിൽ
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
തരനിർവചനം SSIZE_T ssize_t;
#endif
#endif

സ്ഥിതം വരിയിൽ bool
is_aligned(സ്ഥിരം ശൂന്യം *restrict pointer, size_t byte_എണ്ണം)
{
    തിരികെ (uintptr_t) pointer % byte_എണ്ണം == 0;
}


#define ARRAY_SIZE(arr) ((വലുപ്പം(arr) / വലുപ്പം(*(arr))))

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* Round up @a so it's divisible by @b. */
#define ROUNDUP(a, b) (((a) + (b) - 1) / (b) * (b))

#define STRINGIFY(x) #x
#define STRINGIFY2(x) STRINGIFY(x)
#define CONCAT(x,y) x ## y
#define CONCAT2(x,y) CONCAT(x,y)

/* Check if a Unicode code point is valid in a string literal */
സ്ഥിതം വരിയിൽ bool
is_valid_char(uint32_t cp)
{
    /* Currently we only check for NULL character, but this could be extended
     * in the future to further ASCII control characters. */
    തിരികെ cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
സ്ഥിതം വരിയിൽ bool
is_surrogate(uint32_t cp)
{
    തിരികെ (cp >= 0xd800 && cp <= 0xdfff);
}

അക്ഷരം
to_lower(അക്ഷരം c);

പൂർണ്ണസംഖ്യ
istrcmp(സ്ഥിരം അക്ഷരം *a, സ്ഥിരം അക്ഷരം *b);

പൂർണ്ണസംഖ്യ
istrncmp(സ്ഥിരം അക്ഷരം *a, സ്ഥിരം അക്ഷരം *b, size_t n);

സ്ഥിതം വരിയിൽ bool
streq(സ്ഥിരം അക്ഷരം *s1, സ്ഥിരം അക്ഷരം *s2)
{
    പ്രതിജ്ഞാ(s1 && s2);
    തിരികെ strcmp(s1, s2) == 0;
}

സ്ഥിതം വരിയിൽ bool
streq_null(സ്ഥിരം അക്ഷരം *s1, സ്ഥിരം അക്ഷരം *s2)
{
    എങ്കിൽ (s1 == ശൂന്യ || s2 == ശൂന്യ)
        തിരികെ s1 == s2;
    തിരികെ streq(s1, s2);
}

സ്ഥിതം വരിയിൽ bool
streq_not_null(സ്ഥിരം അക്ഷരം *s1, സ്ഥിരം അക്ഷരം *s2)
{
    എങ്കിൽ (!s1 || !s2)
        തിരികെ false;
    തിരികെ streq(s1, s2);
}

സ്ഥിതം വരിയിൽ bool
istreq(സ്ഥിരം അക്ഷരം *s1, സ്ഥിരം അക്ഷരം *s2)
{
    തിരികെ istrcmp(s1, s2) == 0;
}

സ്ഥിതം വരിയിൽ bool
istrneq(സ്ഥിരം അക്ഷരം *s1, സ്ഥിരം അക്ഷരം *s2, size_t len)
{
    തിരികെ istrncmp(s1, s2, len) == 0;
}

#define istreq_prefix(s1, s2) istrneq(s1, s2, വലുപ്പം(s1) - 1)

സ്ഥിതം വരിയിൽ അക്ഷരം *
strcpy_safe(അക്ഷരം *dest, size_t size, സ്ഥിരം അക്ഷരം *src) {
    എങ്കിൽ (!dest || !size || !src)
        തിരികെ ശൂന്യ;
    സ്ഥിരം അക്ഷരം * സ്ഥിരം limit = dest + size - 1;
    ആകുമ്പോൾ (dest < limit && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    തിരികെ *src ? ശൂന്യ : dest;
}

സ്ഥിതം വരിയിൽ അക്ഷരം *
strdup_safe(സ്ഥിരം അക്ഷരം *s)
{
    തിരികെ s ? strdup(s) : ശൂന്യ;
}

സ്ഥിതം വരിയിൽ size_t
strlen_safe(സ്ഥിരം അക്ഷരം *s)
{
    തിരികെ s ? strlen(s) : 0;
}

സ്ഥിതം വരിയിൽ bool
isempty(സ്ഥിരം അക്ഷരം *s)
{
    തിരികെ s == ശൂന്യ || s[0] == '\0';
}

സ്ഥിതം വരിയിൽ സ്ഥിരം അക്ഷരം *
strnull(സ്ഥിരം അക്ഷരം *s)
{
    തിരികെ s ? s : "(null)";
}

സ്ഥിതം വരിയിൽ സ്ഥിരം അക്ഷരം *
strempty(സ്ഥിരം അക്ഷരം *s)
{
    തിരികെ s ? s : "";
}

സ്ഥിതം വരിയിൽ ശൂന്യം *
memdup(സ്ഥിരം ശൂന്യം *mem, size_t nmemb, size_t size)
{
    ശൂന്യം *p = calloc(nmemb, size);
    എങ്കിൽ (p)
        memcpy(p, mem, nmemb * size);
    തിരികെ p;
}

#എങ്കിൽ !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
സ്ഥിതം വരിയിൽ അക്ഷരം *
strndup(സ്ഥിരം അക്ഷരം *s, size_t n)
{
    size_t slen = strlen(s);
    size_t len = MIN(slen, n);
    അക്ഷരം *p = malloc(len + 1);
    എങ്കിൽ (!p)
        തിരികെ ശൂന്യ;
    memcpy(p, s, len);
    p[len] = '\0';
    തിരികെ p;
}
#endif

/* ctype.h is locale-dependent and has other oddities. */
സ്ഥിതം വരിയിൽ bool
is_ascii(അക്ഷരം ch)
{
    തിരികെ (ch & ~0x7f) == 0;
}

സ്ഥിതം വരിയിൽ bool
is_space(അക്ഷരം ch)
{
    തിരികെ ch == ' ' || (ch >= '\t' && ch <= '\r');
}

സ്ഥിതം വരിയിൽ bool
is_alpha(അക്ഷരം ch)
{
    തിരികെ (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

സ്ഥിതം വരിയിൽ bool
is_digit(അക്ഷരം ch)
{
    തിരികെ ch >= '0' && ch <= '9';
}

സ്ഥിതം വരിയിൽ bool
is_alnum(അക്ഷരം ch)
{
    തിരികെ is_alpha(ch) || is_digit(ch);
}

സ്ഥിതം വരിയിൽ bool
is_xdigit(അക്ഷരം ch)
{
    തിരികെ
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

സ്ഥിതം വരിയിൽ bool
is_graph(അക്ഷരം ch)
{
    /* See table in ascii(7). */
    തിരികെ ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ
msb_pos(uint32_t mask)
{
    unsigned പൂർണ്ണസംഖ്യ pos = 0;
    ആകുമ്പോൾ (mask) {
        pos++;
        mask >>= 1u;
    }
    തിരികെ pos;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
one_bit_set(uint32_t x)
{
    തിരികെ x && (x & (x - 1)) == 0;
}

bool
map_file(FILE *file, അക്ഷരം **string_out, size_t *size_out);

ശൂന്യം
unmap_file(അക്ഷരം *string, size_t size);

സ്ഥിതം വരിയിൽ bool
check_eaccess(സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ mode)
{
#എങ്കിൽ defined(HAVE_EACCESS)
    എങ്കിൽ (eaccess(path, mode) != 0)
        തിരികെ false;
#അല്ലെങ്കിലെങ്കിൽ defined(HAVE_EUIDACCESS)
    എങ്കിൽ (euidaccess(path, mode) != 0)
        തിരികെ false;
#endif

    തിരികെ true;
}

FILE*
open_file(സ്ഥിരം അക്ഷരം *path);

#എങ്കിൽ defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#അല്ലെങ്കിലെങ്കിൽ defined(HAVE___SECURE_GETENV)
# define secure_getenv __secure_getenv
#അല്ലെങ്കിൽ
# define secure_getenv getenv
#endif

#എങ്കിൽ defined(HAVE___BUILTIN_EXPECT)
# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#അല്ലെങ്കിൽ
# define likely(x)   (x)
# define unlikely(x) (x)
#endif

/* Compiler Attributes */

/* Private functions only exposed in tests. */
#ifdef ENABLE_PRIVATE_APIS
# എങ്കിൽ defined(__GNUC__) && !defined(__CYGWIN__)
#  define XKB_EXPORT_PRIVATE __attribute__((visibility("default")))
# elif defined(_WIN32)
#  define XKB_EXPORT_PRIVATE __declspec(dllexport)
# അല്ലെങ്കിൽ
#  define XKB_EXPORT_PRIVATE
# endif
#അല്ലെങ്കിൽ
# define XKB_EXPORT_PRIVATE
#endif

#എങ്കിൽ defined(__MINGW32__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__MINGW_PRINTF_FORMAT, x, y)))
#അല്ലെങ്കിലെങ്കിൽ defined(__GNUC__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__printf__, x, y)))
#അല്ലെങ്കിൽ
# define ATTR_PRINTF(x,y)
#endif

#എങ്കിൽ defined(__GNUC__)
# define ATTR_NORETURN __attribute__((__noreturn__))
#അല്ലെങ്കിൽ
# define ATTR_NORETURN
#endif /* GNUC  */

#എങ്കിൽ defined(__GNUC__)
#define ATTR_MALLOC  __attribute__((__malloc__))
#അല്ലെങ്കിൽ
#define ATTR_MALLOC
#endif

#എങ്കിൽ defined(__GNUC__)
# define ATTR_NULL_SENTINEL __attribute__((__sentinel__))
#അല്ലെങ്കിൽ
# define ATTR_NULL_SENTINEL
#endif

#എങ്കിൽ defined(__GNUC__)
#define ATTR_PACKED  __attribute__((__packed__))
#അല്ലെങ്കിൽ
#define ATTR_PACKED
#endif

#എങ്കിൽ defined(HAVE_ATTR_COUNTED_BY) && HAVE_ATTR_COUNTED_BY
# define ATTR_COUNTED_BY(എണ്ണം) __attribute__((counted_by(എണ്ണം)))
#അല്ലെങ്കിൽ
# define ATTR_COUNTED_BY(എണ്ണം)
#endif

#എങ്കിൽ !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
XKB_EXPORT_PRIVATE പൂർണ്ണസംഖ്യ asprintf(അക്ഷരം **strp, സ്ഥിരം അക്ഷരം *fmt, ...) ATTR_PRINTF(2, 3);
# എങ്കിൽ !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_PRIVATE പൂർണ്ണസംഖ്യ vasprintf(അക്ഷരം **strp, സ്ഥിരം അക്ഷരം *fmt, va_list ap);
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */

സ്ഥിതം വരിയിൽ bool
ATTR_PRINTF(3, 4)
snprintf_safe(അക്ഷരം *buf, size_t sz, സ്ഥിരം അക്ഷരം *format, ...)
{
    va_list ap;
    പൂർണ്ണസംഖ്യ rc;

    va_start(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    va_end(ap);

    തിരികെ rc >= 0 && (size_t)rc < sz;
}

സ്ഥിതം വരിയിൽ അക്ഷരം *
ATTR_PRINTF(1, 0)
vasprintf_safe(സ്ഥിരം അക്ഷരം *fmt, va_list args)
{
    അക്ഷരം *str;
    പൂർണ്ണസംഖ്യ len;

    len = vasprintf(&str, fmt, args);

    എങ്കിൽ (len == -1)
        തിരികെ ശൂന്യ;

    തിരികെ str;
}

/**
 * A version of asprintf that returns the allocated string or NULL on error.
 */
സ്ഥിതം വരിയിൽ അക്ഷരം *
ATTR_PRINTF(1, 2)
asprintf_safe(സ്ഥിരം അക്ഷരം *fmt, ...)
{
    va_list args;
    അക്ഷരം *str;

    va_start(args, fmt);
    str = vasprintf_safe(fmt, args);
    va_end(args);

    തിരികെ str;
}

#ifdef NDEBUG
    #എങ്കിൽ defined(__GNUC__) || defined(__clang__)
        #define PANIC_UNREACHABLE() __builtin_unreachable()
    #അല്ലെങ്കിലെങ്കിൽ defined(_MSC_VER)
        #define PANIC_UNREACHABLE() __assume(0)
    #അല്ലെങ്കിൽ
        #define PANIC_UNREACHABLE() abort()
    #endif
#അല്ലെങ്കിൽ
    #define PANIC_UNREACHABLE() \
        ചെയ്യുക { \
            fprintf(stderr, "Critical Error: Reached unreachable line in %s at %d\n", __FILE__, __LINE__); \
            abort(); \
        } ആകുമ്പോൾ (0)
#endif

#എങ്കിൽ defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#അല്ലെങ്കിലെങ്കിൽ defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#അല്ലെങ്കിലെങ്കിൽ defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#അല്ലെങ്കിൽ
#  define NOINLINE
#endif
