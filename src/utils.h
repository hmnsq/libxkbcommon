/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>
#అన్తర్భావ <errno.h>
#అన్తర్భావ <inttypes.h>
#అన్తర్భావ <stdarg.h>
#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
# include <unistd.h>
#లేకపోతే
/* Required on Windows where unistd.h doesn't exist */
# define R_OK    4               /* Test for read permission.  */
# define W_OK    2               /* Test for write permission.  */
# define X_OK    1               /* Test for execute permission.  */
# define F_OK    0               /* Test for existence.  */
#అయితే_ముగింపు

#నిర్వచితమైతే _WIN32
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
#నిర్వచితమైతే _MSC_VER
రకనిర్వచనం SSIZE_T ssize_t;
#అయితే_ముగింపు
#అయితే_ముగింపు

స్థిర అంతఃవరుస bool
is_aligned(స్థిరరాశి శూన్య *పరిమిత సూచిక, కొలత_t byte_లెక్క)
{
    తిరిగి (uintptr_t) సూచిక % byte_లెక్క == 0;
}


#నిర్వచించు ARRAY_కొలత(arr) ((పరిమాణం(arr) / పరిమాణం(*(arr))))

#నిర్వచించు MIN(a, b) ((a) < (b) ? (a) : (b))
#నిర్వచించు MAX(a, b) ((a) > (b) ? (a) : (b))

/* Round up @a so it's divisible by @b. */
#నిర్వచించు ROUNDUP(a, b) (((a) + (b) - 1) / (b) * (b))

#నిర్వచించు STRINGIFY(x) #x
#నిర్వచించు STRINGIFY2(x) STRINGIFY(x)
#నిర్వచించు CONCAT(x,y) x ## y
#నిర్వచించు CONCAT2(x,y) CONCAT(x,y)

/* Check if a Unicode code point is valid in a string literal */
స్థిర అంతఃవరుస bool
is_valid_char(uint32_t cp)
{
    /* Currently we only check for NULL character, but this could be extended
     * in the future to further ASCII control characters. */
    తిరిగి cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
స్థిర అంతఃవరుస bool
is_surrogate(uint32_t cp)
{
    తిరిగి (cp >= 0xd800 && cp <= 0xdfff);
}

అక్షరం
గమ్యం_క్రింది(అక్షరం c);

పూర్ణాంకం
istrcmp(స్థిరరాశి అక్షరం *a, స్థిరరాశి అక్షరం *b);

పూర్ణాంకం
istrncmp(స్థిరరాశి అక్షరం *a, స్థిరరాశి అక్షరం *b, కొలత_t n);

స్థిర అంతఃవరుస bool
streq(స్థిరరాశి అక్షరం *s1, స్థిరరాశి అక్షరం *s2)
{
    ప్రతిజ్ఞా(s1 && s2);
    తిరిగి strcmp(s1, s2) == 0;
}

స్థిర అంతఃవరుస bool
streq_null(స్థిరరాశి అక్షరం *s1, స్థిరరాశి అక్షరం *s2)
{
    ఒకవేళ (s1 == ఖాళీ || s2 == ఖాళీ)
        తిరిగి s1 == s2;
    తిరిగి streq(s1, s2);
}

స్థిర అంతఃవరుస bool
streq_not_null(స్థిరరాశి అక్షరం *s1, స్థిరరాశి అక్షరం *s2)
{
    ఒకవేళ (!s1 || !s2)
        తిరిగి false;
    తిరిగి streq(s1, s2);
}

స్థిర అంతఃవరుస bool
istreq(స్థిరరాశి అక్షరం *s1, స్థిరరాశి అక్షరం *s2)
{
    తిరిగి istrcmp(s1, s2) == 0;
}

స్థిర అంతఃవరుస bool
istrneq(స్థిరరాశి అక్షరం *s1, స్థిరరాశి అక్షరం *s2, కొలత_t len)
{
    తిరిగి istrncmp(s1, s2, len) == 0;
}

#నిర్వచించు istreq_ఉపసర్గ(s1, s2) istrneq(s1, s2, పరిమాణం(s1) - 1)

స్థిర అంతఃవరుస అక్షరం *
strcpy_సురక్షిత(అక్షరం *dest, కొలత_t కొలత, స్థిరరాశి అక్షరం *src) {
    ఒకవేళ (!dest || !కొలత || !src)
        తిరిగి ఖాళీ;
    స్థిరరాశి అక్షరం * స్థిరరాశి హద్దు = dest + కొలత - 1;
    అయితే (dest < హద్దు && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    తిరిగి *src ? ఖాళీ : dest;
}

స్థిర అంతఃవరుస అక్షరం *
strdup_సురక్షిత(స్థిరరాశి అక్షరం *s)
{
    తిరిగి s ? strdup(s) : ఖాళీ;
}

స్థిర అంతఃవరుస కొలత_t
strlen_సురక్షిత(స్థిరరాశి అక్షరం *s)
{
    తిరిగి s ? అక్షరశ్రేణి_పొడవు(s) : 0;
}

స్థిర అంతఃవరుస bool
isempty(స్థిరరాశి అక్షరం *s)
{
    తిరిగి s == ఖాళీ || s[0] == '\0';
}

స్థిర అంతఃవరుస స్థిరరాశి అక్షరం *
strnull(స్థిరరాశి అక్షరం *s)
{
    తిరిగి s ? s : "(null)";
}

స్థిర అంతఃవరుస స్థిరరాశి అక్షరం *
strempty(స్థిరరాశి అక్షరం *s)
{
    తిరిగి s ? s : "";
}

స్థిర అంతఃవరుస శూన్య *
memdup(స్థిరరాశి శూన్య *mem, కొలత_t nmemb, కొలత_t కొలత)
{
    శూన్య *p = సున్నా_జ్ఞాపకం_ఇవ్వు(nmemb, కొలత);
    ఒకవేళ (p)
        జ్ఞాపకం_నకలు(p, mem, nmemb * కొలత);
    తిరిగి p;
}

#ఒకవేళ !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
స్థిర అంతఃవరుస అక్షరం *
strndup(స్థిరరాశి అక్షరం *s, కొలత_t n)
{
    కొలత_t slen = అక్షరశ్రేణి_పొడవు(s);
    కొలత_t len = MIN(slen, n);
    అక్షరం *p = జ్ఞాపకం_ఇవ్వు(len + 1);
    ఒకవేళ (!p)
        తిరిగి ఖాళీ;
    జ్ఞాపకం_నకలు(p, s, len);
    p[len] = '\0';
    తిరిగి p;
}
#అయితే_ముగింపు

/* ctype.h is locale-dependent and has other oddities. */
స్థిర అంతఃవరుస bool
is_ascii(అక్షరం ch)
{
    తిరిగి (ch & ~0x7f) == 0;
}

స్థిర అంతఃవరుస bool
is_space(అక్షరం ch)
{
    తిరిగి ch == ' ' || (ch >= '\t' && ch <= '\r');
}

స్థిర అంతఃవరుస bool
is_alpha(అక్షరం ch)
{
    తిరిగి (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

స్థిర అంతఃవరుస bool
is_digit(అక్షరం ch)
{
    తిరిగి ch >= '0' && ch <= '9';
}

స్థిర అంతఃవరుస bool
is_alnum(అక్షరం ch)
{
    తిరిగి is_alpha(ch) || is_digit(ch);
}

స్థిర అంతఃవరుస bool
is_xdigit(అక్షరం ch)
{
    తిరిగి
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

స్థిర అంతఃవరుస bool
is_లేఖాచిత్రం(అక్షరం ch)
{
    /* See table in ascii(7). */
    తిరిగి ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం
msb_pos(uint32_t ముసుగు)
{
    గుర్తులేని పూర్ణాంకం pos = 0;
    అయితే (ముసుగు) {
        pos++;
        ముసుగు >>= 1u;
    }
    తిరిగి pos;
}

స్థిర అంతఃవరుస పూర్ణాంకం
one_బిట్టు_సమితి(uint32_t x)
{
    తిరిగి x && (x & (x - 1)) == 0;
}

bool
map_file(FILE *file, అక్షరం **string_బహిర్గతం, కొలత_t *కొలత_బహిర్గతం);

శూన్య
unmap_file(అక్షరం *string, కొలత_t కొలత);

స్థిర అంతఃవరుస bool
check_eaccess(స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం బహుళకం)
{
#ఒకవేళ defined(HAVE_EACCESS)
    ఒకవేళ (eaccess(మార్గం, బహుళకం) != 0)
        తిరిగి false;
#లేదావేళ defined(HAVE_EUIDACCESS)
    ఒకవేళ (euidaccess(మార్గం, బహుళకం) != 0)
        తిరిగి false;
#అయితే_ముగింపు

    తిరిగి true;
}

FILE*
open_file(స్థిరరాశి అక్షరం *మార్గం);

#ఒకవేళ defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#లేదావేళ defined(HAVE___SECURE_GETENV)
# define secure_getenv __secure_getenv
#లేకపోతే
# define secure_getenv getenv
#అయితే_ముగింపు

#ఒకవేళ defined(HAVE___BUILTIN_EXPECT)
# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#లేకపోతే
# define likely(x)   (x)
# define unlikely(x) (x)
#అయితే_ముగింపు

/* Compiler Attributes */

/* Private functions only exposed in tests. */
#నిర్వచితమైతే ENABLE_వ్యక్తిగత_APIS
# ఒకవేళ defined(__GNUC__) && !defined(__CYGWIN__)
#  define XKB_EXPORT_వ్యక్తిగత __attribute__((visibility("default")))
# elif defined(_WIN32)
#  define XKB_EXPORT_వ్యక్తిగత __declspec(dllexport)
# లేకపోతే
#  define XKB_EXPORT_వ్యక్తిగత
# endif
#లేకపోతే
# define XKB_EXPORT_వ్యక్తిగత
#అయితే_ముగింపు

#ఒకవేళ defined(__MINGW32__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__MINGW_PRINTF_FORMAT, x, y)))
#లేదావేళ defined(__GNUC__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__printf__, x, y)))
#లేకపోతే
# define ATTR_PRINTF(x,y)
#అయితే_ముగింపు

#ఒకవేళ defined(__GNUC__)
# define ATTR_NORETURN __attribute__((__noreturn__))
#లేకపోతే
# define ATTR_NORETURN
#అయితే_ముగింపు /* GNUC  */

#ఒకవేళ defined(__GNUC__)
#నిర్వచించు ATTR_MALLOC  __attribute__((__malloc__))
#లేకపోతే
#నిర్వచించు ATTR_MALLOC
#అయితే_ముగింపు

#ఒకవేళ defined(__GNUC__)
# define ATTR_NULL_SENTINEL __attribute__((__sentinel__))
#లేకపోతే
# define ATTR_NULL_SENTINEL
#అయితే_ముగింపు

#ఒకవేళ defined(__GNUC__)
#నిర్వచించు ATTR_PACKED  __attribute__((__packed__))
#లేకపోతే
#నిర్వచించు ATTR_PACKED
#అయితే_ముగింపు

#ఒకవేళ defined(HAVE_ATTR_COUNTED_BY) && HAVE_ATTR_COUNTED_BY
# define ATTR_COUNTED_BY(లెక్క) __attribute__((counted_by(లెక్క)))
#లేకపోతే
# define ATTR_COUNTED_BY(లెక్క)
#అయితే_ముగింపు

#ఒకవేళ !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
XKB_EXPORT_వ్యక్తిగత పూర్ణాంకం asprintf(అక్షరం **strp, స్థిరరాశి అక్షరం *fmt, ...) ATTR_PRINTF(2, 3);
# ఒకవేళ !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_వ్యక్తిగత పూర్ణాంకం vasprintf(అక్షరం **strp, స్థిరరాశి అక్షరం *fmt, va_list ap);
# endif /* !HAVE_VASPRINTF */
#అయితే_ముగింపు /* !HAVE_ASPRINTF */

స్థిర అంతఃవరుస bool
ATTR_PRINTF(3, 4)
snprintf_సురక్షిత(అక్షరం *buf, కొలత_t sz, స్థిరరాశి అక్షరం *format, ...)
{
    va_list ap;
    పూర్ణాంకం rc;

    va_ప్రారంభం(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    va_చివర(ap);

    తిరిగి rc >= 0 && (కొలత_t)rc < sz;
}

స్థిర అంతఃవరుస అక్షరం *
ATTR_PRINTF(1, 0)
vasprintf_సురక్షిత(స్థిరరాశి అక్షరం *fmt, va_list వాదనలు)
{
    అక్షరం *str;
    పూర్ణాంకం len;

    len = vasprintf(&str, fmt, వాదనలు);

    ఒకవేళ (len == -1)
        తిరిగి ఖాళీ;

    తిరిగి str;
}

/**
 * A version of asprintf that returns the allocated string or NULL on error.
 */
స్థిర అంతఃవరుస అక్షరం *
ATTR_PRINTF(1, 2)
asprintf_సురక్షిత(స్థిరరాశి అక్షరం *fmt, ...)
{
    va_list వాదనలు;
    అక్షరం *str;

    va_ప్రారంభం(వాదనలు, fmt);
    str = vasprintf_సురక్షిత(fmt, వాదనలు);
    va_చివర(వాదనలు);

    తిరిగి str;
}

#నిర్వచితమైతే NDEBUG
    #ఒకవేళ defined(__GNUC__) || defined(__clang__)
        #నిర్వచించు PANIC_UNREACHABLE() __builtin_unreachable()
    #లేదావేళ defined(_MSC_VER)
        #నిర్వచించు PANIC_UNREACHABLE() __assume(0)
    #లేకపోతే
        #నిర్వచించు PANIC_UNREACHABLE() abort()
    #అయితే_ముగింపు
#లేకపోతే
    #నిర్వచించు PANIC_UNREACHABLE() \
        చేయి { \
            fprintf(stderr, "Critical Error: Reached unreachable line in %s at %d\n", __FILE__, __LINE__); \
            abort(); \
        } అయితే (0)
#అయితే_ముగింపు

#ఒకవేళ defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#లేదావేళ defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#లేదావేళ defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#లేకపోతే
#  define NOINLINE
#అయితే_ముగింపు
