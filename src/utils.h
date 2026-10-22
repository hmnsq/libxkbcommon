/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdarg.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
# include <unistd.h>
#ਨਹੀਂਤਾਂ
/* Required on Windows where unistd.h doesn't exist */
# define R_OK    4               /* Test for read permission.  */
# define W_OK    2               /* Test for write permission.  */
# define X_OK    1               /* Test for execute permission.  */
# define F_OK    0               /* Test for existence.  */
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
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
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _MSC_VER
ਕਿਸਮਪਰਿਭਾਸ਼ਾ SSIZE_T ssize_t;
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
is_aligned(ਅਚਲ ਖਾਲੀ *ਸੀਮਿਤ ਸੂਚਕ, ਮਾਪ_t byte_ਗਿਣਤੀ)
{
    ਵਾਪਸ (uintptr_t) ਸੂਚਕ % byte_ਗਿਣਤੀ == 0;
}


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ARRAY_ਮਾਪ(arr) ((ਆਕਾਰ(arr) / ਆਕਾਰ(*(arr))))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MIN(a, b) ((a) < (b) ? (a) : (b))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX(a, b) ((a) > (b) ? (a) : (b))

/* Round up @a so it's divisible by @b. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ROUNDUP(a, b) (((a) + (b) - 1) / (b) * (b))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STRINGIFY(x) #x
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STRINGIFY2(x) STRINGIFY(x)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CONCAT(x,y) x ## y
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CONCAT2(x,y) CONCAT(x,y)

/* Check if a Unicode code point is valid in a string literal */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool
is_valid_char(uint32_t cp)
{
    /* Currently we only check for NULL character, but this could be extended
     * in the future to further ASCII control characters. */
    ਵਾਪਸ cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool
is_surrogate(uint32_t cp)
{
    ਵਾਪਸ (cp >= 0xd800 && cp <= 0xdfff);
}

ਅੱਖਰ
ਮੰਜ਼ਿਲ_ਹੇਠਲਾ(ਅੱਖਰ c);

ਪੂਰਨ
istrcmp(ਅਚਲ ਅੱਖਰ *a, ਅਚਲ ਅੱਖਰ *b);

ਪੂਰਨ
istrncmp(ਅਚਲ ਅੱਖਰ *a, ਅਚਲ ਅੱਖਰ *b, ਮਾਪ_t n);

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
streq(ਅਚਲ ਅੱਖਰ *s1, ਅਚਲ ਅੱਖਰ *s2)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(s1 && s2);
    ਵਾਪਸ strcmp(s1, s2) == 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
streq_null(ਅਚਲ ਅੱਖਰ *s1, ਅਚਲ ਅੱਖਰ *s2)
{
    ਜੇ (s1 == ਨਿਰਾ || s2 == ਨਿਰਾ)
        ਵਾਪਸ s1 == s2;
    ਵਾਪਸ streq(s1, s2);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
streq_not_null(ਅਚਲ ਅੱਖਰ *s1, ਅਚਲ ਅੱਖਰ *s2)
{
    ਜੇ (!s1 || !s2)
        ਵਾਪਸ false;
    ਵਾਪਸ streq(s1, s2);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
istreq(ਅਚਲ ਅੱਖਰ *s1, ਅਚਲ ਅੱਖਰ *s2)
{
    ਵਾਪਸ istrcmp(s1, s2) == 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
istrneq(ਅਚਲ ਅੱਖਰ *s1, ਅਚਲ ਅੱਖਰ *s2, ਮਾਪ_t len)
{
    ਵਾਪਸ istrncmp(s1, s2, len) == 0;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ istreq_ਅਗੇਤਰ(s1, s2) istrneq(s1, s2, ਆਕਾਰ(s1) - 1)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅੱਖਰ *
strcpy_ਸੁਰੱਖਿਅਤ(ਅੱਖਰ *dest, ਮਾਪ_t ਮਾਪ, ਅਚਲ ਅੱਖਰ *src) {
    ਜੇ (!dest || !ਮਾਪ || !src)
        ਵਾਪਸ ਨਿਰਾ;
    ਅਚਲ ਅੱਖਰ * ਅਚਲ ਹੱਦਬੰਦੀ = dest + ਮਾਪ - 1;
    ਜਦੋਂ (dest < ਹੱਦਬੰਦੀ && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    ਵਾਪਸ *src ? ਨਿਰਾ : dest;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅੱਖਰ *
strdup_ਸੁਰੱਖਿਅਤ(ਅਚਲ ਅੱਖਰ *s)
{
    ਵਾਪਸ s ? strdup(s) : ਨਿਰਾ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਮਾਪ_t
strlen_ਸੁਰੱਖਿਅਤ(ਅਚਲ ਅੱਖਰ *s)
{
    ਵਾਪਸ s ? ਲੜੀ_ਲੰਬਾਈ(s) : 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
isempty(ਅਚਲ ਅੱਖਰ *s)
{
    ਵਾਪਸ s == ਨਿਰਾ || s[0] == '\0';
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ ਅੱਖਰ *
strnull(ਅਚਲ ਅੱਖਰ *s)
{
    ਵਾਪਸ s ? s : "(null)";
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ ਅੱਖਰ *
strempty(ਅਚਲ ਅੱਖਰ *s)
{
    ਵਾਪਸ s ? s : "";
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ *
memdup(ਅਚਲ ਖਾਲੀ *mem, ਮਾਪ_t nmemb, ਮਾਪ_t ਮਾਪ)
{
    ਖਾਲੀ *p = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(nmemb, ਮਾਪ);
    ਜੇ (p)
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(p, mem, nmemb * ਮਾਪ);
    ਵਾਪਸ p;
}

#ਜੇ !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
ਸਥਿਰ ਸਤਰਅੰਦਰ ਅੱਖਰ *
strndup(ਅਚਲ ਅੱਖਰ *s, ਮਾਪ_t n)
{
    ਮਾਪ_t slen = ਲੜੀ_ਲੰਬਾਈ(s);
    ਮਾਪ_t len = MIN(slen, n);
    ਅੱਖਰ *p = ਸਮ੍ਰਿਤੀ_ਦਿਓ(len + 1);
    ਜੇ (!p)
        ਵਾਪਸ ਨਿਰਾ;
    ਸਮ੍ਰਿਤੀ_ਨਕਲ(p, s, len);
    p[len] = '\0';
    ਵਾਪਸ p;
}
#ਜੇ_ਅੰਤ

/* ctype.h is locale-dependent and has other oddities. */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool
is_ascii(ਅੱਖਰ ch)
{
    ਵਾਪਸ (ch & ~0x7f) == 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
is_space(ਅੱਖਰ ch)
{
    ਵਾਪਸ ch == ' ' || (ch >= '\t' && ch <= '\r');
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
is_alpha(ਅੱਖਰ ch)
{
    ਵਾਪਸ (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
is_digit(ਅੱਖਰ ch)
{
    ਵਾਪਸ ch >= '0' && ch <= '9';
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
is_alnum(ਅੱਖਰ ch)
{
    ਵਾਪਸ is_alpha(ch) || is_digit(ch);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
is_xdigit(ਅੱਖਰ ch)
{
    ਵਾਪਸ
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
is_ਗ੍ਰਾਫ(ਅੱਖਰ ch)
{
    /* See table in ascii(7). */
    ਵਾਪਸ ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
msb_pos(uint32_t ਨਕਾਬ)
{
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pos = 0;
    ਜਦੋਂ (ਨਕਾਬ) {
        pos++;
        ਨਕਾਬ >>= 1u;
    }
    ਵਾਪਸ pos;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
one_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ(uint32_t x)
{
    ਵਾਪਸ x && (x & (x - 1)) == 0;
}

bool
map_file(FILE *file, ਅੱਖਰ **string_ਬਾਹਰਲਾ, ਮਾਪ_t *ਮਾਪ_ਬਾਹਰਲਾ);

ਖਾਲੀ
unmap_file(ਅੱਖਰ *string, ਮਾਪ_t ਮਾਪ);

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
check_eaccess(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ ਬਹੁਲਕ)
{
#ਜੇ defined(HAVE_EACCESS)
    ਜੇ (eaccess(ਰਸਤਾ, ਬਹੁਲਕ) != 0)
        ਵਾਪਸ false;
#ਜਾਂਜੇ defined(HAVE_EUIDACCESS)
    ਜੇ (euidaccess(ਰਸਤਾ, ਬਹੁਲਕ) != 0)
        ਵਾਪਸ false;
#ਜੇ_ਅੰਤ

    ਵਾਪਸ true;
}

FILE*
open_file(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ);

#ਜੇ defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#ਜਾਂਜੇ defined(HAVE___SECURE_GETENV)
# define secure_getenv __secure_getenv
#ਨਹੀਂਤਾਂ
# define secure_getenv getenv
#ਜੇ_ਅੰਤ

#ਜੇ defined(HAVE___BUILTIN_EXPECT)
# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#ਨਹੀਂਤਾਂ
# define likely(x)   (x)
# define unlikely(x) (x)
#ਜੇ_ਅੰਤ

/* Compiler Attributes */

/* Private functions only exposed in tests. */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_ਨਿੱਜੀ_APIS
# ਜੇ defined(__GNUC__) && !defined(__CYGWIN__)
#  define XKB_EXPORT_ਨਿੱਜੀ __attribute__((visibility("default")))
# elif defined(_WIN32)
#  define XKB_EXPORT_ਨਿੱਜੀ __declspec(dllexport)
# ਨਹੀਂਤਾਂ
#  define XKB_EXPORT_ਨਿੱਜੀ
# endif
#ਨਹੀਂਤਾਂ
# define XKB_EXPORT_ਨਿੱਜੀ
#ਜੇ_ਅੰਤ

#ਜੇ defined(__MINGW32__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__MINGW_PRINTF_FORMAT, x, y)))
#ਜਾਂਜੇ defined(__GNUC__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__printf__, x, y)))
#ਨਹੀਂਤਾਂ
# define ATTR_PRINTF(x,y)
#ਜੇ_ਅੰਤ

#ਜੇ defined(__GNUC__)
# define ATTR_NORETURN __attribute__((__noreturn__))
#ਨਹੀਂਤਾਂ
# define ATTR_NORETURN
#ਜੇ_ਅੰਤ /* GNUC  */

#ਜੇ defined(__GNUC__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ATTR_MALLOC  __attribute__((__malloc__))
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ATTR_MALLOC
#ਜੇ_ਅੰਤ

#ਜੇ defined(__GNUC__)
# define ATTR_NULL_SENTINEL __attribute__((__sentinel__))
#ਨਹੀਂਤਾਂ
# define ATTR_NULL_SENTINEL
#ਜੇ_ਅੰਤ

#ਜੇ defined(__GNUC__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ATTR_PACKED  __attribute__((__packed__))
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ATTR_PACKED
#ਜੇ_ਅੰਤ

#ਜੇ defined(HAVE_ATTR_COUNTED_BY) && HAVE_ATTR_COUNTED_BY
# define ATTR_COUNTED_BY(ਗਿਣਤੀ_2) __attribute__((counted_by(ਗਿਣਤੀ_2)))
#ਨਹੀਂਤਾਂ
# define ATTR_COUNTED_BY(ਗਿਣਤੀ_2)
#ਜੇ_ਅੰਤ

#ਜੇ !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
XKB_EXPORT_ਨਿੱਜੀ ਪੂਰਨ asprintf(ਅੱਖਰ **strp, ਅਚਲ ਅੱਖਰ *fmt, ...) ATTR_PRINTF(2, 3);
# ਜੇ !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_ਨਿੱਜੀ ਪੂਰਨ vasprintf(ਅੱਖਰ **strp, ਅਚਲ ਅੱਖਰ *fmt, va_list ap);
# endif /* !HAVE_VASPRINTF */
#ਜੇ_ਅੰਤ /* !HAVE_ASPRINTF */

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
ATTR_PRINTF(3, 4)
snprintf_ਸੁਰੱਖਿਅਤ(ਅੱਖਰ *buf, ਮਾਪ_t sz, ਅਚਲ ਅੱਖਰ *format, ...)
{
    va_list ap;
    ਪੂਰਨ rc;

    va_ਸ਼ੁਰੂ(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    va_ਸਿਰਾ(ap);

    ਵਾਪਸ rc >= 0 && (ਮਾਪ_t)rc < sz;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅੱਖਰ *
ATTR_PRINTF(1, 0)
vasprintf_ਸੁਰੱਖਿਅਤ(ਅਚਲ ਅੱਖਰ *fmt, va_list ਦਲੀਲਾਂ)
{
    ਅੱਖਰ *str;
    ਪੂਰਨ len;

    len = vasprintf(&str, fmt, ਦਲੀਲਾਂ);

    ਜੇ (len == -1)
        ਵਾਪਸ ਨਿਰਾ;

    ਵਾਪਸ str;
}

/**
 * A version of asprintf that returns the allocated string or NULL on error.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਅੱਖਰ *
ATTR_PRINTF(1, 2)
asprintf_ਸੁਰੱਖਿਅਤ(ਅਚਲ ਅੱਖਰ *fmt, ...)
{
    va_list ਦਲੀਲਾਂ;
    ਅੱਖਰ *str;

    va_ਸ਼ੁਰੂ(ਦਲੀਲਾਂ, fmt);
    str = vasprintf_ਸੁਰੱਖਿਅਤ(fmt, ਦਲੀਲਾਂ);
    va_ਸਿਰਾ(ਦਲੀਲਾਂ);

    ਵਾਪਸ str;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NDEBUG
    #ਜੇ defined(__GNUC__) || defined(__clang__)
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PANIC_UNREACHABLE() __builtin_unreachable()
    #ਜਾਂਜੇ defined(_MSC_VER)
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PANIC_UNREACHABLE() __assume(0)
    #ਨਹੀਂਤਾਂ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PANIC_UNREACHABLE() abort()
    #ਜੇ_ਅੰਤ
#ਨਹੀਂਤਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PANIC_UNREACHABLE() \
        ਕਰੋ { \
            fprintf(stderr, "Critical Error: Reached unreachable line in %s at %d\n", __FILE__, __LINE__); \
            abort(); \
        } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ

#ਜੇ defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#ਜਾਂਜੇ defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#ਜਾਂਜੇ defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#ਨਹੀਂਤਾਂ
#  define NOINLINE
#ਜੇ_ਅੰਤ
