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
#यदि HAVE_UNISTD_H
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
प्रकारनिर्देश SSIZE_T ssize_t;
#endif
#endif

स्थिर अन्तःस्थ bool
is_aligned(अचर शून्य *restrict pointer, माप_प्रकार byte_गन्ती)
{
    फर्काऊ (uintptr_t) pointer % byte_गन्ती == 0;
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
    फर्काऊ cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
स्थिर अन्तःस्थ bool
is_surrogate(uint32_t cp)
{
    फर्काऊ (cp >= 0xd800 && cp <= 0xdfff);
}

वर्ण
to_lower(वर्ण c);

पूर्णाङ्क
istrcmp(अचर वर्ण *a, अचर वर्ण *b);

पूर्णाङ्क
istrncmp(अचर वर्ण *a, अचर वर्ण *b, माप_प्रकार n);

स्थिर अन्तःस्थ bool
streq(अचर वर्ण *s1, अचर वर्ण *s2)
{
    प्रतिज्ञा(s1 && s2);
    फर्काऊ म_भेद(s1, s2) == 0;
}

स्थिर अन्तःस्थ bool
streq_null(अचर वर्ण *s1, अचर वर्ण *s2)
{
    यदि (s1 == रिक्त || s2 == रिक्त)
        फर्काऊ s1 == s2;
    फर्काऊ streq(s1, s2);
}

स्थिर अन्तःस्थ bool
streq_not_null(अचर वर्ण *s1, अचर वर्ण *s2)
{
    यदि (!s1 || !s2)
        फर्काऊ false;
    फर्काऊ streq(s1, s2);
}

स्थिर अन्तःस्थ bool
istreq(अचर वर्ण *s1, अचर वर्ण *s2)
{
    फर्काऊ istrcmp(s1, s2) == 0;
}

स्थिर अन्तःस्थ bool
istrneq(अचर वर्ण *s1, अचर वर्ण *s2, माप_प्रकार len)
{
    फर्काऊ istrncmp(s1, s2, len) == 0;
}

#define istreq_prefix(s1, s2) istrneq(s1, s2, परिमाण(s1) - 1)

स्थिर अन्तःस्थ वर्ण *
strcpy_safe(वर्ण *dest, माप_प्रकार size, अचर वर्ण *src) {
    यदि (!dest || !size || !src)
        फर्काऊ रिक्त;
    अचर वर्ण * अचर limit = dest + size - 1;
    जबसम्म (dest < limit && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    फर्काऊ *src ? रिक्त : dest;
}

स्थिर अन्तःस्थ वर्ण *
strdup_safe(अचर वर्ण *s)
{
    फर्काऊ s ? strdup(s) : रिक्त;
}

स्थिर अन्तःस्थ माप_प्रकार
strlen_safe(अचर वर्ण *s)
{
    फर्काऊ s ? म_माप(s) : 0;
}

स्थिर अन्तःस्थ bool
isempty(अचर वर्ण *s)
{
    फर्काऊ s == रिक्त || s[0] == '\0';
}

स्थिर अन्तःस्थ अचर वर्ण *
strnull(अचर वर्ण *s)
{
    फर्काऊ s ? s : "(null)";
}

स्थिर अन्तःस्थ अचर वर्ण *
strempty(अचर वर्ण *s)
{
    फर्काऊ s ? s : "";
}

स्थिर अन्तःस्थ शून्य *
memdup(अचर शून्य *mem, माप_प्रकार nmemb, माप_प्रकार size)
{
    शून्य *p = सुस्मृति(nmemb, size);
    यदि (p)
        स_नकल(p, mem, nmemb * size);
    फर्काऊ p;
}

#यदि !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
स्थिर अन्तःस्थ वर्ण *
strndup(अचर वर्ण *s, माप_प्रकार n)
{
    माप_प्रकार slen = म_माप(s);
    माप_प्रकार len = MIN(slen, n);
    वर्ण *p = दो_स्मृति(len + 1);
    यदि (!p)
        फर्काऊ रिक्त;
    स_नकल(p, s, len);
    p[len] = '\0';
    फर्काऊ p;
}
#endif

/* ctype.h is locale-dependent and has other oddities. */
स्थिर अन्तःस्थ bool
is_ascii(वर्ण ch)
{
    फर्काऊ (ch & ~0x7f) == 0;
}

स्थिर अन्तःस्थ bool
is_space(वर्ण ch)
{
    फर्काऊ ch == ' ' || (ch >= '\t' && ch <= '\r');
}

स्थिर अन्तःस्थ bool
is_alpha(वर्ण ch)
{
    फर्काऊ (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

स्थिर अन्तःस्थ bool
is_digit(वर्ण ch)
{
    फर्काऊ ch >= '0' && ch <= '9';
}

स्थिर अन्तःस्थ bool
is_alnum(वर्ण ch)
{
    फर्काऊ is_alpha(ch) || is_digit(ch);
}

स्थिर अन्तःस्थ bool
is_xdigit(वर्ण ch)
{
    फर्काऊ
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

स्थिर अन्तःस्थ bool
is_graph(वर्ण ch)
{
    /* See table in ascii(7). */
    फर्काऊ ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
स्थिर अन्तःस्थ अचिन्हित पूर्णाङ्क
msb_pos(uint32_t mask)
{
    अचिन्हित पूर्णाङ्क pos = 0;
    जबसम्म (mask) {
        pos++;
        mask >>= 1u;
    }
    फर्काऊ pos;
}

स्थिर अन्तःस्थ पूर्णाङ्क
one_bit_set(uint32_t x)
{
    फर्काऊ x && (x & (x - 1)) == 0;
}

bool
map_file(खाता *file, वर्ण **string_out, माप_प्रकार *size_out);

शून्य
unmap_file(वर्ण *string, माप_प्रकार size);

स्थिर अन्तःस्थ bool
check_eaccess(अचर वर्ण *path, पूर्णाङ्क mode)
{
#यदि defined(HAVE_EACCESS)
    यदि (eaccess(path, mode) != 0)
        फर्काऊ false;
#अथवायदि defined(HAVE_EUIDACCESS)
    यदि (euidaccess(path, mode) != 0)
        फर्काऊ false;
#endif

    फर्काऊ true;
}

खाता*
open_file(अचर वर्ण *path);

#यदि defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#अथवायदि defined(HAVE___SECURE_GETENV)
# define secure_getenv __secure_getenv
#अन्यथा
# define secure_getenv दो_पर्या
#endif

#यदि defined(HAVE___BUILTIN_EXPECT)
# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#अन्यथा
# define likely(x)   (x)
# define unlikely(x) (x)
#endif

/* Compiler Attributes */

/* Private functions only exposed in tests. */
#ifdef ENABLE_PRIVATE_APIS
# यदि defined(__GNUC__) && !defined(__CYGWIN__)
#  define XKB_EXPORT_PRIVATE __attribute__((visibility("default")))
# elif defined(_WIN32)
#  define XKB_EXPORT_PRIVATE __declspec(dllexport)
# अन्यथा
#  define XKB_EXPORT_PRIVATE
# endif
#अन्यथा
# define XKB_EXPORT_PRIVATE
#endif

#यदि defined(__MINGW32__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__MINGW_PRINTF_FORMAT, x, y)))
#अथवायदि defined(__GNUC__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__printf__, x, y)))
#अन्यथा
# define ATTR_PRINTF(x,y)
#endif

#यदि defined(__GNUC__)
# define ATTR_NORETURN __attribute__((__noreturn__))
#अन्यथा
# define ATTR_NORETURN
#endif /* GNUC  */

#यदि defined(__GNUC__)
#define ATTR_MALLOC  __attribute__((__malloc__))
#अन्यथा
#define ATTR_MALLOC
#endif

#यदि defined(__GNUC__)
# define ATTR_NULL_SENTINEL __attribute__((__sentinel__))
#अन्यथा
# define ATTR_NULL_SENTINEL
#endif

#यदि defined(__GNUC__)
#define ATTR_PACKED  __attribute__((__packed__))
#अन्यथा
#define ATTR_PACKED
#endif

#यदि defined(HAVE_ATTR_COUNTED_BY) && HAVE_ATTR_COUNTED_BY
# define ATTR_COUNTED_BY(गन्ती) __attribute__((counted_by(गन्ती)))
#अन्यथा
# define ATTR_COUNTED_BY(गन्ती)
#endif

#यदि !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
XKB_EXPORT_PRIVATE पूर्णाङ्क asprintf(वर्ण **strp, अचर वर्ण *fmt, ...) ATTR_PRINTF(2, 3);
# यदि !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_PRIVATE पूर्णाङ्क vasprintf(वर्ण **strp, अचर वर्ण *fmt, बहु_सूची ap);
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */

स्थिर अन्तःस्थ bool
ATTR_PRINTF(3, 4)
snprintf_safe(वर्ण *buf, माप_प्रकार sz, अचर वर्ण *format, ...)
{
    बहु_सूची ap;
    पूर्णाङ्क rc;

    बहु_शुरू(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    बहु_पूर्ण(ap);

    फर्काऊ rc >= 0 && (माप_प्रकार)rc < sz;
}

स्थिर अन्तःस्थ वर्ण *
ATTR_PRINTF(1, 0)
vasprintf_safe(अचर वर्ण *fmt, बहु_सूची args)
{
    वर्ण *str;
    पूर्णाङ्क len;

    len = vasprintf(&str, fmt, args);

    यदि (len == -1)
        फर्काऊ रिक्त;

    फर्काऊ str;
}

/**
 * A version of asprintf that returns the allocated string or NULL on error.
 */
स्थिर अन्तःस्थ वर्ण *
ATTR_PRINTF(1, 2)
asprintf_safe(अचर वर्ण *fmt, ...)
{
    बहु_सूची args;
    वर्ण *str;

    बहु_शुरू(args, fmt);
    str = vasprintf_safe(fmt, args);
    बहु_पूर्ण(args);

    फर्काऊ str;
}

#ifdef न_संशोधन
    #यदि defined(__GNUC__) || defined(__clang__)
        #define PANIC_UNREACHABLE() __builtin_unreachable()
    #अथवायदि defined(_MSC_VER)
        #define PANIC_UNREACHABLE() __assume(0)
    #अन्यथा
        #define PANIC_UNREACHABLE() पात()
    #endif
#अन्यथा
    #define PANIC_UNREACHABLE() \
        गर { \
            ख_लिखो(मानक_त्रुटि, "Critical Error: Reached unreachable line in %s at %d\n", __FILE__, __LINE__); \
            पात(); \
        } जबसम्म (0)
#endif

#यदि defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#अथवायदि defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#अथवायदि defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#अन्यथा
#  define NOINLINE
#endif
