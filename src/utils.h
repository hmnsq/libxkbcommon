/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <inttypes.h>
#अन्तर्भाव <stdarg.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
# include <unistd.h>
#अन्यथा
/* Required on Windows where unistd.h doesn't exist */
# define R_OK    4               /* Test for read permission.  */
# define W_OK    2               /* Test for write permission.  */
# define X_OK    1               /* Test for execute permission.  */
# define F_OK    0               /* Test for existence.  */
#यदि_अन्तः

#यदि_परिभाषितम् _WIN32
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
#यदि_परिभाषितम् _MSC_VER
प्रकारनिर्देश SSIZE_T ssize_t;
#यदि_अन्तः
#यदि_अन्तः

स्थिर अन्तःस्थ bool
is_aligned(ध्रुव शून्य *सीमित pointer, आकार_t byte_गणनम्)
{
    प्रत्यागम (uintptr_t) pointer % byte_गणनम् == 0;
}


#परिभाषय ARRAY_आकार(arr) ((परिमाण(arr) / परिमाण(*(arr))))

#परिभाषय MIN(a, b) ((a) < (b) ? (a) : (b))
#परिभाषय MAX(a, b) ((a) > (b) ? (a) : (b))

/* Round up @a so it's divisible by @b. */
#परिभाषय ROUNDUP(a, b) (((a) + (b) - 1) / (b) * (b))

#परिभाषय STRINGIFY(x) #x
#परिभाषय STRINGIFY2(x) STRINGIFY(x)
#परिभाषय CONCAT(x,y) x ## y
#परिभाषय CONCAT2(x,y) CONCAT(x,y)

/* Check if a Unicode code point is valid in a string literal */
स्थिर अन्तःस्थ bool
is_valid_char(uint32_t cp)
{
    /* Currently we only check for NULL character, but this could be extended
     * in the future to further ASCII control characters. */
    प्रत्यागम cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
स्थिर अन्तःस्थ bool
is_surrogate(uint32_t cp)
{
    प्रत्यागम (cp >= 0xd800 && cp <= 0xdfff);
}

वर्ण
गन्तव्यम्_अधस्तनम्(वर्ण c);

पूर्णाङ्क
istrcmp(ध्रुव वर्ण *a, ध्रुव वर्ण *b);

पूर्णाङ्क
istrncmp(ध्रुव वर्ण *a, ध्रुव वर्ण *b, आकार_t n);

स्थिर अन्तःस्थ bool
streq(ध्रुव वर्ण *s1, ध्रुव वर्ण *s2)
{
    प्रतिज्ञा(s1 && s2);
    प्रत्यागम वाक्यं_तुलय(s1, s2) == 0;
}

स्थिर अन्तःस्थ bool
streq_null(ध्रुव वर्ण *s1, ध्रुव वर्ण *s2)
{
    यदि (s1 == रिक्त || s2 == रिक्त)
        प्रत्यागम s1 == s2;
    प्रत्यागम streq(s1, s2);
}

स्थिर अन्तःस्थ bool
streq_not_null(ध्रुव वर्ण *s1, ध्रुव वर्ण *s2)
{
    यदि (!s1 || !s2)
        प्रत्यागम false;
    प्रत्यागम streq(s1, s2);
}

स्थिर अन्तःस्थ bool
istreq(ध्रुव वर्ण *s1, ध्रुव वर्ण *s2)
{
    प्रत्यागम istrcmp(s1, s2) == 0;
}

स्थिर अन्तःस्थ bool
istrneq(ध्रुव वर्ण *s1, ध्रुव वर्ण *s2, आकार_t len)
{
    प्रत्यागम istrncmp(s1, s2, len) == 0;
}

#परिभाषय istreq_उपसर्गः(s1, s2) istrneq(s1, s2, परिमाण(s1) - 1)

स्थिर अन्तःस्थ वर्ण *
strcpy_safe(वर्ण *dest, आकार_t आकार, ध्रुव वर्ण *src) {
    यदि (!dest || !आकार || !src)
        प्रत्यागम रिक्त;
    ध्रुव वर्ण * ध्रुव सीमा = dest + आकार - 1;
    यावत् (dest < सीमा && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    प्रत्यागम *src ? रिक्त : dest;
}

स्थिर अन्तःस्थ वर्ण *
strdup_safe(ध्रुव वर्ण *s)
{
    प्रत्यागम s ? strdup(s) : रिक्त;
}

स्थिर अन्तःस्थ आकार_t
strlen_safe(ध्रुव वर्ण *s)
{
    प्रत्यागम s ? वाक्यदैर्घ्यम्(s) : 0;
}

स्थिर अन्तःस्थ bool
isempty(ध्रुव वर्ण *s)
{
    प्रत्यागम s == रिक्त || s[0] == '\0';
}

स्थिर अन्तःस्थ ध्रुव वर्ण *
strnull(ध्रुव वर्ण *s)
{
    प्रत्यागम s ? s : "(null)";
}

स्थिर अन्तःस्थ ध्रुव वर्ण *
strempty(ध्रुव वर्ण *s)
{
    प्रत्यागम s ? s : "";
}

स्थिर अन्तःस्थ शून्य *
memdup(ध्रुव शून्य *mem, आकार_t nmemb, आकार_t आकार)
{
    शून्य *p = शून्यस्मृतिं_देहि(nmemb, आकार);
    यदि (p)
        स्मृतिं_प्रतिलिख(p, mem, nmemb * आकार);
    प्रत्यागम p;
}

#यदि !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
स्थिर अन्तःस्थ वर्ण *
strndup(ध्रुव वर्ण *s, आकार_t n)
{
    आकार_t slen = वाक्यदैर्घ्यम्(s);
    आकार_t len = MIN(slen, n);
    वर्ण *p = स्मृतिं_देहि(len + 1);
    यदि (!p)
        प्रत्यागम रिक्त;
    स्मृतिं_प्रतिलिख(p, s, len);
    p[len] = '\0';
    प्रत्यागम p;
}
#यदि_अन्तः

/* ctype.h is locale-dependent and has other oddities. */
स्थिर अन्तःस्थ bool
is_ascii(वर्ण ch)
{
    प्रत्यागम (ch & ~0x7f) == 0;
}

स्थिर अन्तःस्थ bool
is_space(वर्ण ch)
{
    प्रत्यागम ch == ' ' || (ch >= '\t' && ch <= '\r');
}

स्थिर अन्तःस्थ bool
is_alpha(वर्ण ch)
{
    प्रत्यागम (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

स्थिर अन्तःस्थ bool
is_digit(वर्ण ch)
{
    प्रत्यागम ch >= '0' && ch <= '9';
}

स्थिर अन्तःस्थ bool
is_alnum(वर्ण ch)
{
    प्रत्यागम is_alpha(ch) || is_digit(ch);
}

स्थिर अन्तःस्थ bool
is_xdigit(वर्ण ch)
{
    प्रत्यागम
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

स्थिर अन्तःस्थ bool
is_graph(वर्ण ch)
{
    /* See table in ascii(7). */
    प्रत्यागम ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
स्थिर अन्तःस्थ अचिह्नित पूर्णाङ्क
msb_pos(uint32_t आवरणम्)
{
    अचिह्नित पूर्णाङ्क pos = 0;
    यावत् (आवरणम्) {
        pos++;
        आवरणम् >>= 1u;
    }
    प्रत्यागम pos;
}

स्थिर अन्तःस्थ पूर्णाङ्क
one_bit_समुच्चयः(uint32_t x)
{
    प्रत्यागम x && (x & (x - 1)) == 0;
}

bool
map_file(FILE *file, वर्ण **string_बहिर्गमनम्, आकार_t *आकार_बहिर्गमनम्);

शून्य
unmap_file(वर्ण *string, आकार_t आकार);

स्थिर अन्तःस्थ bool
check_eaccess(ध्रुव वर्ण *मार्गः, पूर्णाङ्क बहुलकम्)
{
#यदि defined(HAVE_EACCESS)
    यदि (eaccess(मार्गः, बहुलकम्) != 0)
        प्रत्यागम false;
#नोचेद्यदि defined(HAVE_EUIDACCESS)
    यदि (euidaccess(मार्गः, बहुलकम्) != 0)
        प्रत्यागम false;
#यदि_अन्तः

    प्रत्यागम true;
}

FILE*
open_file(ध्रुव वर्ण *मार्गः);

#यदि defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#नोचेद्यदि defined(HAVE___SECURE_GETENV)
# define secure_getenv __secure_getenv
#अन्यथा
# define secure_getenv getenv
#यदि_अन्तः

#यदि defined(HAVE___BUILTIN_EXPECT)
# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#अन्यथा
# define likely(x)   (x)
# define unlikely(x) (x)
#यदि_अन्तः

/* Compiler Attributes */

/* Private functions only exposed in tests. */
#यदि_परिभाषितम् ENABLE_वैयक्तिकम्_APIS
# यदि defined(__GNUC__) && !defined(__CYGWIN__)
#  define XKB_EXPORT_वैयक्तिकम् __attribute__((visibility("default")))
# elif defined(_WIN32)
#  define XKB_EXPORT_वैयक्तिकम् __declspec(dllexport)
# अन्यथा
#  define XKB_EXPORT_वैयक्तिकम्
# endif
#अन्यथा
# define XKB_EXPORT_वैयक्तिकम्
#यदि_अन्तः

#यदि defined(__MINGW32__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__MINGW_PRINTF_FORMAT, x, y)))
#नोचेद्यदि defined(__GNUC__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__printf__, x, y)))
#अन्यथा
# define ATTR_PRINTF(x,y)
#यदि_अन्तः

#यदि defined(__GNUC__)
# define ATTR_NORETURN __attribute__((__noreturn__))
#अन्यथा
# define ATTR_NORETURN
#यदि_अन्तः /* GNUC  */

#यदि defined(__GNUC__)
#परिभाषय ATTR_MALLOC  __attribute__((__malloc__))
#अन्यथा
#परिभाषय ATTR_MALLOC
#यदि_अन्तः

#यदि defined(__GNUC__)
# define ATTR_NULL_SENTINEL __attribute__((__sentinel__))
#अन्यथा
# define ATTR_NULL_SENTINEL
#यदि_अन्तः

#यदि defined(__GNUC__)
#परिभाषय ATTR_PACKED  __attribute__((__packed__))
#अन्यथा
#परिभाषय ATTR_PACKED
#यदि_अन्तः

#यदि defined(HAVE_ATTR_COUNTED_BY) && HAVE_ATTR_COUNTED_BY
# define ATTR_COUNTED_BY(गणनम्) __attribute__((counted_by(गणनम्)))
#अन्यथा
# define ATTR_COUNTED_BY(गणनम्)
#यदि_अन्तः

#यदि !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
XKB_EXPORT_वैयक्तिकम् पूर्णाङ्क asprintf(वर्ण **strp, ध्रुव वर्ण *fmt, ...) ATTR_PRINTF(2, 3);
# यदि !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_वैयक्तिकम् पूर्णाङ्क vasprintf(वर्ण **strp, ध्रुव वर्ण *fmt, va_list ap);
# endif /* !HAVE_VASPRINTF */
#यदि_अन्तः /* !HAVE_ASPRINTF */

स्थिर अन्तःस्थ bool
ATTR_PRINTF(3, 4)
snprintf_safe(वर्ण *buf, आकार_t sz, ध्रुव वर्ण *format, ...)
{
    va_list ap;
    पूर्णाङ्क rc;

    va_start(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    va_अन्तः(ap);

    प्रत्यागम rc >= 0 && (आकार_t)rc < sz;
}

स्थिर अन्तःस्थ वर्ण *
ATTR_PRINTF(1, 0)
vasprintf_safe(ध्रुव वर्ण *fmt, va_list तर्काः)
{
    वर्ण *str;
    पूर्णाङ्क len;

    len = vasprintf(&str, fmt, तर्काः);

    यदि (len == -1)
        प्रत्यागम रिक्त;

    प्रत्यागम str;
}

/**
 * A version of asprintf that returns the allocated string or NULL on error.
 */
स्थिर अन्तःस्थ वर्ण *
ATTR_PRINTF(1, 2)
asprintf_safe(ध्रुव वर्ण *fmt, ...)
{
    va_list तर्काः;
    वर्ण *str;

    va_start(तर्काः, fmt);
    str = vasprintf_safe(fmt, तर्काः);
    va_अन्तः(तर्काः);

    प्रत्यागम str;
}

#यदि_परिभाषितम् NDEBUG
    #यदि defined(__GNUC__) || defined(__clang__)
        #परिभाषय PANIC_UNREACHABLE() __builtin_unreachable()
    #नोचेद्यदि defined(_MSC_VER)
        #परिभाषय PANIC_UNREACHABLE() __assume(0)
    #अन्यथा
        #परिभाषय PANIC_UNREACHABLE() abort()
    #यदि_अन्तः
#अन्यथा
    #परिभाषय PANIC_UNREACHABLE() \
        कुरु { \
            fprintf(stderr, "Critical Error: Reached unreachable line in %s at %d\n", __FILE__, __LINE__); \
            abort(); \
        } यावत् (0)
#यदि_अन्तः

#यदि defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#नोचेद्यदि defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#नोचेद्यदि defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#अन्यथा
#  define NOINLINE
#यदि_अन्तः
