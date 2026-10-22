/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ "utils.h"

#ଯଦି HAVE_MMAP

#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <sys/mman.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>

bool
map_file(FILE *file, ଅକ୍ଷର **string_out, size_t *size_out)
{
    ସଂରଚନା stat stat_buf;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
    ଅକ୍ଷର *string;

    /* Make sure to keep the errno on failure! */
    fd = fileno(file);
    ଯଦି (fd < 0)
        ଫେରନ୍ତୁ false;

    ଯଦି (fstat(fd, &stat_buf) != 0)
        ଫେରନ୍ତୁ false;

    string = mmap(ଖାଲି, (size_t)stat_buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    ଯଦି (string == MAP_FAILED)
        ଫେରନ୍ତୁ false;

    *string_out = string;
    *size_out = (size_t)stat_buf.st_size;
    ଫେରନ୍ତୁ true;
}

ଶୂନ୍ୟ
unmap_file(ଅକ୍ଷର *str, size_t size)
{
    munmap(str, size);
}

#ଅନ୍ୟଥା

bool
map_file(FILE *file, ଅକ୍ଷର **string_out, size_t *size_out)
{
    long ret;
    size_t ret_s;
    ଅକ୍ଷର *string;
    size_t size;

    /* Make sure to keep the errno on failure! */

    ret = fseek(file, 0, SEEK_END);
    ଯଦି (ret != 0)
        ଫେରନ୍ତୁ false;

    ret = ftell(file);
    ଯଦି (ret < 0)
        ଫେରନ୍ତୁ false;
    size = (size_t) ret;

    ret = fseek(file, 0, SEEK_SET);
    ଯଦି (ret < 0)
        ଫେରନ୍ତୁ false;

    string = malloc(size);
    ଯଦି (!string)
        ଫେରନ୍ତୁ false;

    ret_s = fread(string, 1, size, file);
    ଯଦି (ret_s < size) {
        ମୁକ୍ତ(string);
        ଫେରନ୍ତୁ false;
    }

    *string_out = string;
    *size_out = size;
    ଫେରନ୍ତୁ true;
}

ଶୂନ୍ୟ
unmap_file(ଅକ୍ଷର *str, size_t size)
{
    ମୁକ୍ତ(str);
}

#endif

/* Open a file and ensure it is a regular file.
 * Returns NULL in case of error. */
FILE*
open_file(ଅଚଳ ଅକ୍ଷର *path)
{
    ଯଦି (!path)
        ଫେରନ୍ତୁ ଖାଲି;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = open(path, O_RDONLY);
    ଯଦି (fd < 0)
        ଫେରନ୍ତୁ ଖାଲି;

    ସଂରଚନା stat stat_buf;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = fstat(fd, &stat_buf);

    ଯଦି (err != 0 || !S_ISREG(stat_buf.st_mode)) {
        close(fd);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    /* While unlikely to happen, if `fp` is NULL then we must close the file
     * descriptor. This is poorly documented and missing from the man page; see instead:
     * https://www.ibm.com/docs/en/i/7.3?topic=functions-fdopen-associates-stream-file-descriptor)
     * Confirmed by Peter to be necessary in glibc. */
    FILE *fp = fdopen(fd, "rb");
    ଯଦି (fp == ଖାଲି) {
        close(fd);
    }

    ଫେରନ୍ତୁ fp;
}

// ASCII lower-case map.
ସ୍ଥିର ଅଚଳ unsigned ଅକ୍ଷର lower_map[] = {
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
ଅକ୍ଷର
to_lower(ଅକ୍ଷର c)
{
    ଫେରନ୍ତୁ (ଅକ୍ଷର) lower_map[(unsigned ଅକ୍ଷର) c];
}

// ASCII strcasecmp (to avoid locale issues).
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
istrcmp(ଅଚଳ ଅକ୍ଷର *a, ଅଚଳ ଅକ୍ଷର *b)
{
    ପାଇଁ (size_t i = 0; ; i++) {
        ଯଦି (to_lower(a[i]) != to_lower(b[i]))
            ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) to_lower(a[i]) - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) to_lower(b[i]);
        ଯଦି (!a[i])
            ଭାଙ୍ଗ;
    }
    ଫେରନ୍ତୁ 0;
}

// ASCII strncasecmp (to avoid locale issues).
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
istrncmp(ଅଚଳ ଅକ୍ଷର *a, ଅଚଳ ଅକ୍ଷର *b, size_t n)
{
    ପାଇଁ (size_t i = 0; i < n; i++) {
        ଯଦି (to_lower(a[i]) != to_lower(b[i]))
            ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) to_lower(a[i]) - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) to_lower(b[i]);
        ଯଦି (!a[i])
            ଭାଙ୍ଗ;
    }
    ଫେରନ୍ତୁ 0;
}

#ଯଦି !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
asprintf(ଅକ୍ଷର **strp, ଅଚଳ ଅକ୍ଷର *fmt, ...)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
    va_list ap;
    va_start(ap, fmt);
    ret = vasprintf(strp, fmt, ap);
    va_end(ap);
    ଫେରନ୍ତୁ ret;
}

# ଯଦି !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
vasprintf(ଅକ୍ଷର **strp, ଅଚଳ ଅକ୍ଷର *fmt, va_list ap)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
    ଅକ୍ଷର *buf;
    va_list ap_copy;

    /*
     * The value of the va_list parameter is undefined after the call to
     * vsnprintf() returns: pass a copy to make sure "ap" remains valid.
     */
    va_copy(ap_copy, ap);
    ret = vsnprintf(ଖାଲି, 0, fmt, ap_copy);
    va_end(ap_copy);

    ଯଦି (ret < 0)
        ଫେରନ୍ତୁ ret;

    ଯଦି (!(buf = malloc(ret + 1)))
        ଫେରନ୍ତୁ -1;

    ଯଦି ((ret = vsnprintf(buf, ret + 1, fmt, ap)) < 0) {
        ମୁକ୍ତ(buf);
        ଫେରନ୍ତୁ ret;
    }

    *strp = buf;
    ଫେରନ୍ତୁ ret;
}
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */
