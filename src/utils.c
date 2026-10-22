/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव "utils.h"

#जर HAVE_MMAP

#अन्तर्भाव <unistd.h>
#अन्तर्भाव <sys/mman.h>
#अन्तर्भाव <sys/types.h>

bool
map_file(खाता *file, अक्षर **string_out, माप_प्रकार *size_out)
{
    संरचना stat stat_buf;
    पूर्णांक fd;
    अक्षर *string;

    /* Make sure to keep the errno on failure! */
    fd = fileno(file);
    जर (fd < 0)
        परत false;

    जर (ख_स्थिति(fd, &stat_buf) != 0)
        परत false;

    string = mmap(रिक्त, (माप_प्रकार)stat_buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    जर (string == MAP_FAILED)
        परत false;

    *string_out = string;
    *size_out = (माप_प्रकार)stat_buf.st_size;
    परत true;
}

शून्य
unmap_file(अक्षर *str, माप_प्रकार size)
{
    munmap(str, size);
}

#अन्यथा

bool
map_file(खाता *file, अक्षर **string_out, माप_प्रकार *size_out)
{
    दीर्घ ret;
    माप_प्रकार ret_s;
    अक्षर *string;
    माप_प्रकार size;

    /* Make sure to keep the errno on failure! */

    ret = ख_जाओ(file, 0, अंत_से);
    जर (ret != 0)
        परत false;

    ret = ख_बताओ(file);
    जर (ret < 0)
        परत false;
    size = (माप_प्रकार) ret;

    ret = ख_जाओ(file, 0, शुरू_से);
    जर (ret < 0)
        परत false;

    string = दो_स्मृति(size);
    जर (!string)
        परत false;

    ret_s = ख_पढ़ो(string, 1, size, file);
    जर (ret_s < size) {
        मुक्त(string);
        परत false;
    }

    *string_out = string;
    *size_out = size;
    परत true;
}

शून्य
unmap_file(अक्षर *str, माप_प्रकार size)
{
    मुक्त(str);
}

#endif

/* Open a file and ensure it is a regular file.
 * Returns NULL in case of error. */
खाता*
open_file(स्थिरांक अक्षर *path)
{
    जर (!path)
        परत रिक्त;

    पूर्णांक fd = खोलो(path, O_RDONLY);
    जर (fd < 0)
        परत रिक्त;

    संरचना stat stat_buf;
    पूर्णांक err = ख_स्थिति(fd, &stat_buf);

    जर (err != 0 || !S_ISREG(stat_buf.st_mode)) {
        बंद(fd);
        परत रिक्त;
    }

    /* While unlikely to happen, if `fp` is NULL then we must close the file
     * descriptor. This is poorly documented and missing from the man page; see instead:
     * https://www.ibm.com/docs/en/i/7.3?topic=functions-fdopen-associates-stream-file-descriptor)
     * Confirmed by Peter to be necessary in glibc. */
    खाता *fp = fdopen(fd, "rb");
    जर (fp == रिक्त) {
        बंद(fd);
    }

    परत fp;
}

// ASCII lower-case map.
स्थिर स्थिरांक अचिन्हित अक्षर lower_map[] = {
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
अक्षर
to_lower(अक्षर c)
{
    परत (अक्षर) lower_map[(अचिन्हित अक्षर) c];
}

// ASCII strcasecmp (to avoid locale issues).
पूर्णांक
istrcmp(स्थिरांक अक्षर *a, स्थिरांक अक्षर *b)
{
    साठी (माप_प्रकार i = 0; ; i++) {
        जर (to_lower(a[i]) != to_lower(b[i]))
            परत (पूर्णांक) to_lower(a[i]) - (पूर्णांक) to_lower(b[i]);
        जर (!a[i])
            थांबा;
    }
    परत 0;
}

// ASCII strncasecmp (to avoid locale issues).
पूर्णांक
istrncmp(स्थिरांक अक्षर *a, स्थिरांक अक्षर *b, माप_प्रकार n)
{
    साठी (माप_प्रकार i = 0; i < n; i++) {
        जर (to_lower(a[i]) != to_lower(b[i]))
            परत (पूर्णांक) to_lower(a[i]) - (पूर्णांक) to_lower(b[i]);
        जर (!a[i])
            थांबा;
    }
    परत 0;
}

#जर !(defined(HAVE_ASPRINTF) && HAVE_ASPRINTF)
पूर्णांक
asprintf(अक्षर **strp, स्थिरांक अक्षर *fmt, ...)
{
    पूर्णांक ret;
    बहु_सूची ap;
    बहु_शुरू(ap, fmt);
    ret = vasprintf(strp, fmt, ap);
    बहु_पूर्ण(ap);
    परत ret;
}

# जर !(defined(HAVE_VASPRINTF) && HAVE_VASPRINTF)
पूर्णांक
vasprintf(अक्षर **strp, स्थिरांक अक्षर *fmt, बहु_सूची ap)
{
    पूर्णांक ret;
    अक्षर *buf;
    बहु_सूची ap_copy;

    /*
     * The value of the va_list parameter is undefined after the call to
     * vsnprintf() returns: pass a copy to make sure "ap" remains valid.
     */
    va_copy(ap_copy, ap);
    ret = vsnprintf(रिक्त, 0, fmt, ap_copy);
    बहु_पूर्ण(ap_copy);

    जर (ret < 0)
        परत ret;

    जर (!(buf = दो_स्मृति(ret + 1)))
        परत -1;

    जर ((ret = vsnprintf(buf, ret + 1, fmt, ap)) < 0) {
        मुक्त(buf);
        परत ret;
    }

    *strp = buf;
    परत ret;
}
# endif /* !HAVE_VASPRINTF */
#endif /* !HAVE_ASPRINTF */
