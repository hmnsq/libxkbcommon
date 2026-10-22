/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <errno.h>
#ضمن <inttypes.h>
#ضمن <stdarg.h>
#ضمن <stdbool.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
# include <unistd.h>
#وإلا
/* Required on Windows where unistd.h doesn't exist */
# define R_OK    4               /* Test for read permission.  */
# define W_OK    2               /* Test for write permission.  */
# define X_OK    1               /* Test for execute permission.  */
# define F_OK    0               /* Test for existence.  */
#جے_انت

#جے_تعریف_شدہ _WIN32
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
#جے_تعریف_شدہ _MSC_VER
تعريفنوع SSIZE_T ssize_t;
#جے_انت
#جے_انت

ساكن مضمن bool
is_aligned(ثابت فراغ *محدود اشارہ_گر, ناپ_t byte_گنتی)
{
    أرجع (uintptr_t) اشارہ_گر % byte_گنتی == 0;
}


#تعریف_کرو ARRAY_ناپ(arr) ((حجم(arr) / حجم(*(arr))))

#تعریف_کرو MIN(a, b) ((a) < (b) ? (a) : (b))
#تعریف_کرو MAX(a, b) ((a) > (b) ? (a) : (b))

/* Round up @a so it's divisible by @b. */
#تعریف_کرو ROUNDUP(a, b) (((a) + (b) - 1) / (b) * (b))

#تعریف_کرو STRINGIFY(x) #x
#تعریف_کرو STRINGIFY2(x) STRINGIFY(x)
#تعریف_کرو CONCAT(x,y) x ## y
#تعریف_کرو CONCAT2(x,y) CONCAT(x,y)

/* Check if a Unicode code point is valid in a string literal */
ساكن مضمن bool
is_valid_char(uint32_t cp)
{
    /* Currently we only check for NULL character, but this could be extended
     * in the future to further ASCII control characters. */
    أرجع cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
ساكن مضمن bool
is_surrogate(uint32_t cp)
{
    أرجع (cp >= 0xd800 && cp <= 0xdfff);
}

حرف
منزل_ہیٹھلا(حرف c);

صحيح
istrcmp(ثابت حرف *a, ثابت حرف *b);

صحيح
istrncmp(ثابت حرف *a, ثابت حرف *b, ناپ_t n);

ساكن مضمن bool
streq(ثابت حرف *s1, ثابت حرف *s2)
{
    أكد(s1 && s2);
    أرجع strcmp(s1, s2) == 0;
}

ساكن مضمن bool
streq_null(ثابت حرف *s1, ثابت حرف *s2)
{
    إذا (s1 == عدم || s2 == عدم)
        أرجع s1 == s2;
    أرجع streq(s1, s2);
}

ساكن مضمن bool
streq_not_null(ثابت حرف *s1, ثابت حرف *s2)
{
    إذا (!s1 || !s2)
        أرجع false;
    أرجع streq(s1, s2);
}

ساكن مضمن bool
istreq(ثابت حرف *s1, ثابت حرف *s2)
{
    أرجع istrcmp(s1, s2) == 0;
}

ساكن مضمن bool
istrneq(ثابت حرف *s1, ثابت حرف *s2, ناپ_t len)
{
    أرجع istrncmp(s1, s2, len) == 0;
}

#تعریف_کرو istreq_اگیتر(s1, s2) istrneq(s1, s2, حجم(s1) - 1)

ساكن مضمن حرف *
strcpy_محفوظ(حرف *dest, ناپ_t ناپ, ثابت حرف *src) {
    إذا (!dest || !ناپ || !src)
        أرجع عدم;
    ثابت حرف * ثابت حد_بندی = dest + ناپ - 1;
    طالما (dest < حد_بندی && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    أرجع *src ? عدم : dest;
}

ساكن مضمن حرف *
strdup_محفوظ(ثابت حرف *s)
{
    أرجع s ? strdup(s) : عدم;
}

ساكن مضمن ناپ_t
strlen_محفوظ(ثابت حرف *s)
{
    أرجع s ? لڑی_لمبائی(s) : 0;
}

ساكن مضمن bool
isempty(ثابت حرف *s)
{
    أرجع s == عدم || s[0] == '\0';
}

ساكن مضمن ثابت حرف *
strnull(ثابت حرف *s)
{
    أرجع s ? s : "(null)";
}

ساكن مضمن ثابت حرف *
strempty(ثابت حرف *s)
{
    أرجع s ? s : "";
}

ساكن مضمن فراغ *
memdup(ثابت فراغ *mem, ناپ_t nmemb, ناپ_t ناپ)
{
    فراغ *p = صفر_حافظہ_دیو(nmemb, ناپ);
    إذا (p)
        حافظہ_نقل(p, mem, nmemb * ناپ);
    أرجع p;
}

#إذا !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
ساكن مضمن حرف *
strndup(ثابت حرف *s, ناپ_t n)
{
    ناپ_t slen = لڑی_لمبائی(s);
    ناپ_t len = MIN(slen, n);
    حرف *p = حافظہ_دیو(len + 1);
    إذا (!p)
        أرجع عدم;
    حافظہ_نقل(p, s, len);
    p[len] = '\0';
    أرجع p;
}
#جے_انت

/* ctype.h is locale-dependent and has other oddities. */
ساكن مضمن bool
is_ascii(حرف ch)
{
    أرجع (ch & ~0x7f) == 0;
}

ساكن مضمن bool
is_space(حرف ch)
{
    أرجع ch == ' ' || (ch >= '\t' && ch <= '\r');
}

ساكن مضمن bool
is_alpha(حرف ch)
{
    أرجع (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

ساكن مضمن bool
is_digit(حرف ch)
{
    أرجع ch >= '0' && ch <= '9';
}

ساكن مضمن bool
is_alnum(حرف ch)
{
    أرجع is_alpha(ch) || is_digit(ch);
}

ساكن مضمن bool
is_xdigit(حرف ch)
{
    أرجع
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

ساكن مضمن bool
is_گراف(حرف ch)
{
    /* See table in ascii(7). */
    أرجع ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
ساكن مضمن بنانشانی صحيح
msb_pos(uint32_t نقاب)
{
    بنانشانی صحيح pos = 0;
    طالما (نقاب) {
        pos++;
        نقاب >>= 1u;
    }
    أرجع pos;
}

ساكن مضمن صحيح
one_ثنائی_ہندسہ_سیٹ(uint32_t x)
{
    أرجع x && (x & (x - 1)) == 0;
}

bool
map_file(FILE *file, حرف **string_باہرلا, ناپ_t *ناپ_باہرلا);

فراغ
unmap_file(حرف *string, ناپ_t ناپ);

ساكن مضمن bool
check_eaccess(ثابت حرف *رستہ, صحيح منوال)
{
#إذا defined(HAVE_EACCESS)
    إذا (eaccess(رستہ, منوال) != 0)
        أرجع false;
#وإلاإذا defined(HAVE_EUIDACCESS)
    إذا (euidaccess(رستہ, منوال) != 0)
        أرجع false;
#جے_انت

    أرجع true;
}

FILE*
open_file(ثابت حرف *رستہ);

#إذا defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#وإلاإذا defined(HAVE___SECURE_GETENV)
# define secure_getenv __secure_getenv
#وإلا
# define secure_getenv getenv
#جے_انت

#إذا defined(HAVE___BUILTIN_EXPECT)
# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#وإلا
# define likely(x)   (x)
# define unlikely(x) (x)
#جے_انت

/* Compiler Attributes */

/* Private functions only exposed in tests. */
#جے_تعریف_شدہ ENABLE_نجی_APIS
# إذا defined(__GNUC__) && !defined(__CYGWIN__)
#  define XKB_EXPORT_نجی __attribute__((visibility("default")))
# elif defined(_WIN32)
#  define XKB_EXPORT_نجی __declspec(dllexport)
# وإلا
#  define XKB_EXPORT_نجی
# endif
#وإلا
# define XKB_EXPORT_نجی
#جے_انت

#إذا defined(__MINGW32__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__MINGW_PRINTF_FORMAT, x, y)))
#وإلاإذا defined(__GNUC__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__printf__, x, y)))
#وإلا
# define ATTR_PRINTF(x,y)
#جے_انت

#إذا defined(__GNUC__)
# define ATTR_NORETURN __attribute__((__noreturn__))
#وإلا
# define ATTR_NORETURN
#جے_انت /* GNUC  */

#إذا defined(__GNUC__)
#تعریف_کرو ATTR_MALLOC  __attribute__((__malloc__))
#وإلا
#تعریف_کرو ATTR_MALLOC
#جے_انت

#إذا defined(__GNUC__)
# define ATTR_NULL_SENTINEL __attribute__((__sentinel__))
#وإلا
# define ATTR_NULL_SENTINEL
#جے_انت

#إذا defined(__GNUC__)
#تعریف_کرو ATTR_PACKED  __attribute__((__packed__))
#وإلا
#تعریف_کرو ATTR_PACKED
#جے_انت

#إذا defined(HAVE_ATTR_COUNTED_BY) && HAVE_ATTR_COUNTED_BY
# define ATTR_COUNTED_BY(گنتی) __attribute__((counted_by(گنتی)))
#وإلا
# define ATTR_COUNTED_BY(گنتی)
#جے_انت

#إذا !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
XKB_EXPORT_نجی صحيح asprintf(حرف **strp, ثابت حرف *fmt, ...) ATTR_PRINTF(2, 3);
# إذا !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_نجی صحيح vasprintf(حرف **strp, ثابت حرف *fmt, va_list ap);
# endif /* !HAVE_VASPRINTF */
#جے_انت /* !HAVE_ASPRINTF */

ساكن مضمن bool
ATTR_PRINTF(3, 4)
snprintf_محفوظ(حرف *buf, ناپ_t sz, ثابت حرف *format, ...)
{
    va_list ap;
    صحيح rc;

    va_شروع(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    va_سرا(ap);

    أرجع rc >= 0 && (ناپ_t)rc < sz;
}

ساكن مضمن حرف *
ATTR_PRINTF(1, 0)
vasprintf_محفوظ(ثابت حرف *fmt, va_list دلیلاں)
{
    حرف *str;
    صحيح len;

    len = vasprintf(&str, fmt, دلیلاں);

    إذا (len == -1)
        أرجع عدم;

    أرجع str;
}

/**
 * A version of asprintf that returns the allocated string or NULL on error.
 */
ساكن مضمن حرف *
ATTR_PRINTF(1, 2)
asprintf_محفوظ(ثابت حرف *fmt, ...)
{
    va_list دلیلاں;
    حرف *str;

    va_شروع(دلیلاں, fmt);
    str = vasprintf_محفوظ(fmt, دلیلاں);
    va_سرا(دلیلاں);

    أرجع str;
}

#جے_تعریف_شدہ NDEBUG
    #إذا defined(__GNUC__) || defined(__clang__)
        #تعریف_کرو PANIC_UNREACHABLE() __builtin_unreachable()
    #وإلاإذا defined(_MSC_VER)
        #تعریف_کرو PANIC_UNREACHABLE() __assume(0)
    #وإلا
        #تعریف_کرو PANIC_UNREACHABLE() abort()
    #جے_انت
#وإلا
    #تعریف_کرو PANIC_UNREACHABLE() \
        افعل { \
            fprintf(stderr, "Critical Error: Reached unreachable line in %s at %d\n", __FILE__, __LINE__); \
            abort(); \
        } طالما (0)
#جے_انت

#إذا defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#وإلاإذا defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#وإلاإذا defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#وإلا
#  define NOINLINE
#جے_انت
