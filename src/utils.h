/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <inttypes.h>
#ଅନ୍ତର୍ଭାଵ <stdarg.h>
#ଅନ୍ତର୍ଭାଵ <stdbool.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
# include <unistd.h>
#ଅନ୍ୟଥା
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
ପ୍ରକାରବ୍ୟାଖ୍ୟା SSIZE_T ssize_t;
#endif
#endif

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
is_aligned(ଅଚଳ ଶୂନ୍ୟ *restrict pointer, size_t byte_ଗଣନା)
{
    ଫେରନ୍ତୁ (uintptr_t) pointer % byte_ଗଣନା == 0;
}


#define ARRAY_SIZE(arr) ((ଆକାର(arr) / ଆକାର(*(arr))))

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* Round up @a so it's divisible by @b. */
#define ROUNDUP(a, b) (((a) + (b) - 1) / (b) * (b))

#define STRINGIFY(x) #x
#define STRINGIFY2(x) STRINGIFY(x)
#define CONCAT(x,y) x ## y
#define CONCAT2(x,y) CONCAT(x,y)

/* Check if a Unicode code point is valid in a string literal */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
is_valid_char(uint32_t cp)
{
    /* Currently we only check for NULL character, but this could be extended
     * in the future to further ASCII control characters. */
    ଫେରନ୍ତୁ cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
is_surrogate(uint32_t cp)
{
    ଫେରନ୍ତୁ (cp >= 0xd800 && cp <= 0xdfff);
}

ଅକ୍ଷର
to_lower(ଅକ୍ଷର c);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
istrcmp(ଅଚଳ ଅକ୍ଷର *a, ଅଚଳ ଅକ୍ଷର *b);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
istrncmp(ଅଚଳ ଅକ୍ଷର *a, ଅଚଳ ଅକ୍ଷର *b, size_t n);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
streq(ଅଚଳ ଅକ୍ଷର *s1, ଅଚଳ ଅକ୍ଷର *s2)
{
    ପ୍ରତିଜ୍ଞା(s1 && s2);
    ଫେରନ୍ତୁ strcmp(s1, s2) == 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
streq_null(ଅଚଳ ଅକ୍ଷର *s1, ଅଚଳ ଅକ୍ଷର *s2)
{
    ଯଦି (s1 == ଖାଲି || s2 == ଖାଲି)
        ଫେରନ୍ତୁ s1 == s2;
    ଫେରନ୍ତୁ streq(s1, s2);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
streq_not_null(ଅଚଳ ଅକ୍ଷର *s1, ଅଚଳ ଅକ୍ଷର *s2)
{
    ଯଦି (!s1 || !s2)
        ଫେରନ୍ତୁ false;
    ଫେରନ୍ତୁ streq(s1, s2);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
istreq(ଅଚଳ ଅକ୍ଷର *s1, ଅଚଳ ଅକ୍ଷର *s2)
{
    ଫେରନ୍ତୁ istrcmp(s1, s2) == 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
istrneq(ଅଚଳ ଅକ୍ଷର *s1, ଅଚଳ ଅକ୍ଷର *s2, size_t len)
{
    ଫେରନ୍ତୁ istrncmp(s1, s2, len) == 0;
}

#define istreq_prefix(s1, s2) istrneq(s1, s2, ଆକାର(s1) - 1)

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅକ୍ଷର *
strcpy_safe(ଅକ୍ଷର *dest, size_t size, ଅଚଳ ଅକ୍ଷର *src) {
    ଯଦି (!dest || !size || !src)
        ଫେରନ୍ତୁ ଖାଲି;
    ଅଚଳ ଅକ୍ଷର * ଅଚଳ limit = dest + size - 1;
    ଯେତେବେଳେ (dest < limit && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    ଫେରନ୍ତୁ *src ? ଖାଲି : dest;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅକ୍ଷର *
strdup_safe(ଅଚଳ ଅକ୍ଷର *s)
{
    ଫେରନ୍ତୁ s ? strdup(s) : ଖାଲି;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ size_t
strlen_safe(ଅଚଳ ଅକ୍ଷର *s)
{
    ଫେରନ୍ତୁ s ? strlen(s) : 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
isempty(ଅଚଳ ଅକ୍ଷର *s)
{
    ଫେରନ୍ତୁ s == ଖାଲି || s[0] == '\0';
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ ଅକ୍ଷର *
strnull(ଅଚଳ ଅକ୍ଷର *s)
{
    ଫେରନ୍ତୁ s ? s : "(null)";
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ ଅକ୍ଷର *
strempty(ଅଚଳ ଅକ୍ଷର *s)
{
    ଫେରନ୍ତୁ s ? s : "";
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ *
memdup(ଅଚଳ ଶୂନ୍ୟ *mem, size_t nmemb, size_t size)
{
    ଶୂନ୍ୟ *p = calloc(nmemb, size);
    ଯଦି (p)
        memcpy(p, mem, nmemb * size);
    ଫେରନ୍ତୁ p;
}

#ଯଦି !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅକ୍ଷର *
strndup(ଅଚଳ ଅକ୍ଷର *s, size_t n)
{
    size_t slen = strlen(s);
    size_t len = MIN(slen, n);
    ଅକ୍ଷର *p = malloc(len + 1);
    ଯଦି (!p)
        ଫେରନ୍ତୁ ଖାଲି;
    memcpy(p, s, len);
    p[len] = '\0';
    ଫେରନ୍ତୁ p;
}
#endif

/* ctype.h is locale-dependent and has other oddities. */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
is_ascii(ଅକ୍ଷର ch)
{
    ଫେରନ୍ତୁ (ch & ~0x7f) == 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
is_space(ଅକ୍ଷର ch)
{
    ଫେରନ୍ତୁ ch == ' ' || (ch >= '\t' && ch <= '\r');
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
is_alpha(ଅକ୍ଷର ch)
{
    ଫେରନ୍ତୁ (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
is_digit(ଅକ୍ଷର ch)
{
    ଫେରନ୍ତୁ ch >= '0' && ch <= '9';
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
is_alnum(ଅକ୍ଷର ch)
{
    ଫେରନ୍ତୁ is_alpha(ch) || is_digit(ch);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
is_xdigit(ଅକ୍ଷର ch)
{
    ଫେରନ୍ତୁ
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
is_graph(ଅକ୍ଷର ch)
{
    /* See table in ascii(7). */
    ଫେରନ୍ତୁ ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
msb_pos(uint32_t mask)
{
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos = 0;
    ଯେତେବେଳେ (mask) {
        pos++;
        mask >>= 1u;
    }
    ଫେରନ୍ତୁ pos;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
one_bit_set(uint32_t x)
{
    ଫେରନ୍ତୁ x && (x & (x - 1)) == 0;
}

bool
map_file(FILE *file, ଅକ୍ଷର **string_out, size_t *size_out);

ଶୂନ୍ୟ
unmap_file(ଅକ୍ଷର *string, size_t size);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
check_eaccess(ଅଚଳ ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode)
{
#ଯଦି defined(HAVE_EACCESS)
    ଯଦି (eaccess(path, mode) != 0)
        ଫେରନ୍ତୁ false;
#ନଚେତଯଦି defined(HAVE_EUIDACCESS)
    ଯଦି (euidaccess(path, mode) != 0)
        ଫେରନ୍ତୁ false;
#endif

    ଫେରନ୍ତୁ true;
}

FILE*
open_file(ଅଚଳ ଅକ୍ଷର *path);

#ଯଦି defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#ନଚେତଯଦି defined(HAVE___SECURE_GETENV)
# define secure_getenv __secure_getenv
#ଅନ୍ୟଥା
# define secure_getenv getenv
#endif

#ଯଦି defined(HAVE___BUILTIN_EXPECT)
# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#ଅନ୍ୟଥା
# define likely(x)   (x)
# define unlikely(x) (x)
#endif

/* Compiler Attributes */

/* Private functions only exposed in tests. */
#ifdef ENABLE_PRIVATE_APIS
# ଯଦି defined(__GNUC__) && !defined(__CYGWIN__)
#  define XKB_EXPORT_PRIVATE __attribute__((visibility("default")))
# elif defined(_WIN32)
#  define XKB_EXPORT_PRIVATE __declspec(dllexport)
# ଅନ୍ୟଥା
#  define XKB_EXPORT_PRIVATE
# endif
#ଅନ୍ୟଥା
# define XKB_EXPORT_PRIVATE
#endif

#ଯଦି defined(__MINGW32__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__MINGW_PRINTF_FORMAT, x, y)))
#ନଚେତଯଦି defined(__GNUC__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__printf__, x, y)))
#ଅନ୍ୟଥା
# define ATTR_PRINTF(x,y)
#endif

#ଯଦି defined(__GNUC__)
# define ATTR_NORETURN __attribute__((__noreturn__))
#ଅନ୍ୟଥା
# define ATTR_NORETURN
#endif /* GNUC  */

#ଯଦି defined(__GNUC__)
#define ATTR_MALLOC  __attribute__((__malloc__))
#ଅନ୍ୟଥା
#define ATTR_MALLOC
#endif

#ଯଦି defined(__GNUC__)
# define ATTR_NULL_SENTINEL __attribute__((__sentinel__))
#ଅନ୍ୟଥା
# define ATTR_NULL_SENTINEL
#endif

#ଯଦି defined(__GNUC__)
#define ATTR_PACKED  __attribute__((__packed__))
#ଅନ୍ୟଥା
#define ATTR_PACKED
#endif

#ଯଦି defined(HAVE_ATTR_COUNTED_BY) && HAVE_ATTR_COUNTED_BY
# define ATTR_COUNTED_BY(ଗଣନା_2) __attribute__((counted_by(ଗଣନା_2)))
#ଅନ୍ୟଥା
# define ATTR_COUNTED_BY(ଗଣନା_2)
#endif

#ଯଦି !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
XKB_EXPORT_PRIVATE ପୂର୍ଣ୍ଣସଂଖ୍ୟା asprintf(ଅକ୍ଷର **strp, ଅଚଳ ଅକ୍ଷର *fmt, ...) ATTR_PRINTF(2, 3);
# ଯଦି !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_PRIVATE ପୂର୍ଣ୍ଣସଂଖ୍ୟା vasprintf(ଅକ୍ଷର **strp, ଅଚଳ ଅକ୍ଷର *fmt, va_list ap);
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
ATTR_PRINTF(3, 4)
snprintf_safe(ଅକ୍ଷର *buf, size_t sz, ଅଚଳ ଅକ୍ଷର *format, ...)
{
    va_list ap;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା rc;

    va_start(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    va_end(ap);

    ଫେରନ୍ତୁ rc >= 0 && (size_t)rc < sz;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅକ୍ଷର *
ATTR_PRINTF(1, 0)
vasprintf_safe(ଅଚଳ ଅକ୍ଷର *fmt, va_list args)
{
    ଅକ୍ଷର *str;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;

    len = vasprintf(&str, fmt, args);

    ଯଦି (len == -1)
        ଫେରନ୍ତୁ ଖାଲି;

    ଫେରନ୍ତୁ str;
}

/**
 * A version of asprintf that returns the allocated string or NULL on error.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅକ୍ଷର *
ATTR_PRINTF(1, 2)
asprintf_safe(ଅଚଳ ଅକ୍ଷର *fmt, ...)
{
    va_list args;
    ଅକ୍ଷର *str;

    va_start(args, fmt);
    str = vasprintf_safe(fmt, args);
    va_end(args);

    ଫେରନ୍ତୁ str;
}

#ifdef NDEBUG
    #ଯଦି defined(__GNUC__) || defined(__clang__)
        #define PANIC_UNREACHABLE() __builtin_unreachable()
    #ନଚେତଯଦି defined(_MSC_VER)
        #define PANIC_UNREACHABLE() __assume(0)
    #ଅନ୍ୟଥା
        #define PANIC_UNREACHABLE() abort()
    #endif
#ଅନ୍ୟଥା
    #define PANIC_UNREACHABLE() \
        କର { \
            fprintf(stderr, "Critical Error: Reached unreachable line in %s at %d\n", __FILE__, __LINE__); \
            abort(); \
        } ଯେତେବେଳେ (0)
#endif

#ଯଦି defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#ନଚେତଯଦି defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#ନଚେତଯଦି defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#ଅନ୍ୟଥା
#  define NOINLINE
#endif
