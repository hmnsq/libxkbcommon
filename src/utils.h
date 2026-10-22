/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

#شامل "config.h"

#شامل <assert.h>
#شامل <errno.h>
#شامل <inttypes.h>
#شامل <stdarg.h>
#شامل <stdbool.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
# include <unistd.h>
#ورنہ
/* Required on Windows where unistd.h doesn't exist */
# define R_OK    4               /* Test for read permission.  */
# define W_OK    2               /* Test for write permission.  */
# define X_OK    1               /* Test for execute permission.  */
# define F_OK    0               /* Test for existence.  */
#اگر_ختم

#اگر_تعریف_شدہ _WIN32
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
#اگر_تعریف_شدہ _MSC_VER
قسمتعریف SSIZE_T ssize_t;
#اگر_ختم
#اگر_ختم

ساکن اندرونی bool
is_aligned(ثابت خالی *محدود اشارہ_گر, سائز_t byte_گنتی)
{
    واپس (uintptr_t) اشارہ_گر % byte_گنتی == 0;
}


#تعریف_کرو ARRAY_سائز(arr) ((حجم(arr) / حجم(*(arr))))

#تعریف_کرو MIN(a, b) ((a) < (b) ? (a) : (b))
#تعریف_کرو MAX(a, b) ((a) > (b) ? (a) : (b))

/* Round up @a so it's divisible by @b. */
#تعریف_کرو ROUNDUP(a, b) (((a) + (b) - 1) / (b) * (b))

#تعریف_کرو STRINGIFY(x) #x
#تعریف_کرو STRINGIFY2(x) STRINGIFY(x)
#تعریف_کرو CONCAT(x,y) x ## y
#تعریف_کرو CONCAT2(x,y) CONCAT(x,y)

/* Check if a Unicode code point is valid in a string literal */
ساکن اندرونی bool
is_valid_char(uint32_t cp)
{
    /* Currently we only check for NULL character, but this could be extended
     * in the future to further ASCII control characters. */
    واپس cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
ساکن اندرونی bool
is_surrogate(uint32_t cp)
{
    واپس (cp >= 0xd800 && cp <= 0xdfff);
}

حرف
مقصد_زیریں(حرف c);

صحیح
istrcmp(ثابت حرف *a, ثابت حرف *b);

صحیح
istrncmp(ثابت حرف *a, ثابت حرف *b, سائز_t n);

ساکن اندرونی bool
streq(ثابت حرف *s1, ثابت حرف *s2)
{
    أكد(s1 && s2);
    واپس strcmp(s1, s2) == 0;
}

ساکن اندرونی bool
streq_null(ثابت حرف *s1, ثابت حرف *s2)
{
    اگر (s1 == تہی || s2 == تہی)
        واپس s1 == s2;
    واپس streq(s1, s2);
}

ساکن اندرونی bool
streq_not_null(ثابت حرف *s1, ثابت حرف *s2)
{
    اگر (!s1 || !s2)
        واپس false;
    واپس streq(s1, s2);
}

ساکن اندرونی bool
istreq(ثابت حرف *s1, ثابت حرف *s2)
{
    واپس istrcmp(s1, s2) == 0;
}

ساکن اندرونی bool
istrneq(ثابت حرف *s1, ثابت حرف *s2, سائز_t len)
{
    واپس istrncmp(s1, s2, len) == 0;
}

#تعریف_کرو istreq_سابقہ(s1, s2) istrneq(s1, s2, حجم(s1) - 1)

ساکن اندرونی حرف *
strcpy_محفوظ(حرف *dest, سائز_t سائز, ثابت حرف *src) {
    اگر (!dest || !سائز || !src)
        واپس تہی;
    ثابت حرف * ثابت حد_بندی = dest + سائز - 1;
    جب (dest < حد_بندی && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    واپس *src ? تہی : dest;
}

ساکن اندرونی حرف *
strdup_محفوظ(ثابت حرف *s)
{
    واپس s ? strdup(s) : تہی;
}

ساکن اندرونی سائز_t
strlen_محفوظ(ثابت حرف *s)
{
    واپس s ? لڑی_لمبائی(s) : 0;
}

ساکن اندرونی bool
isempty(ثابت حرف *s)
{
    واپس s == تہی || s[0] == '\0';
}

ساکن اندرونی ثابت حرف *
strnull(ثابت حرف *s)
{
    واپس s ? s : "(null)";
}

ساکن اندرونی ثابت حرف *
strempty(ثابت حرف *s)
{
    واپس s ? s : "";
}

ساکن اندرونی خالی *
memdup(ثابت خالی *mem, سائز_t nmemb, سائز_t سائز)
{
    خالی *p = صفر_حافظہ_دو(nmemb, سائز);
    اگر (p)
        حافظہ_نقل(p, mem, nmemb * سائز);
    واپس p;
}

#اگر !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
ساکن اندرونی حرف *
strndup(ثابت حرف *s, سائز_t n)
{
    سائز_t slen = لڑی_لمبائی(s);
    سائز_t len = MIN(slen, n);
    حرف *p = حافظہ_دو(len + 1);
    اگر (!p)
        واپس تہی;
    حافظہ_نقل(p, s, len);
    p[len] = '\0';
    واپس p;
}
#اگر_ختم

/* ctype.h is locale-dependent and has other oddities. */
ساکن اندرونی bool
is_ascii(حرف ch)
{
    واپس (ch & ~0x7f) == 0;
}

ساکن اندرونی bool
is_space(حرف ch)
{
    واپس ch == ' ' || (ch >= '\t' && ch <= '\r');
}

ساکن اندرونی bool
is_alpha(حرف ch)
{
    واپس (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

ساکن اندرونی bool
is_digit(حرف ch)
{
    واپس ch >= '0' && ch <= '9';
}

ساکن اندرونی bool
is_alnum(حرف ch)
{
    واپس is_alpha(ch) || is_digit(ch);
}

ساکن اندرونی bool
is_xdigit(حرف ch)
{
    واپس
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

ساکن اندرونی bool
is_گراف(حرف ch)
{
    /* See table in ascii(7). */
    واپس ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
ساکن اندرونی غیرعلامتی صحیح
msb_pos(uint32_t نقاب)
{
    غیرعلامتی صحیح pos = 0;
    جب (نقاب) {
        pos++;
        نقاب >>= 1u;
    }
    واپس pos;
}

ساکن اندرونی صحیح
one_ثنائی_ہندسہ_سیٹ(uint32_t x)
{
    واپس x && (x & (x - 1)) == 0;
}

bool
map_file(FILE *file, حرف **string_برآمد, سائز_t *سائز_برآمد);

خالی
unmap_file(حرف *string, سائز_t سائز);

ساکن اندرونی bool
check_eaccess(ثابت حرف *راستہ, صحیح منوال)
{
#اگر defined(HAVE_EACCESS)
    اگر (eaccess(راستہ, منوال) != 0)
        واپس false;
#وگرنہ_اگر defined(HAVE_EUIDACCESS)
    اگر (euidaccess(راستہ, منوال) != 0)
        واپس false;
#اگر_ختم

    واپس true;
}

FILE*
open_file(ثابت حرف *راستہ);

#اگر defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#وگرنہ_اگر defined(HAVE___SECURE_GETENV)
# define secure_getenv __secure_getenv
#ورنہ
# define secure_getenv getenv
#اگر_ختم

#اگر defined(HAVE___BUILTIN_EXPECT)
# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#ورنہ
# define likely(x)   (x)
# define unlikely(x) (x)
#اگر_ختم

/* Compiler Attributes */

/* Private functions only exposed in tests. */
#اگر_تعریف_شدہ ENABLE_نجی_APIS
# اگر defined(__GNUC__) && !defined(__CYGWIN__)
#  define XKB_EXPORT_نجی __attribute__((visibility("default")))
# elif defined(_WIN32)
#  define XKB_EXPORT_نجی __declspec(dllexport)
# ورنہ
#  define XKB_EXPORT_نجی
# endif
#ورنہ
# define XKB_EXPORT_نجی
#اگر_ختم

#اگر defined(__MINGW32__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__MINGW_PRINTF_FORMAT, x, y)))
#وگرنہ_اگر defined(__GNUC__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__printf__, x, y)))
#ورنہ
# define ATTR_PRINTF(x,y)
#اگر_ختم

#اگر defined(__GNUC__)
# define ATTR_NORETURN __attribute__((__noreturn__))
#ورنہ
# define ATTR_NORETURN
#اگر_ختم /* GNUC  */

#اگر defined(__GNUC__)
#تعریف_کرو ATTR_MALLOC  __attribute__((__malloc__))
#ورنہ
#تعریف_کرو ATTR_MALLOC
#اگر_ختم

#اگر defined(__GNUC__)
# define ATTR_NULL_SENTINEL __attribute__((__sentinel__))
#ورنہ
# define ATTR_NULL_SENTINEL
#اگر_ختم

#اگر defined(__GNUC__)
#تعریف_کرو ATTR_PACKED  __attribute__((__packed__))
#ورنہ
#تعریف_کرو ATTR_PACKED
#اگر_ختم

#اگر defined(HAVE_ATTR_COUNTED_BY) && HAVE_ATTR_COUNTED_BY
# define ATTR_COUNTED_BY(گنتی) __attribute__((counted_by(گنتی)))
#ورنہ
# define ATTR_COUNTED_BY(گنتی)
#اگر_ختم

#اگر !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
XKB_EXPORT_نجی صحیح asprintf(حرف **strp, ثابت حرف *fmt, ...) ATTR_PRINTF(2, 3);
# اگر !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_نجی صحیح vasprintf(حرف **strp, ثابت حرف *fmt, va_list ap);
# endif /* !HAVE_VASPRINTF */
#اگر_ختم /* !HAVE_ASPRINTF */

ساکن اندرونی bool
ATTR_PRINTF(3, 4)
snprintf_محفوظ(حرف *buf, سائز_t sz, ثابت حرف *format, ...)
{
    va_list ap;
    صحیح rc;

    va_آغاز(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    va_انتہا(ap);

    واپس rc >= 0 && (سائز_t)rc < sz;
}

ساکن اندرونی حرف *
ATTR_PRINTF(1, 0)
vasprintf_محفوظ(ثابت حرف *fmt, va_list دلائل)
{
    حرف *str;
    صحیح len;

    len = vasprintf(&str, fmt, دلائل);

    اگر (len == -1)
        واپس تہی;

    واپس str;
}

/**
 * A version of asprintf that returns the allocated string or NULL on error.
 */
ساکن اندرونی حرف *
ATTR_PRINTF(1, 2)
asprintf_محفوظ(ثابت حرف *fmt, ...)
{
    va_list دلائل;
    حرف *str;

    va_آغاز(دلائل, fmt);
    str = vasprintf_محفوظ(fmt, دلائل);
    va_انتہا(دلائل);

    واپس str;
}

#اگر_تعریف_شدہ NDEBUG
    #اگر defined(__GNUC__) || defined(__clang__)
        #تعریف_کرو PANIC_UNREACHABLE() __builtin_unreachable()
    #وگرنہ_اگر defined(_MSC_VER)
        #تعریف_کرو PANIC_UNREACHABLE() __assume(0)
    #ورنہ
        #تعریف_کرو PANIC_UNREACHABLE() abort()
    #اگر_ختم
#ورنہ
    #تعریف_کرو PANIC_UNREACHABLE() \
        کرو { \
            fprintf(stderr, "Critical Error: Reached unreachable line in %s at %d\n", __FILE__, __LINE__); \
            abort(); \
        } جب (0)
#اگر_ختم

#اگر defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#وگرنہ_اگر defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#وگرنہ_اگر defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#ورنہ
#  define NOINLINE
#اگر_ختم
