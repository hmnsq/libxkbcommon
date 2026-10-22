/*
 * Copyright © 2025 Ran Benita <ran@unusedvar.com>
 * SPDX-License-Identifier: MIT
 */

#include "config.h"

#include "scanner-utils.h"

struct scanner_loc
scanner_token_location(struct scanner *s)
{
    /*
     * The following article and accompying code compares some algorithms:
     * https://lemire.me/blog/2017/02/14/how-fast-can-you-count-lines/
     * https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/blob/master/2017/02/14/newlines.c
     * This is the fastest portable one for me, and it's simple.
     *
     * To avoid rescanning on each call, cache the previously found position
     * and start from that on the next call. This is effective as long as the
     * tokens go forward.
     */
    size_t लिन्या, column;
    size_t लिन्या_pos = 0;
    if (s->cached_pos > s->token_pos) {
        s->cached_pos = 0;
        s->cached_loc.लिन्या = s->cached_loc.column = 1;
    }
    लिन्या = s->cached_loc.लिन्या;
    const char *ptr = s->s + s->cached_pos;
    const char *last = s->s + s->token_pos;
    while ((ptr = memchr(ptr, '\n', (size_t)(last - ptr)))) {
        लिन्या++;
        ptr++;
        लिन्या_pos = (size_t)(ptr - s->s);
    }
    if (लिन्या == s->cached_loc.लिन्या) {
        column = s->cached_loc.column + (s->token_pos - s->cached_pos);
    } else {
        column = s->token_pos - लिन्या_pos + 1;
    }
    struct scanner_loc loc = {.लिन्या = लिन्या, .column = column};
    s->cached_pos = s->token_pos;
    s->cached_loc = loc;
    return loc;
}
