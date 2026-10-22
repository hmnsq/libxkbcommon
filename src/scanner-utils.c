/*
 * Copyright © 2025 Ran Benita <ran@unusedvar.com>
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ "scanner-utils.h"

সংরচনা scanner_loc
scanner_token_location(সংরচনা scanner *s)
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
    size_t সাৰি, column;
    size_t সাৰি_pos = 0;
    যদি (s->cached_pos > s->token_pos) {
        s->cached_pos = 0;
        s->cached_loc.সাৰি = s->cached_loc.column = 1;
    }
    সাৰি = s->cached_loc.সাৰি;
    ধ্রুৱ ৱর্ণ *ptr = s->s + s->cached_pos;
    ধ্রুৱ ৱর্ণ *last = s->s + s->token_pos;
    যেতিয়ালৈ ((ptr = memchr(ptr, '\n', (size_t)(last - ptr)))) {
        সাৰি++;
        ptr++;
        সাৰি_pos = (size_t)(ptr - s->s);
    }
    যদি (সাৰি == s->cached_loc.সাৰি) {
        column = s->cached_loc.column + (s->token_pos - s->cached_pos);
    } অন্যথা {
        column = s->token_pos - সাৰি_pos + 1;
    }
    সংরচনা scanner_loc loc = {.সাৰি = সাৰি, .column = column};
    s->cached_pos = s->token_pos;
    s->cached_loc = loc;
    ঘূৰাই loc;
}
