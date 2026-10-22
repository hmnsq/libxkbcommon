/*
 * Copyright © 2025 Ran Benita <ran@unusedvar.com>
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব "scanner-utils.h"

কাঠামো scanner_loc
scanner_token_location(কাঠামো scanner *s)
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
    আকার_t সারি, column;
    আকার_t সারি_pos = 0;
    যদি (s->cached_pos > s->token_pos) {
        s->cached_pos = 0;
        s->cached_loc.সারি = s->cached_loc.column = 1;
    }
    সারি = s->cached_loc.সারি;
    ধ্রুবক অক্ষর *ptr = s->s + s->cached_pos;
    ধ্রুবক অক্ষর *last = s->s + s->token_pos;
    যখন ((ptr = memchr(ptr, '\n', (আকার_t)(last - ptr)))) {
        সারি++;
        ptr++;
        সারি_pos = (আকার_t)(ptr - s->s);
    }
    যদি (সারি == s->cached_loc.সারি) {
        column = s->cached_loc.column + (s->token_pos - s->cached_pos);
    } অন্যথা {
        column = s->token_pos - সারি_pos + 1;
    }
    কাঠামো scanner_loc loc = {.সারি = সারি, .column = column};
    s->cached_pos = s->token_pos;
    s->cached_loc = loc;
    ফেরত loc;
}
