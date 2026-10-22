/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ "utils.h"

#ఒకవేళ HAVE_MMAP

#అన్తర్భావ <unistd.h>
#అన్తర్భావ <sys/mman.h>
#అన్తర్భావ <sys/types.h>

bool
map_file(FILE *file, అక్షరం **string_బహిర్గతం, కొలత_t *కొలత_బహిర్గతం)
{
    నిర్మాణం stat stat_buf;
    పూర్ణాంకం fd;
    అక్షరం *string;

    /* Make sure to keep the errno on failure! */
    fd = fileno(file);
    ఒకవేళ (fd < 0)
        తిరిగి false;

    ఒకవేళ (fstat(fd, &stat_buf) != 0)
        తిరిగి false;

    string = mmap(ఖాళీ, (కొలత_t)stat_buf.st_కొలత, PROT_READ, MAP_SHARED, fd, 0);
    ఒకవేళ (string == MAP_FAILED)
        తిరిగి false;

    *string_బహిర్గతం = string;
    *కొలత_బహిర్గతం = (కొలత_t)stat_buf.st_కొలత;
    తిరిగి true;
}

శూన్య
unmap_file(అక్షరం *str, కొలత_t కొలత)
{
    munmap(str, కొలత);
}

#లేకపోతే

bool
map_file(FILE *file, అక్షరం **string_బహిర్గతం, కొలత_t *కొలత_బహిర్గతం)
{
    దీర్ఘ ret;
    కొలత_t ret_s;
    అక్షరం *string;
    కొలత_t కొలత;

    /* Make sure to keep the errno on failure! */

    ret = fseek(file, 0, SEEK_END);
    ఒకవేళ (ret != 0)
        తిరిగి false;

    ret = ftell(file);
    ఒకవేళ (ret < 0)
        తిరిగి false;
    కొలత = (కొలత_t) ret;

    ret = fseek(file, 0, SEEK_SET);
    ఒకవేళ (ret < 0)
        తిరిగి false;

    string = జ్ఞాపకం_ఇవ్వు(కొలత);
    ఒకవేళ (!string)
        తిరిగి false;

    ret_s = fread(string, 1, కొలత, file);
    ఒకవేళ (ret_s < కొలత) {
        విడుదల(string);
        తిరిగి false;
    }

    *string_బహిర్గతం = string;
    *కొలత_బహిర్గతం = కొలత;
    తిరిగి true;
}

శూన్య
unmap_file(అక్షరం *str, కొలత_t కొలత)
{
    విడుదల(str);
}

#అయితే_ముగింపు

/* Open a file and ensure it is a regular file.
 * Returns NULL in case of error. */
FILE*
open_file(స్థిరరాశి అక్షరం *మార్గం)
{
    ఒకవేళ (!మార్గం)
        తిరిగి ఖాళీ;

    పూర్ణాంకం fd = open(మార్గం, O_RDONLY);
    ఒకవేళ (fd < 0)
        తిరిగి ఖాళీ;

    నిర్మాణం stat stat_buf;
    పూర్ణాంకం err = fstat(fd, &stat_buf);

    ఒకవేళ (err != 0 || !S_ISREG(stat_buf.st_బహుళకం)) {
        close(fd);
        తిరిగి ఖాళీ;
    }

    /* While unlikely to happen, if `fp` is NULL then we must close the file
     * descriptor. This is poorly documented and missing from the man page; see instead:
     * https://www.ibm.com/docs/en/i/7.3?topic=functions-fdopen-associates-stream-file-descriptor)
     * Confirmed by Peter to be necessary in glibc. */
    FILE *fp = fdopen(fd, "rb");
    ఒకవేళ (fp == ఖాళీ) {
        close(fd);
    }

    తిరిగి fp;
}

// ASCII lower-case map.
స్థిర స్థిరరాశి గుర్తులేని అక్షరం క్రింది_map[] = {
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
అక్షరం
గమ్యం_క్రింది(అక్షరం c)
{
    తిరిగి (అక్షరం) క్రింది_map[(గుర్తులేని అక్షరం) c];
}

// ASCII strcasecmp (to avoid locale issues).
పూర్ణాంకం
istrcmp(స్థిరరాశి అక్షరం *a, స్థిరరాశి అక్షరం *b)
{
    కొరకు (కొలత_t i = 0; ; i++) {
        ఒకవేళ (గమ్యం_క్రింది(a[i]) != గమ్యం_క్రింది(b[i]))
            తిరిగి (పూర్ణాంకం) గమ్యం_క్రింది(a[i]) - (పూర్ణాంకం) గమ్యం_క్రింది(b[i]);
        ఒకవేళ (!a[i])
            ఆపు;
    }
    తిరిగి 0;
}

// ASCII strncasecmp (to avoid locale issues).
పూర్ణాంకం
istrncmp(స్థిరరాశి అక్షరం *a, స్థిరరాశి అక్షరం *b, కొలత_t n)
{
    కొరకు (కొలత_t i = 0; i < n; i++) {
        ఒకవేళ (గమ్యం_క్రింది(a[i]) != గమ్యం_క్రింది(b[i]))
            తిరిగి (పూర్ణాంకం) గమ్యం_క్రింది(a[i]) - (పూర్ణాంకం) గమ్యం_క్రింది(b[i]);
        ఒకవేళ (!a[i])
            ఆపు;
    }
    తిరిగి 0;
}

#ఒకవేళ !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
పూర్ణాంకం
asprintf(అక్షరం **strp, స్థిరరాశి అక్షరం *fmt, ...)
{
    పూర్ణాంకం ret;
    va_list ap;
    va_ప్రారంభం(ap, fmt);
    ret = vasprintf(strp, fmt, ap);
    va_చివర(ap);
    తిరిగి ret;
}

# ఒకవేళ !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
పూర్ణాంకం
vasprintf(అక్షరం **strp, స్థిరరాశి అక్షరం *fmt, va_list ap)
{
    పూర్ణాంకం ret;
    అక్షరం *buf;
    va_list ap_copy;

    /*
     * The value of the va_list parameter is undefined after the call to
     * vsnprintf() returns: pass a copy to make sure "ap" remains valid.
     */
    va_copy(ap_copy, ap);
    ret = vsnprintf(ఖాళీ, 0, fmt, ap_copy);
    va_చివర(ap_copy);

    ఒకవేళ (ret < 0)
        తిరిగి ret;

    ఒకవేళ (!(buf = జ్ఞాపకం_ఇవ్వు(ret + 1)))
        తిరిగి -1;

    ఒకవేళ ((ret = vsnprintf(buf, ret + 1, fmt, ap)) < 0) {
        విడుదల(buf);
        తిరిగి ret;
    }

    *strp = buf;
    తిరిగి ret;
}
# endif /* !HAVE_VASPRINTF */
#అయితే_ముగింపు /* !HAVE_ASPRINTF */
