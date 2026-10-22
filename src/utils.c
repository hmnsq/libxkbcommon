/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ "utils.h"

#எனில் HAVE_MMAP

#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <sys/mman.h>
#அந்தர்பாவ <sys/types.h>

bool
map_file(FILE *file, எழுத்து **string_out, size_t *size_out)
{
    கட்டமைப்பு stat stat_buf;
    முழு fd;
    எழுத்து *string;

    /* Make sure to keep the errno on failure! */
    fd = fileno(file);
    எனில் (fd < 0)
        திரும்பு false;

    எனில் (fstat(fd, &stat_buf) != 0)
        திரும்பு false;

    string = mmap(வெற்று, (size_t)stat_buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    எனில் (string == MAP_FAILED)
        திரும்பு false;

    *string_out = string;
    *size_out = (size_t)stat_buf.st_size;
    திரும்பு true;
}

வெறுமை
unmap_file(எழுத்து *str, size_t size)
{
    munmap(str, size);
}

#இல்லையெனில்

bool
map_file(FILE *file, எழுத்து **string_out, size_t *size_out)
{
    long ret;
    size_t ret_s;
    எழுத்து *string;
    size_t size;

    /* Make sure to keep the errno on failure! */

    ret = fseek(file, 0, SEEK_END);
    எனில் (ret != 0)
        திரும்பு false;

    ret = ftell(file);
    எனில் (ret < 0)
        திரும்பு false;
    size = (size_t) ret;

    ret = fseek(file, 0, SEEK_SET);
    எனில் (ret < 0)
        திரும்பு false;

    string = malloc(size);
    எனில் (!string)
        திரும்பு false;

    ret_s = fread(string, 1, size, file);
    எனில் (ret_s < size) {
        விடுவி(string);
        திரும்பு false;
    }

    *string_out = string;
    *size_out = size;
    திரும்பு true;
}

வெறுமை
unmap_file(எழுத்து *str, size_t size)
{
    விடுவி(str);
}

#endif

/* Open a file and ensure it is a regular file.
 * Returns NULL in case of error. */
FILE*
open_file(மாறிலி எழுத்து *path)
{
    எனில் (!path)
        திரும்பு வெற்று;

    முழு fd = open(path, O_RDONLY);
    எனில் (fd < 0)
        திரும்பு வெற்று;

    கட்டமைப்பு stat stat_buf;
    முழு err = fstat(fd, &stat_buf);

    எனில் (err != 0 || !S_ISREG(stat_buf.st_mode)) {
        close(fd);
        திரும்பு வெற்று;
    }

    /* While unlikely to happen, if `fp` is NULL then we must close the file
     * descriptor. This is poorly documented and missing from the man page; see instead:
     * https://www.ibm.com/docs/en/i/7.3?topic=functions-fdopen-associates-stream-file-descriptor)
     * Confirmed by Peter to be necessary in glibc. */
    FILE *fp = fdopen(fd, "rb");
    எனில் (fp == வெற்று) {
        close(fd);
    }

    திரும்பு fp;
}

// ASCII lower-case map.
நிலையான மாறிலி unsigned எழுத்து lower_map[] = {
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
எழுத்து
to_lower(எழுத்து c)
{
    திரும்பு (எழுத்து) lower_map[(unsigned எழுத்து) c];
}

// ASCII strcasecmp (to avoid locale issues).
முழு
istrcmp(மாறிலி எழுத்து *a, மாறிலி எழுத்து *b)
{
    க்கு (size_t i = 0; ; i++) {
        எனில் (to_lower(a[i]) != to_lower(b[i]))
            திரும்பு (முழு) to_lower(a[i]) - (முழு) to_lower(b[i]);
        எனில் (!a[i])
            நிறுத்து;
    }
    திரும்பு 0;
}

// ASCII strncasecmp (to avoid locale issues).
முழு
istrncmp(மாறிலி எழுத்து *a, மாறிலி எழுத்து *b, size_t n)
{
    க்கு (size_t i = 0; i < n; i++) {
        எனில் (to_lower(a[i]) != to_lower(b[i]))
            திரும்பு (முழு) to_lower(a[i]) - (முழு) to_lower(b[i]);
        எனில் (!a[i])
            நிறுத்து;
    }
    திரும்பு 0;
}

#எனில் !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
முழு
asprintf(எழுத்து **strp, மாறிலி எழுத்து *fmt, ...)
{
    முழு ret;
    va_list ap;
    va_start(ap, fmt);
    ret = vasprintf(strp, fmt, ap);
    va_end(ap);
    திரும்பு ret;
}

# எனில் !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
முழு
vasprintf(எழுத்து **strp, மாறிலி எழுத்து *fmt, va_list ap)
{
    முழு ret;
    எழுத்து *buf;
    va_list ap_copy;

    /*
     * The value of the va_list parameter is undefined after the call to
     * vsnprintf() returns: pass a copy to make sure "ap" remains valid.
     */
    va_copy(ap_copy, ap);
    ret = vsnprintf(வெற்று, 0, fmt, ap_copy);
    va_end(ap_copy);

    எனில் (ret < 0)
        திரும்பு ret;

    எனில் (!(buf = malloc(ret + 1)))
        திரும்பு -1;

    எனில் ((ret = vsnprintf(buf, ret + 1, fmt, ap)) < 0) {
        விடுவி(buf);
        திரும்பு ret;
    }

    *strp = buf;
    திரும்பு ret;
}
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */
