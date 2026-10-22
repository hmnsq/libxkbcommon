/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن <sys/stat.h>
#ضمن <fcntl.h>
#ضمن "utils.h"

#إذا HAVE_MMAP

#ضمن <unistd.h>
#ضمن <sys/mman.h>
#ضمن <sys/types.h>

bool
map_file(FILE *file, حرف **string_بهر, اندازه_t *اندازه_بهر)
{
    بنية stat stat_buf;
    صحيح fd;
    حرف *string;

    /* Make sure to keep the errno on failure! */
    fd = fileno(file);
    إذا (fd < 0)
        أرجع false;

    إذا (fstat(fd, &stat_buf) != 0)
        أرجع false;

    string = mmap(عدم, (اندازه_t)stat_buf.st_اندازه, PROT_READ, MAP_SHARED, fd, 0);
    إذا (string == MAP_FAILED)
        أرجع false;

    *string_بهر = string;
    *اندازه_بهر = (اندازه_t)stat_buf.st_اندازه;
    أرجع true;
}

فراغ
unmap_file(حرف *str, اندازه_t اندازه)
{
    munmap(str, اندازه);
}

#وإلا

bool
map_file(FILE *file, حرف **string_بهر, اندازه_t *اندازه_بهر)
{
    اوږد ret;
    اندازه_t ret_s;
    حرف *string;
    اندازه_t اندازه;

    /* Make sure to keep the errno on failure! */

    ret = fseek(file, 0, SEEK_END);
    إذا (ret != 0)
        أرجع false;

    ret = ftell(file);
    إذا (ret < 0)
        أرجع false;
    اندازه = (اندازه_t) ret;

    ret = fseek(file, 0, SEEK_SET);
    إذا (ret < 0)
        أرجع false;

    string = حافظه_ورکړه(اندازه);
    إذا (!string)
        أرجع false;

    ret_s = fread(string, 1, اندازه, file);
    إذا (ret_s < اندازه) {
        حرر(string);
        أرجع false;
    }

    *string_بهر = string;
    *اندازه_بهر = اندازه;
    أرجع true;
}

فراغ
unmap_file(حرف *str, اندازه_t اندازه)
{
    حرر(str);
}

#د_که_پای

/* Open a file and ensure it is a regular file.
 * Returns NULL in case of error. */
FILE*
open_file(ثابت حرف *لار)
{
    إذا (!لار)
        أرجع عدم;

    صحيح fd = open(لار, O_RDONLY);
    إذا (fd < 0)
        أرجع عدم;

    بنية stat stat_buf;
    صحيح err = fstat(fd, &stat_buf);

    إذا (err != 0 || !S_ISREG(stat_buf.st_ډېرپېښ)) {
        close(fd);
        أرجع عدم;
    }

    /* While unlikely to happen, if `fp` is NULL then we must close the file
     * descriptor. This is poorly documented and missing from the man page; see instead:
     * https://www.ibm.com/docs/en/i/7.3?topic=functions-fdopen-associates-stream-file-descriptor)
     * Confirmed by Peter to be necessary in glibc. */
    FILE *fp = fdopen(fd, "rb");
    إذا (fp == عدم) {
        close(fd);
    }

    أرجع fp;
}

// ASCII lower-case map.
ساكن ثابت بینښه حرف ښکتنی_map[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58,
    59, 60, 61, 62, 63, 64, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107,
    108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
    91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107,
    108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
    123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137,
    138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152,
    153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167,
    168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182,
    183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197,
    198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212,
    213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
    228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242,
    243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
};

// ASCII tolower (to avoid locale issues).
حرف
منزل_ښکتنی(حرف c)
{
    أرجع (حرف) ښکتنی_map[(بینښه حرف) c];
}

// ASCII strcasecmp (to avoid locale issues).
صحيح
istrcmp(ثابت حرف *a, ثابت حرف *b)
{
    لكل (اندازه_t i = 0; ; i++) {
        إذا (منزل_ښکتنی(a[i]) != منزل_ښکتنی(b[i]))
            أرجع (صحيح) منزل_ښکتنی(a[i]) - (صحيح) منزل_ښکتنی(b[i]);
        إذا (!a[i])
            اكسر;
    }
    أرجع 0;
}

// ASCII strncasecmp (to avoid locale issues).
صحيح
istrncmp(ثابت حرف *a, ثابت حرف *b, اندازه_t n)
{
    لكل (اندازه_t i = 0; i < n; i++) {
        إذا (منزل_ښکتنی(a[i]) != منزل_ښکتنی(b[i]))
            أرجع (صحيح) منزل_ښکتنی(a[i]) - (صحيح) منزل_ښکتنی(b[i]);
        إذا (!a[i])
            اكسر;
    }
    أرجع 0;
}

#إذا !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
صحيح
asprintf(حرف **strp, ثابت حرف *fmt, ...)
{
    صحيح ret;
    va_list ap;
    va_پیل(ap, fmt);
    ret = vasprintf(strp, fmt, ap);
    va_پای_ټکی(ap);
    أرجع ret;
}

# إذا !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
صحيح
vasprintf(حرف **strp, ثابت حرف *fmt, va_list ap)
{
    صحيح ret;
    حرف *buf;
    va_list ap_copy;

    /*
     * The value of the va_list parameter is undefined after the call to
     * vsnprintf() returns: pass a copy to make sure "ap" remains valid.
     */
    va_copy(ap_copy, ap);
    ret = vsnprintf(عدم, 0, fmt, ap_copy);
    va_پای_ټکی(ap_copy);

    إذا (ret < 0)
        أرجع ret;

    إذا (!(buf = حافظه_ورکړه(ret + 1)))
        أرجع -1;

    إذا ((ret = vsnprintf(buf, ret + 1, fmt, ap)) < 0) {
        حرر(buf);
        أرجع ret;
    }

    *strp = buf;
    أرجع ret;
}
# endif /* !HAVE_VASPRINTF */
#د_که_پای /* !HAVE_ASPRINTF */
