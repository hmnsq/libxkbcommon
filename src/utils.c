/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव "utils.h"

#यदि HAVE_MMAP

#अन्तर्भाव <unistd.h>
#अन्तर्भाव <sys/mman.h>
#अन्तर्भाव <sys/types.h>

bool
map_file(FILE *file, वर्ण **string_बहिर्गमनम्, आकार_t *आकार_बहिर्गमनम्)
{
    संरचना stat stat_buf;
    पूर्णाङ्क fd;
    वर्ण *string;

    /* Make sure to keep the errno on failure! */
    fd = fileno(file);
    यदि (fd < 0)
        प्रत्यागम false;

    यदि (fstat(fd, &stat_buf) != 0)
        प्रत्यागम false;

    string = mmap(रिक्त, (आकार_t)stat_buf.st_आकार, PROT_READ, MAP_SHARED, fd, 0);
    यदि (string == MAP_FAILED)
        प्रत्यागम false;

    *string_बहिर्गमनम् = string;
    *आकार_बहिर्गमनम् = (आकार_t)stat_buf.st_आकार;
    प्रत्यागम true;
}

शून्य
unmap_file(वर्ण *str, आकार_t आकार)
{
    munmap(str, आकार);
}

#अन्यथा

bool
map_file(FILE *file, वर्ण **string_बहिर्गमनम्, आकार_t *आकार_बहिर्गमनम्)
{
    दीर्घ ret;
    आकार_t ret_s;
    वर्ण *string;
    आकार_t आकार;

    /* Make sure to keep the errno on failure! */

    ret = fseek(file, 0, SEEK_END);
    यदि (ret != 0)
        प्रत्यागम false;

    ret = ftell(file);
    यदि (ret < 0)
        प्रत्यागम false;
    आकार = (आकार_t) ret;

    ret = fseek(file, 0, SEEK_SET);
    यदि (ret < 0)
        प्रत्यागम false;

    string = स्मृतिं_देहि(आकार);
    यदि (!string)
        प्रत्यागम false;

    ret_s = fread(string, 1, आकार, file);
    यदि (ret_s < आकार) {
        मुक्त(string);
        प्रत्यागम false;
    }

    *string_बहिर्गमनम् = string;
    *आकार_बहिर्गमनम् = आकार;
    प्रत्यागम true;
}

शून्य
unmap_file(वर्ण *str, आकार_t आकार)
{
    मुक्त(str);
}

#यदि_अन्तः

/* Open a file and ensure it is a regular file.
 * Returns NULL in case of error. */
FILE*
open_file(ध्रुव वर्ण *मार्गः)
{
    यदि (!मार्गः)
        प्रत्यागम रिक्त;

    पूर्णाङ्क fd = उद्घाटय(मार्गः, O_RDONLY);
    यदि (fd < 0)
        प्रत्यागम रिक्त;

    संरचना stat stat_buf;
    पूर्णाङ्क err = fstat(fd, &stat_buf);

    यदि (err != 0 || !S_ISREG(stat_buf.st_बहुलकम्)) {
        close(fd);
        प्रत्यागम रिक्त;
    }

    /* While unlikely to happen, if `fp` is NULL then we must close the file
     * descriptor. This is poorly documented and missing from the man page; see instead:
     * https://www.ibm.com/docs/en/i/7.3?topic=functions-fdopen-associates-stream-file-descriptor)
     * Confirmed by Peter to be necessary in glibc. */
    FILE *fp = fdopen(fd, "rb");
    यदि (fp == रिक्त) {
        close(fd);
    }

    प्रत्यागम fp;
}

// ASCII lower-case map.
स्थिर ध्रुव अचिह्नित वर्ण अधस्तनम्_map[] = {
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
वर्ण
गन्तव्यम्_अधस्तनम्(वर्ण c)
{
    प्रत्यागम (वर्ण) अधस्तनम्_map[(अचिह्नित वर्ण) c];
}

// ASCII strcasecmp (to avoid locale issues).
पूर्णाङ्क
istrcmp(ध्रुव वर्ण *a, ध्रुव वर्ण *b)
{
    प्रति (आकार_t i = 0; ; i++) {
        यदि (गन्तव्यम्_अधस्तनम्(a[i]) != गन्तव्यम्_अधस्तनम्(b[i]))
            प्रत्यागम (पूर्णाङ्क) गन्तव्यम्_अधस्तनम्(a[i]) - (पूर्णाङ्क) गन्तव्यम्_अधस्तनम्(b[i]);
        यदि (!a[i])
            भङ्ग;
    }
    प्रत्यागम 0;
}

// ASCII strncasecmp (to avoid locale issues).
पूर्णाङ्क
istrncmp(ध्रुव वर्ण *a, ध्रुव वर्ण *b, आकार_t n)
{
    प्रति (आकार_t i = 0; i < n; i++) {
        यदि (गन्तव्यम्_अधस्तनम्(a[i]) != गन्तव्यम्_अधस्तनम्(b[i]))
            प्रत्यागम (पूर्णाङ्क) गन्तव्यम्_अधस्तनम्(a[i]) - (पूर्णाङ्क) गन्तव्यम्_अधस्तनम्(b[i]);
        यदि (!a[i])
            भङ्ग;
    }
    प्रत्यागम 0;
}

#यदि !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
पूर्णाङ्क
asprintf(वर्ण **strp, ध्रुव वर्ण *fmt, ...)
{
    पूर्णाङ्क ret;
    va_list ap;
    va_start(ap, fmt);
    ret = vasprintf(strp, fmt, ap);
    va_अन्तः(ap);
    प्रत्यागम ret;
}

# यदि !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
पूर्णाङ्क
vasprintf(वर्ण **strp, ध्रुव वर्ण *fmt, va_list ap)
{
    पूर्णाङ्क ret;
    वर्ण *buf;
    va_list ap_copy;

    /*
     * The value of the va_list parameter is undefined after the call to
     * vsnprintf() returns: pass a copy to make sure "ap" remains valid.
     */
    va_copy(ap_copy, ap);
    ret = vsnprintf(रिक्त, 0, fmt, ap_copy);
    va_अन्तः(ap_copy);

    यदि (ret < 0)
        प्रत्यागम ret;

    यदि (!(buf = स्मृतिं_देहि(ret + 1)))
        प्रत्यागम -1;

    यदि ((ret = vsnprintf(buf, ret + 1, fmt, ap)) < 0) {
        मुक्त(buf);
        प्रत्यागम ret;
    }

    *strp = buf;
    प्रत्यागम ret;
}
# endif /* !HAVE_VASPRINTF */
#यदि_अन्तः /* !HAVE_ASPRINTF */
