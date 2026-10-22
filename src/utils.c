/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#include "config.h"

#include <sys/stat.h>
#include <fcntl.h>
#include "utils.h"

#အကယ်၍ HAVE_MMAP

#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>

bool
map_file(FILE *file, var_na **string_out, size_t *size_out)
{
    sa_nracanaa stat stat_buf;
    ကိန်းပြည့် fd;
    var_na *string;

    /* Make sure to keep the errno on failure! */
    fd = fileno(file);
    အကယ်၍ (fd < 0)
        ပြန်ပေး false;

    အကယ်၍ (fstat(fd, &stat_buf) != 0)
        ပြန်ပေး false;

    string = mmap(NULL, (size_t)stat_buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    အကယ်၍ (string == MAP_FAILED)
        ပြန်ပေး false;

    *string_out = string;
    *size_out = (size_t)stat_buf.st_size;
    ပြန်ပေး true;
}

void
unmap_file(var_na *str, size_t size)
{
    munmap(str, size);
}

#မဟုတ်ပါက

bool
map_file(FILE *file, var_na **string_out, size_t *size_out)
{
    long ret;
    size_t ret_s;
    var_na *string;
    size_t size;

    /* Make sure to keep the errno on failure! */

    ret = fseek(file, 0, SEEK_END);
    အကယ်၍ (ret != 0)
        ပြန်ပေး false;

    ret = ftell(file);
    အကယ်၍ (ret < 0)
        ပြန်ပေး false;
    size = (size_t) ret;

    ret = fseek(file, 0, SEEK_SET);
    အကယ်၍ (ret < 0)
        ပြန်ပေး false;

    string = malloc(size);
    အကယ်၍ (!string)
        ပြန်ပေး false;

    ret_s = fread(string, 1, size, file);
    အကယ်၍ (ret_s < size) {
        free(string);
        ပြန်ပေး false;
    }

    *string_out = string;
    *size_out = size;
    ပြန်ပေး true;
}

void
unmap_file(var_na *str, size_t size)
{
    free(str);
}

#endif

/* Open a file and ensure it is a regular file.
 * Returns NULL in case of error. */
FILE*
open_file(const var_na *path)
{
    အကယ်၍ (!path)
        ပြန်ပေး NULL;

    ကိန်းပြည့် fd = open(path, O_RDONLY);
    အကယ်၍ (fd < 0)
        ပြန်ပေး NULL;

    sa_nracanaa stat stat_buf;
    ကိန်းပြည့် err = fstat(fd, &stat_buf);

    အကယ်၍ (err != 0 || !S_ISREG(stat_buf.st_mode)) {
        close(fd);
        ပြန်ပေး NULL;
    }

    /* While unlikely to happen, if `fp` is NULL then we must close the file
     * descriptor. This is poorly documented and missing from the man page; see instead:
     * https://www.ibm.com/docs/en/i/7.3?topic=functions-fdopen-associates-stream-file-descriptor)
     * Confirmed by Peter to be necessary in glibc. */
    FILE *fp = fdopen(fd, "rb");
    အကယ်၍ (fp == NULL) {
        close(fd);
    }

    ပြန်ပေး fp;
}

// ASCII lower-case map.
static const unsigned var_na lower_map[] = {
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
var_na
to_lower(var_na c)
{
    ပြန်ပေး (var_na) lower_map[(unsigned var_na) c];
}

// ASCII strcasecmp (to avoid locale issues).
ကိန်းပြည့်
istrcmp(const var_na *a, const var_na *b)
{
    အတွက် (size_t i = 0; ; i++) {
        အကယ်၍ (to_lower(a[i]) != to_lower(b[i]))
            ပြန်ပေး (ကိန်းပြည့်) to_lower(a[i]) - (ကိန်းပြည့်) to_lower(b[i]);
        အကယ်၍ (!a[i])
            bha_nga;
    }
    ပြန်ပေး 0;
}

// ASCII strncasecmp (to avoid locale issues).
ကိန်းပြည့်
istrncmp(const var_na *a, const var_na *b, size_t n)
{
    အတွက် (size_t i = 0; i < n; i++) {
        အကယ်၍ (to_lower(a[i]) != to_lower(b[i]))
            ပြန်ပေး (ကိန်းပြည့်) to_lower(a[i]) - (ကိန်းပြည့်) to_lower(b[i]);
        အကယ်၍ (!a[i])
            bha_nga;
    }
    ပြန်ပေး 0;
}

#အကယ်၍ !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
ကိန်းပြည့်
asprintf(var_na **strp, const var_na *fmt, ...)
{
    ကိန်းပြည့် ret;
    va_list ap;
    va_start(ap, fmt);
    ret = vasprintf(strp, fmt, ap);
    va_end(ap);
    ပြန်ပေး ret;
}

# အကယ်၍ !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
ကိန်းပြည့်
vasprintf(var_na **strp, const var_na *fmt, va_list ap)
{
    ကိန်းပြည့် ret;
    var_na *buf;
    va_list ap_copy;

    /*
     * The value of the va_list parameter is undefined after the call to
     * vsnprintf() returns: pass a copy to make sure "ap" remains valid.
     */
    va_copy(ap_copy, ap);
    ret = vsnprintf(NULL, 0, fmt, ap_copy);
    va_end(ap_copy);

    အကယ်၍ (ret < 0)
        ပြန်ပေး ret;

    အကယ်၍ (!(buf = malloc(ret + 1)))
        ပြန်ပေး -1;

    အကယ်၍ ((ret = vsnprintf(buf, ret + 1, fmt, ap)) < 0) {
        free(buf);
        ပြန်ပေး ret;
    }

    *strp = buf;
    ပြန်ပေး ret;
}
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */
