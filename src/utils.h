/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <inttypes.h>
#अन्तर्भाव <stdarg.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>
#सचे HAVE_UNISTD_H
# include <unistd.h>
#अञ्ञथा
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
प्रकारनिर्देश SSIZE_T ssize_t;
#endif
#endif

स्थिर अन्तःस्थ bool
is_aligned(ध्रुव शून्य *restrict pointer, size_t byte_गणनम्)
{
    पटिनिवत्त (uintptr_t) pointer % byte_गणनम् == 0;
}


#define ARRAY_SIZE(arr) ((परिमाण(arr) / परिमाण(*(arr))))

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* Round up @a so it's divisible by @b. */
#define ROUNDUP(a, b) (((a) + (b) - 1) / (b) * (b))

#define STRINGIFY(x) #x
#define STRINGIFY2(x) STRINGIFY(x)
#define CONCAT(x,y) x ## y
#define CONCAT2(x,y) CONCAT(x,y)

/* Check if a Unicode code point is valid in a string literal */
स्थिर अन्तःस्थ bool
is_valid_char(uint32_t cp)
{
    /* Currently we only check for NULL character, but this could be extended
     * in the future to further ASCII control characters. */
    पटिनिवत्त cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
स्थिर अन्तःस्थ bool
is_surrogate(uint32_t cp)
{
    पटिनिवत्त (cp >= 0xd800 && cp <= 0xdfff);
}

वर्ण
to_lower(वर्ण c);

पूर्णाङ्क
istrcmp(ध्रुव वर्ण *a, ध्रुव वर्ण *b);

पूर्णाङ्क
istrncmp(ध्रुव वर्ण *a, ध्रुव वर्ण *b, size_t n);

स्थिर अन्तःस्थ bool
streq(ध्रुव वर्ण *s1, ध्रुव वर्ण *s2)
{
    प्रतिज्ञा(s1 && s2);
    पटिनिवत्त strcmp(s1, s2) == 0;
}

स्थिर अन्तःस्थ bool
streq_null(ध्रुव वर्ण *s1, ध्रुव वर्ण *s2)
{
    सचे (s1 == रिक्त || s2 == रिक्त)
        पटिनिवत्त s1 == s2;
    पटिनिवत्त streq(s1, s2);
}

स्थिर अन्तःस्थ bool
streq_not_null(ध्रुव वर्ण *s1, ध्रुव वर्ण *s2)
{
    सचे (!s1 || !s2)
        पटिनिवत्त false;
    पटिनिवत्त streq(s1, s2);
}

स्थिर अन्तःस्थ bool
istreq(ध्रुव वर्ण *s1, ध्रुव वर्ण *s2)
{
    पटिनिवत्त istrcmp(s1, s2) == 0;
}

स्थिर अन्तःस्थ bool
istrneq(ध्रुव वर्ण *s1, ध्रुव वर्ण *s2, size_t len)
{
    पटिनिवत्त istrncmp(s1, s2, len) == 0;
}

#define istreq_prefix(s1, s2) istrneq(s1, s2, परिमाण(s1) - 1)

स्थिर अन्तःस्थ वर्ण *
strcpy_safe(वर्ण *dest, size_t size, ध्रुव वर्ण *src) {
    सचे (!dest || !size || !src)
        पटिनिवत्त रिक्त;
    ध्रुव वर्ण * ध्रुव limit = dest + size - 1;
    याव (dest < limit && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    पटिनिवत्त *src ? रिक्त : dest;
}

स्थिर अन्तःस्थ वर्ण *
strdup_safe(ध्रुव वर्ण *s)
{
    पटिनिवत्त s ? strdup(s) : रिक्त;
}

स्थिर अन्तःस्थ size_t
strlen_safe(ध्रुव वर्ण *s)
{
    पटिनिवत्त s ? strlen(s) : 0;
}

स्थिर अन्तःस्थ bool
isempty(ध्रुव वर्ण *s)
{
    पटिनिवत्त s == रिक्त || s[0] == '\0';
}

स्थिर अन्तःस्थ ध्रुव वर्ण *
strnull(ध्रुव वर्ण *s)
{
    पटिनिवत्त s ? s : "(null)";
}

स्थिर अन्तःस्थ ध्रुव वर्ण *
strempty(ध्रुव वर्ण *s)
{
    पटिनिवत्त s ? s : "";
}

स्थिर अन्तःस्थ शून्य *
memdup(ध्रुव शून्य *mem, size_t nmemb, size_t size)
{
    शून्य *p = calloc(nmemb, size);
    सचे (p)
        memcpy(p, mem, nmemb * size);
    पटिनिवत्त p;
}

#सचे !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
स्थिर अन्तःस्थ वर्ण *
strndup(ध्रुव वर्ण *s, size_t n)
{
    size_t slen = strlen(s);
    size_t len = MIN(slen, n);
    वर्ण *p = malloc(len + 1);
    सचे (!p)
        पटिनिवत्त रिक्त;
    memcpy(p, s, len);
    p[len] = '\0';
    पटिनिवत्त p;
}
#endif

/* ctype.h is locale-dependent and has other oddities. */
स्थिर अन्तःस्थ bool
is_ascii(वर्ण ch)
{
    पटिनिवत्त (ch & ~0x7f) == 0;
}

स्थिर अन्तःस्थ bool
is_space(वर्ण ch)
{
    पटिनिवत्त ch == ' ' || (ch >= '\t' && ch <= '\r');
}

स्थिर अन्तःस्थ bool
is_alpha(वर्ण ch)
{
    पटिनिवत्त (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

स्थिर अन्तःस्थ bool
is_digit(वर्ण ch)
{
    पटिनिवत्त ch >= '0' && ch <= '9';
}

स्थिर अन्तःस्थ bool
is_alnum(वर्ण ch)
{
    पटिनिवत्त is_alpha(ch) || is_digit(ch);
}

स्थिर अन्तःस्थ bool
is_xdigit(वर्ण ch)
{
    पटिनिवत्त
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

स्थिर अन्तःस्थ bool
is_graph(वर्ण ch)
{
    /* See table in ascii(7). */
    पटिनिवत्त ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
स्थिर अन्तःस्थ unsigned पूर्णाङ्क
msb_pos(uint32_t mask)
{
    unsigned पूर्णाङ्क pos = 0;
    याव (mask) {
        pos++;
        mask >>= 1u;
    }
    पटिनिवत्त pos;
}

स्थिर अन्तःस्थ पूर्णाङ्क
one_bit_set(uint32_t x)
{
    पटिनिवत्त x && (x & (x - 1)) == 0;
}

bool
map_file(FILE *file, वर्ण **string_out, size_t *size_out);

शून्य
unmap_file(वर्ण *string, size_t size);

स्थिर अन्तःस्थ bool
check_eaccess(ध्रुव वर्ण *path, पूर्णाङ्क mode)
{
#सचे defined(HAVE_EACCESS)
    सचे (eaccess(path, mode) != 0)
        पटिनिवत्त false;
#नोचेद्यदि defined(HAVE_EUIDACCESS)
    सचे (euidaccess(path, mode) != 0)
        पटिनिवत्त false;
#endif

    पटिनिवत्त true;
}

FILE*
open_file(ध्रुव वर्ण *path);

#सचे defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#नोचेद्यदि defined(HAVE___SECURE_GETENV)
# define secure_getenv __secure_getenv
#अञ्ञथा
# define secure_getenv getenv
#endif

#सचे defined(HAVE___BUILTIN_EXPECT)
# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#अञ्ञथा
# define likely(x)   (x)
# define unlikely(x) (x)
#endif

/* Compiler Attributes */

/* Private functions only exposed in tests. */
#ifdef ENABLE_PRIVATE_APIS
# सचे defined(__GNUC__) && !defined(__CYGWIN__)
#  define XKB_EXPORT_PRIVATE __attribute__((visibility("default")))
# elif defined(_WIN32)
#  define XKB_EXPORT_PRIVATE __declspec(dllexport)
# अञ्ञथा
#  define XKB_EXPORT_PRIVATE
# endif
#अञ्ञथा
# define XKB_EXPORT_PRIVATE
#endif

#सचे defined(__MINGW32__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__MINGW_PRINTF_FORMAT, x, y)))
#नोचेद्यदि defined(__GNUC__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__printf__, x, y)))
#अञ्ञथा
# define ATTR_PRINTF(x,y)
#endif

#सचे defined(__GNUC__)
# define ATTR_NORETURN __attribute__((__noreturn__))
#अञ्ञथा
# define ATTR_NORETURN
#endif /* GNUC  */

#सचे defined(__GNUC__)
#define ATTR_MALLOC  __attribute__((__malloc__))
#अञ्ञथा
#define ATTR_MALLOC
#endif

#सचे defined(__GNUC__)
# define ATTR_NULL_SENTINEL __attribute__((__sentinel__))
#अञ्ञथा
# define ATTR_NULL_SENTINEL
#endif

#सचे defined(__GNUC__)
#define ATTR_PACKED  __attribute__((__packed__))
#अञ्ञथा
#define ATTR_PACKED
#endif

#सचे defined(HAVE_ATTR_COUNTED_BY) && HAVE_ATTR_COUNTED_BY
# define ATTR_COUNTED_BY(गणनम्) __attribute__((counted_by(गणनम्)))
#अञ्ञथा
# define ATTR_COUNTED_BY(गणनम्)
#endif

#सचे !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
XKB_EXPORT_PRIVATE पूर्णाङ्क asprintf(वर्ण **strp, ध्रुव वर्ण *fmt, ...) ATTR_PRINTF(2, 3);
# सचे !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_PRIVATE पूर्णाङ्क vasprintf(वर्ण **strp, ध्रुव वर्ण *fmt, va_list ap);
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */

स्थिर अन्तःस्थ bool
ATTR_PRINTF(3, 4)
snprintf_safe(वर्ण *buf, size_t sz, ध्रुव वर्ण *format, ...)
{
    va_list ap;
    पूर्णाङ्क rc;

    va_start(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    va_end(ap);

    पटिनिवत्त rc >= 0 && (size_t)rc < sz;
}

स्थिर अन्तःस्थ वर्ण *
ATTR_PRINTF(1, 0)
vasprintf_safe(ध्रुव वर्ण *fmt, va_list args)
{
    वर्ण *str;
    पूर्णाङ्क len;

    len = vasprintf(&str, fmt, args);

    सचे (len == -1)
        पटिनिवत्त रिक्त;

    पटिनिवत्त str;
}

/**
 * A version of asprintf that returns the allocated string or NULL on error.
 */
स्थिर अन्तःस्थ वर्ण *
ATTR_PRINTF(1, 2)
asprintf_safe(ध्रुव वर्ण *fmt, ...)
{
    va_list args;
    वर्ण *str;

    va_start(args, fmt);
    str = vasprintf_safe(fmt, args);
    va_end(args);

    पटिनिवत्त str;
}

#ifdef NDEBUG
    #सचे defined(__GNUC__) || defined(__clang__)
        #define PANIC_UNREACHABLE() __builtin_unreachable()
    #नोचेद्यदि defined(_MSC_VER)
        #define PANIC_UNREACHABLE() __assume(0)
    #अञ्ञथा
        #define PANIC_UNREACHABLE() abort()
    #endif
#अञ्ञथा
    #define PANIC_UNREACHABLE() \
        करोहि { \
            fprintf(stderr, "Critical Error: Reached unreachable line in %s at %d\n", __FILE__, __LINE__); \
            abort(); \
        } याव (0)
#endif

#सचे defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#नोचेद्यदि defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#नोचेद्यदि defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#अञ्ञथा
#  define NOINLINE
#endif
