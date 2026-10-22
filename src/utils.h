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
#जर HAVE_UNISTD_H
# include <unistd.h>
#अन्यथा
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
प्रकारव्याख्या SSIZE_T ssize_t;
#endif
#endif

स्थिर ओळीत bool
is_aligned(स्थिरांक शून्य *restrict pointer, माप_प्रकार byte_मोजणी)
{
    परत (uintptr_t) pointer % byte_मोजणी == 0;
}


#define ARRAY_SIZE(arr) ((आकार(arr) / आकार(*(arr))))

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* Round up @a so it's divisible by @b. */
#define ROUNDUP(a, b) (((a) + (b) - 1) / (b) * (b))

#define STRINGIFY(x) #x
#define STRINGIFY2(x) STRINGIFY(x)
#define CONCAT(x,y) x ## y
#define CONCAT2(x,y) CONCAT(x,y)

/* Check if a Unicode code point is valid in a string literal */
स्थिर ओळीत bool
is_valid_char(uint32_t cp)
{
    /* Currently we only check for NULL character, but this could be extended
     * in the future to further ASCII control characters. */
    परत cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
स्थिर ओळीत bool
is_surrogate(uint32_t cp)
{
    परत (cp >= 0xd800 && cp <= 0xdfff);
}

अक्षर
to_lower(अक्षर c);

पूर्णांक
istrcmp(स्थिरांक अक्षर *a, स्थिरांक अक्षर *b);

पूर्णांक
istrncmp(स्थिरांक अक्षर *a, स्थिरांक अक्षर *b, माप_प्रकार n);

स्थिर ओळीत bool
streq(स्थिरांक अक्षर *s1, स्थिरांक अक्षर *s2)
{
    प्रतिज्ञा(s1 && s2);
    परत म_भेद(s1, s2) == 0;
}

स्थिर ओळीत bool
streq_null(स्थिरांक अक्षर *s1, स्थिरांक अक्षर *s2)
{
    जर (s1 == रिक्त || s2 == रिक्त)
        परत s1 == s2;
    परत streq(s1, s2);
}

स्थिर ओळीत bool
streq_not_null(स्थिरांक अक्षर *s1, स्थिरांक अक्षर *s2)
{
    जर (!s1 || !s2)
        परत false;
    परत streq(s1, s2);
}

स्थिर ओळीत bool
istreq(स्थिरांक अक्षर *s1, स्थिरांक अक्षर *s2)
{
    परत istrcmp(s1, s2) == 0;
}

स्थिर ओळीत bool
istrneq(स्थिरांक अक्षर *s1, स्थिरांक अक्षर *s2, माप_प्रकार len)
{
    परत istrncmp(s1, s2, len) == 0;
}

#define istreq_prefix(s1, s2) istrneq(s1, s2, आकार(s1) - 1)

स्थिर ओळीत अक्षर *
strcpy_safe(अक्षर *dest, माप_प्रकार size, स्थिरांक अक्षर *src) {
    जर (!dest || !size || !src)
        परत रिक्त;
    स्थिरांक अक्षर * स्थिरांक limit = dest + size - 1;
    पर्यंत (dest < limit && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    परत *src ? रिक्त : dest;
}

स्थिर ओळीत अक्षर *
strdup_safe(स्थिरांक अक्षर *s)
{
    परत s ? strdup(s) : रिक्त;
}

स्थिर ओळीत माप_प्रकार
strlen_safe(स्थिरांक अक्षर *s)
{
    परत s ? म_माप(s) : 0;
}

स्थिर ओळीत bool
isempty(स्थिरांक अक्षर *s)
{
    परत s == रिक्त || s[0] == '\0';
}

स्थिर ओळीत स्थिरांक अक्षर *
strnull(स्थिरांक अक्षर *s)
{
    परत s ? s : "(null)";
}

स्थिर ओळीत स्थिरांक अक्षर *
strempty(स्थिरांक अक्षर *s)
{
    परत s ? s : "";
}

स्थिर ओळीत शून्य *
memdup(स्थिरांक शून्य *mem, माप_प्रकार nmemb, माप_प्रकार size)
{
    शून्य *p = सुस्मृति(nmemb, size);
    जर (p)
        स_नकल(p, mem, nmemb * size);
    परत p;
}

#जर !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
स्थिर ओळीत अक्षर *
strndup(स्थिरांक अक्षर *s, माप_प्रकार n)
{
    माप_प्रकार slen = म_माप(s);
    माप_प्रकार len = MIN(slen, n);
    अक्षर *p = दो_स्मृति(len + 1);
    जर (!p)
        परत रिक्त;
    स_नकल(p, s, len);
    p[len] = '\0';
    परत p;
}
#endif

/* ctype.h is locale-dependent and has other oddities. */
स्थिर ओळीत bool
is_ascii(अक्षर ch)
{
    परत (ch & ~0x7f) == 0;
}

स्थिर ओळीत bool
is_space(अक्षर ch)
{
    परत ch == ' ' || (ch >= '\t' && ch <= '\r');
}

स्थिर ओळीत bool
is_alpha(अक्षर ch)
{
    परत (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

स्थिर ओळीत bool
is_digit(अक्षर ch)
{
    परत ch >= '0' && ch <= '9';
}

स्थिर ओळीत bool
is_alnum(अक्षर ch)
{
    परत is_alpha(ch) || is_digit(ch);
}

स्थिर ओळीत bool
is_xdigit(अक्षर ch)
{
    परत
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

स्थिर ओळीत bool
is_graph(अक्षर ch)
{
    /* See table in ascii(7). */
    परत ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
स्थिर ओळीत अचिन्हित पूर्णांक
msb_pos(uint32_t mask)
{
    अचिन्हित पूर्णांक pos = 0;
    पर्यंत (mask) {
        pos++;
        mask >>= 1u;
    }
    परत pos;
}

स्थिर ओळीत पूर्णांक
one_bit_set(uint32_t x)
{
    परत x && (x & (x - 1)) == 0;
}

bool
map_file(खाता *file, अक्षर **string_out, माप_प्रकार *size_out);

शून्य
unmap_file(अक्षर *string, माप_प्रकार size);

स्थिर ओळीत bool
check_eaccess(स्थिरांक अक्षर *path, पूर्णांक mode)
{
#जर defined(HAVE_EACCESS)
    जर (eaccess(path, mode) != 0)
        परत false;
#किंवाजर defined(HAVE_EUIDACCESS)
    जर (euidaccess(path, mode) != 0)
        परत false;
#endif

    परत true;
}

खाता*
open_file(स्थिरांक अक्षर *path);

#जर defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#किंवाजर defined(HAVE___SECURE_GETENV)
# define secure_getenv __secure_getenv
#अन्यथा
# define secure_getenv दो_पर्या
#endif

#जर defined(HAVE___BUILTIN_EXPECT)
# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#अन्यथा
# define likely(x)   (x)
# define unlikely(x) (x)
#endif

/* Compiler Attributes */

/* Private functions only exposed in tests. */
#ifdef ENABLE_PRIVATE_APIS
# जर defined(__GNUC__) && !defined(__CYGWIN__)
#  define XKB_EXPORT_PRIVATE __attribute__((visibility("default")))
# elif defined(_WIN32)
#  define XKB_EXPORT_PRIVATE __declspec(dllexport)
# अन्यथा
#  define XKB_EXPORT_PRIVATE
# endif
#अन्यथा
# define XKB_EXPORT_PRIVATE
#endif

#जर defined(__MINGW32__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__MINGW_PRINTF_FORMAT, x, y)))
#किंवाजर defined(__GNUC__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__printf__, x, y)))
#अन्यथा
# define ATTR_PRINTF(x,y)
#endif

#जर defined(__GNUC__)
# define ATTR_NORETURN __attribute__((__noreturn__))
#अन्यथा
# define ATTR_NORETURN
#endif /* GNUC  */

#जर defined(__GNUC__)
#define ATTR_MALLOC  __attribute__((__malloc__))
#अन्यथा
#define ATTR_MALLOC
#endif

#जर defined(__GNUC__)
# define ATTR_NULL_SENTINEL __attribute__((__sentinel__))
#अन्यथा
# define ATTR_NULL_SENTINEL
#endif

#जर defined(__GNUC__)
#define ATTR_PACKED  __attribute__((__packed__))
#अन्यथा
#define ATTR_PACKED
#endif

#जर defined(HAVE_ATTR_COUNTED_BY) && HAVE_ATTR_COUNTED_BY
# define ATTR_COUNTED_BY(मोजणी) __attribute__((counted_by(मोजणी)))
#अन्यथा
# define ATTR_COUNTED_BY(मोजणी)
#endif

#जर !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
XKB_EXPORT_PRIVATE पूर्णांक asprintf(अक्षर **strp, स्थिरांक अक्षर *fmt, ...) ATTR_PRINTF(2, 3);
# जर !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_PRIVATE पूर्णांक vasprintf(अक्षर **strp, स्थिरांक अक्षर *fmt, बहु_सूची ap);
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */

स्थिर ओळीत bool
ATTR_PRINTF(3, 4)
snprintf_safe(अक्षर *buf, माप_प्रकार sz, स्थिरांक अक्षर *format, ...)
{
    बहु_सूची ap;
    पूर्णांक rc;

    बहु_शुरू(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    बहु_पूर्ण(ap);

    परत rc >= 0 && (माप_प्रकार)rc < sz;
}

स्थिर ओळीत अक्षर *
ATTR_PRINTF(1, 0)
vasprintf_safe(स्थिरांक अक्षर *fmt, बहु_सूची args)
{
    अक्षर *str;
    पूर्णांक len;

    len = vasprintf(&str, fmt, args);

    जर (len == -1)
        परत रिक्त;

    परत str;
}

/**
 * A version of asprintf that returns the allocated string or NULL on error.
 */
स्थिर ओळीत अक्षर *
ATTR_PRINTF(1, 2)
asprintf_safe(स्थिरांक अक्षर *fmt, ...)
{
    बहु_सूची args;
    अक्षर *str;

    बहु_शुरू(args, fmt);
    str = vasprintf_safe(fmt, args);
    बहु_पूर्ण(args);

    परत str;
}

#ifdef न_संशोधन
    #जर defined(__GNUC__) || defined(__clang__)
        #define PANIC_UNREACHABLE() __builtin_unreachable()
    #किंवाजर defined(_MSC_VER)
        #define PANIC_UNREACHABLE() __assume(0)
    #अन्यथा
        #define PANIC_UNREACHABLE() पात()
    #endif
#अन्यथा
    #define PANIC_UNREACHABLE() \
        करा { \
            ख_लिखो(मानक_त्रुटि, "Critical Error: Reached unreachable line in %s at %d\n", __FILE__, __LINE__); \
            पात(); \
        } पर्यंत (0)
#endif

#जर defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#किंवाजर defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#किंवाजर defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#अन्यथा
#  define NOINLINE
#endif
