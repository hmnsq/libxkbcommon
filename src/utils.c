/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ "utils.h"

#જો HAVE_MMAP

#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <sys/mman.h>
#અન્તર્ભાવ <sys/types.h>

bool
map_file(FILE *file, અક્ષર **string_out, size_t *size_out)
{
    સંરચના stat stat_buf;
    પૂર્ણાંક fd;
    અક્ષર *string;

    /* Make sure to keep the errno on failure! */
    fd = fileno(file);
    જો (fd < 0)
        પાછા false;

    જો (fstat(fd, &stat_buf) != 0)
        પાછા false;

    string = mmap(ખાલી, (size_t)stat_buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    જો (string == MAP_FAILED)
        પાછા false;

    *string_out = string;
    *size_out = (size_t)stat_buf.st_size;
    પાછા true;
}

શૂન્ય
unmap_file(અક્ષર *str, size_t size)
{
    munmap(str, size);
}

#નહીંતર

bool
map_file(FILE *file, અક્ષર **string_out, size_t *size_out)
{
    long ret;
    size_t ret_s;
    અક્ષર *string;
    size_t size;

    /* Make sure to keep the errno on failure! */

    ret = fseek(file, 0, SEEK_END);
    જો (ret != 0)
        પાછા false;

    ret = ftell(file);
    જો (ret < 0)
        પાછા false;
    size = (size_t) ret;

    ret = fseek(file, 0, SEEK_SET);
    જો (ret < 0)
        પાછા false;

    string = malloc(size);
    જો (!string)
        પાછા false;

    ret_s = fread(string, 1, size, file);
    જો (ret_s < size) {
        મુક્ત(string);
        પાછા false;
    }

    *string_out = string;
    *size_out = size;
    પાછા true;
}

શૂન્ય
unmap_file(અક્ષર *str, size_t size)
{
    મુક્ત(str);
}

#endif

/* Open a file and ensure it is a regular file.
 * Returns NULL in case of error. */
FILE*
open_file(અચલ અક્ષર *path)
{
    જો (!path)
        પાછા ખાલી;

    પૂર્ણાંક fd = open(path, O_RDONLY);
    જો (fd < 0)
        પાછા ખાલી;

    સંરચના stat stat_buf;
    પૂર્ણાંક err = fstat(fd, &stat_buf);

    જો (err != 0 || !S_ISREG(stat_buf.st_mode)) {
        close(fd);
        પાછા ખાલી;
    }

    /* While unlikely to happen, if `fp` is NULL then we must close the file
     * descriptor. This is poorly documented and missing from the man page; see instead:
     * https://www.ibm.com/docs/en/i/7.3?topic=functions-fdopen-associates-stream-file-descriptor)
     * Confirmed by Peter to be necessary in glibc. */
    FILE *fp = fdopen(fd, "rb");
    જો (fp == ખાલી) {
        close(fd);
    }

    પાછા fp;
}

// ASCII lower-case map.
સ્થિર અચલ unsigned અક્ષર lower_map[] = {
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
અક્ષર
to_lower(અક્ષર c)
{
    પાછા (અક્ષર) lower_map[(unsigned અક્ષર) c];
}

// ASCII strcasecmp (to avoid locale issues).
પૂર્ણાંક
istrcmp(અચલ અક્ષર *a, અચલ અક્ષર *b)
{
    માટે (size_t i = 0; ; i++) {
        જો (to_lower(a[i]) != to_lower(b[i]))
            પાછા (પૂર્ણાંક) to_lower(a[i]) - (પૂર્ણાંક) to_lower(b[i]);
        જો (!a[i])
            તોડો;
    }
    પાછા 0;
}

// ASCII strncasecmp (to avoid locale issues).
પૂર્ણાંક
istrncmp(અચલ અક્ષર *a, અચલ અક્ષર *b, size_t n)
{
    માટે (size_t i = 0; i < n; i++) {
        જો (to_lower(a[i]) != to_lower(b[i]))
            પાછા (પૂર્ણાંક) to_lower(a[i]) - (પૂર્ણાંક) to_lower(b[i]);
        જો (!a[i])
            તોડો;
    }
    પાછા 0;
}

#જો !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
પૂર્ણાંક
asprintf(અક્ષર **strp, અચલ અક્ષર *fmt, ...)
{
    પૂર્ણાંક ret;
    va_list ap;
    va_start(ap, fmt);
    ret = vasprintf(strp, fmt, ap);
    va_end(ap);
    પાછા ret;
}

# જો !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
પૂર્ણાંક
vasprintf(અક્ષર **strp, અચલ અક્ષર *fmt, va_list ap)
{
    પૂર્ણાંક ret;
    અક્ષર *buf;
    va_list ap_copy;

    /*
     * The value of the va_list parameter is undefined after the call to
     * vsnprintf() returns: pass a copy to make sure "ap" remains valid.
     */
    va_copy(ap_copy, ap);
    ret = vsnprintf(ખાલી, 0, fmt, ap_copy);
    va_end(ap_copy);

    જો (ret < 0)
        પાછા ret;

    જો (!(buf = malloc(ret + 1)))
        પાછા -1;

    જો ((ret = vsnprintf(buf, ret + 1, fmt, ap)) < 0) {
        મુક્ત(buf);
        પાછા ret;
    }

    *strp = buf;
    પાછા ret;
}
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */
