/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#आशय once

#समावेश "config.h"

#समावेश <assert.h>
#समावेश <errno.h>
#समावेश <inttypes.h>
#समावेश <stdarg.h>
#समावेश <stdbool.h>
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
# include <unistd.h>
#अन्यथा
/* Required on Windows where unistd.h doesn't exist */
# define R_OK    4               /* Test for read permission.  */
# define W_OK    2               /* Test for write permission.  */
# define X_OK    1               /* Test for execute permission.  */
# define F_OK    0               /* Test for existence.  */
#पूर्ण_अगर

#अगर_घोषित _WIN32
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
#अगर_घोषित _MSC_VER
प्रकार SSIZE_T ssize_t;
#पूर्ण_अगर
#पूर्ण_अगर

अटल अंतरभूत bool
is_aligned(स्थिर व्योम *restrict pointer, माप_प्रकार byte_गिनती)
{
    वापस (uintptr_t) pointer % byte_गिनती == 0;
}


#घोषणा ARRAY_आकार(arr) ((माप(arr) / माप(*(arr))))

#घोषणा MIN(a, b) ((a) < (b) ? (a) : (b))
#घोषणा MAX(a, b) ((a) > (b) ? (a) : (b))

/* Round up @a so it's divisible by @b. */
#घोषणा ROUNDUP(a, b) (((a) + (b) - 1) / (b) * (b))

#घोषणा STRINGIFY(x) #x
#घोषणा STRINGIFY2(x) STRINGIFY(x)
#घोषणा CONCAT(x,y) x ## y
#घोषणा CONCAT2(x,y) CONCAT(x,y)

/* Check if a Unicode code point is valid in a string literal */
अटल अंतरभूत bool
is_valid_char(uint32_t cp)
{
    /* Currently we only check for NULL character, but this could be extended
     * in the future to further ASCII control characters. */
    वापस cp != 0;
}

/* Check if a Unicode code point is a surrogate.
 * Those code points are used only in UTF-16 encodings. */
अटल अंतरभूत bool
is_surrogate(uint32_t cp)
{
    वापस (cp >= 0xd800 && cp <= 0xdfff);
}

अक्षर
to_lower(अक्षर c);

पूर्णांक
istrcmp(स्थिर अक्षर *a, स्थिर अक्षर *b);

पूर्णांक
istrncmp(स्थिर अक्षर *a, स्थिर अक्षर *b, माप_प्रकार n);

अटल अंतरभूत bool
streq(स्थिर अक्षर *s1, स्थिर अक्षर *s2)
{
    निश्चित(s1 && s2);
    वापस म_भेद(s1, s2) == 0;
}

अटल अंतरभूत bool
streq_null(स्थिर अक्षर *s1, स्थिर अक्षर *s2)
{
    अगर (s1 == शून्य || s2 == शून्य)
        वापस s1 == s2;
    वापस streq(s1, s2);
}

अटल अंतरभूत bool
streq_not_null(स्थिर अक्षर *s1, स्थिर अक्षर *s2)
{
    अगर (!s1 || !s2)
        वापस false;
    वापस streq(s1, s2);
}

अटल अंतरभूत bool
istreq(स्थिर अक्षर *s1, स्थिर अक्षर *s2)
{
    वापस istrcmp(s1, s2) == 0;
}

अटल अंतरभूत bool
istrneq(स्थिर अक्षर *s1, स्थिर अक्षर *s2, माप_प्रकार len)
{
    वापस istrncmp(s1, s2, len) == 0;
}

#घोषणा istreq_prefix(s1, s2) istrneq(s1, s2, माप(s1) - 1)

अटल अंतरभूत अक्षर *
strcpy_safe(अक्षर *dest, माप_प्रकार आकार, स्थिर अक्षर *src) {
    अगर (!dest || !आकार || !src)
        वापस शून्य;
    स्थिर अक्षर * स्थिर limit = dest + आकार - 1;
    जबतक (dest < limit && *src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    वापस *src ? शून्य : dest;
}

अटल अंतरभूत अक्षर *
strdup_safe(स्थिर अक्षर *s)
{
    वापस s ? strdup(s) : शून्य;
}

अटल अंतरभूत माप_प्रकार
strlen_safe(स्थिर अक्षर *s)
{
    वापस s ? म_माप(s) : 0;
}

अटल अंतरभूत bool
isempty(स्थिर अक्षर *s)
{
    वापस s == शून्य || s[0] == '\0';
}

अटल अंतरभूत स्थिर अक्षर *
strnull(स्थिर अक्षर *s)
{
    वापस s ? s : "(null)";
}

अटल अंतरभूत स्थिर अक्षर *
strempty(स्थिर अक्षर *s)
{
    वापस s ? s : "";
}

अटल अंतरभूत व्योम *
memdup(स्थिर व्योम *mem, माप_प्रकार nmemb, माप_प्रकार आकार)
{
    व्योम *p = सुस्मृति(nmemb, आकार);
    अगर (p)
        स_नकल(p, mem, nmemb * आकार);
    वापस p;
}

#अगर !(defined(HAVE_STRNDUP) && HAVE_STRNDUP)
अटल अंतरभूत अक्षर *
strndup(स्थिर अक्षर *s, माप_प्रकार n)
{
    माप_प्रकार slen = म_माप(s);
    माप_प्रकार len = MIN(slen, n);
    अक्षर *p = दो_स्मृति(len + 1);
    अगर (!p)
        वापस शून्य;
    स_नकल(p, s, len);
    p[len] = '\0';
    वापस p;
}
#पूर्ण_अगर

/* ctype.h is locale-dependent and has other oddities. */
अटल अंतरभूत bool
is_ascii(अक्षर ch)
{
    वापस (ch & ~0x7f) == 0;
}

अटल अंतरभूत bool
is_space(अक्षर ch)
{
    वापस ch == ' ' || (ch >= '\t' && ch <= '\r');
}

अटल अंतरभूत bool
is_alpha(अक्षर ch)
{
    वापस (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

अटल अंतरभूत bool
is_digit(अक्षर ch)
{
    वापस ch >= '0' && ch <= '9';
}

अटल अंतरभूत bool
is_alnum(अक्षर ch)
{
    वापस is_alpha(ch) || is_digit(ch);
}

अटल अंतरभूत bool
is_xdigit(अक्षर ch)
{
    वापस
        (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

अटल अंतरभूत bool
is_graph(अक्षर ch)
{
    /* See table in ascii(7). */
    वापस ch >= '!' && ch <= '~';
}

/*
 * Return the bit position of the most significant bit.
 * Note: this is 1-based! It's more useful this way, and returns 0 when
 * mask is all 0s.
 */
अटल अंतरभूत अचिन्हित पूर्णांक
msb_pos(uint32_t mask)
{
    अचिन्हित पूर्णांक pos = 0;
    जबतक (mask) {
        pos++;
        mask >>= 1u;
    }
    वापस pos;
}

अटल अंतरभूत पूर्णांक
one_bit_set(uint32_t x)
{
    वापस x && (x & (x - 1)) == 0;
}

bool
map_file(खाता *file, अक्षर **string_out, माप_प्रकार *आकार_out);

व्योम
unmap_file(अक्षर *string, माप_प्रकार आकार);

अटल अंतरभूत bool
check_eaccess(स्थिर अक्षर *path, पूर्णांक mode)
{
#अगर defined(HAVE_EACCESS)
    अगर (eaccess(path, mode) != 0)
        वापस false;
#या_अगर defined(HAVE_EUIDACCESS)
    अगर (euidaccess(path, mode) != 0)
        वापस false;
#पूर्ण_अगर

    वापस true;
}

खाता*
open_file(स्थिर अक्षर *path);

#अगर defined(HAVE_SECURE_GETENV)
# define secure_getenv secure_getenv
#या_अगर defined(HAVE___SECURE_GETENV)
# define secure_getenv __secure_getenv
#अन्यथा
# define secure_getenv दो_पर्या
#पूर्ण_अगर

#अगर defined(HAVE___BUILTIN_EXPECT)
# define likely(x)   __builtin_expect(!!(x), 1)
# define unlikely(x) __builtin_expect(!!(x), 0)
#अन्यथा
# define likely(x)   (x)
# define unlikely(x) (x)
#पूर्ण_अगर

/* Compiler Attributes */

/* Private functions only exposed in tests. */
#अगर_घोषित ENABLE_PRIVATE_APIS
# अगर defined(__GNUC__) && !defined(__CYGWIN__)
#  define XKB_EXPORT_PRIVATE __attribute__((visibility("default")))
# elif defined(_WIN32)
#  define XKB_EXPORT_PRIVATE __declspec(dllexport)
# अन्यथा
#  define XKB_EXPORT_PRIVATE
# endif
#अन्यथा
# define XKB_EXPORT_PRIVATE
#पूर्ण_अगर

#अगर defined(__MINGW32__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__MINGW_PRINTF_FORMAT, x, y)))
#या_अगर defined(__GNUC__)
# define ATTR_PRINTF(x,y) __attribute__((__format__(__printf__, x, y)))
#अन्यथा
# define ATTR_PRINTF(x,y)
#पूर्ण_अगर

#अगर defined(__GNUC__)
# define ATTR_NORETURN __attribute__((__noreturn__))
#अन्यथा
# define ATTR_NORETURN
#पूर्ण_अगर /* GNUC  */

#अगर defined(__GNUC__)
#घोषणा ATTR_MALLOC  __attribute__((__malloc__))
#अन्यथा
#घोषणा ATTR_MALLOC
#पूर्ण_अगर

#अगर defined(__GNUC__)
# define ATTR_NULL_SENTINEL __attribute__((__sentinel__))
#अन्यथा
# define ATTR_NULL_SENTINEL
#पूर्ण_अगर

#अगर defined(__GNUC__)
#घोषणा ATTR_PACKED  __attribute__((__packed__))
#अन्यथा
#घोषणा ATTR_PACKED
#पूर्ण_अगर

#अगर defined(HAVE_ATTR_COUNTED_BY) && HAVE_ATTR_COUNTED_BY
# define ATTR_COUNTED_BY(गिनती) __attribute__((counted_by(गिनती)))
#अन्यथा
# define ATTR_COUNTED_BY(गिनती)
#पूर्ण_अगर

#अगर !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
XKB_EXPORT_PRIVATE पूर्णांक asprintf(अक्षर **strp, स्थिर अक्षर *fmt, ...) ATTR_PRINTF(2, 3);
# अगर !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
#  include <stdarg.h>
XKB_EXPORT_PRIVATE पूर्णांक vasprintf(अक्षर **strp, स्थिर अक्षर *fmt, बहु_सूची ap);
# endif /* !HAVE_VASPRINTF */
#पूर्ण_अगर /* !HAVE_ASPRINTF */

अटल अंतरभूत bool
ATTR_PRINTF(3, 4)
snprintf_safe(अक्षर *buf, माप_प्रकार sz, स्थिर अक्षर *format, ...)
{
    बहु_सूची ap;
    पूर्णांक rc;

    बहु_शुरू(ap, format);
    rc = vsnprintf(buf, sz, format, ap);
    बहु_पूर्ण(ap);

    वापस rc >= 0 && (माप_प्रकार)rc < sz;
}

अटल अंतरभूत अक्षर *
ATTR_PRINTF(1, 0)
vasprintf_safe(स्थिर अक्षर *fmt, बहु_सूची args)
{
    अक्षर *str;
    पूर्णांक len;

    len = vasprintf(&str, fmt, args);

    अगर (len == -1)
        वापस शून्य;

    वापस str;
}

/**
 * A version of asprintf that returns the allocated string or NULL on error.
 */
अटल अंतरभूत अक्षर *
ATTR_PRINTF(1, 2)
asprintf_safe(स्थिर अक्षर *fmt, ...)
{
    बहु_सूची args;
    अक्षर *str;

    बहु_शुरू(args, fmt);
    str = vasprintf_safe(fmt, args);
    बहु_पूर्ण(args);

    वापस str;
}

#अगर_घोषित न_संशोधन
    #अगर defined(__GNUC__) || defined(__clang__)
        #घोषणा PANIC_UNREACHABLE() __builtin_unreachable()
    #या_अगर defined(_MSC_VER)
        #घोषणा PANIC_UNREACHABLE() __assume(0)
    #अन्यथा
        #घोषणा PANIC_UNREACHABLE() पात()
    #पूर्ण_अगर
#अन्यथा
    #घोषणा PANIC_UNREACHABLE() \
        करो { \
            ख_लिखो(मानक_त्रुटि, "Critical Error: Reached unreachable line in %s at %d\n", __खाता__, __LINE__); \
            पात(); \
        } जबतक (0)
#पूर्ण_अगर

#अगर defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define NOINLINE [[noinline]]
#या_अगर defined(__GNUC__) || defined(__clang__)
#  define NOINLINE __attribute__((noinline))
#या_अगर defined(_MSC_VER)
#  define NOINLINE __declspec(noinline)
#अन्यथा
#  define NOINLINE
#पूर्ण_अगर
