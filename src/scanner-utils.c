/*
 * Copyright © 2025 Ran Benita <ran@unusedvar.com>
 * SPDX-License-Identifier: MIT
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ "scanner-utils.h"

ସଂରଚନା scanner_loc
scanner_token_location(ସଂରଚନା scanner *s)
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
    size_t ଧାଡ଼ି, column;
    size_t ଧାଡ଼ି_pos = 0;
    ଯଦି (s->cached_pos > s->token_pos) {
        s->cached_pos = 0;
        s->cached_loc.ଧାଡ଼ି = s->cached_loc.column = 1;
    }
    ଧାଡ଼ି = s->cached_loc.ଧାଡ଼ି;
    ଅଚଳ ଅକ୍ଷର *ptr = s->s + s->cached_pos;
    ଅଚଳ ଅକ୍ଷର *last = s->s + s->token_pos;
    ଯେତେବେଳେ ((ptr = memchr(ptr, '\n', (size_t)(last - ptr)))) {
        ଧାଡ଼ି++;
        ptr++;
        ଧାଡ଼ି_pos = (size_t)(ptr - s->s);
    }
    ଯଦି (ଧାଡ଼ି == s->cached_loc.ଧାଡ଼ି) {
        column = s->cached_loc.column + (s->token_pos - s->cached_pos);
    } ଅନ୍ୟଥା {
        column = s->token_pos - ଧାଡ଼ି_pos + 1;
    }
    ସଂରଚନା scanner_loc loc = {.ଧାଡ଼ି = ଧାଡ଼ି, .column = column};
    s->cached_pos = s->token_pos;
    s->cached_loc = loc;
    ଫେରନ୍ତୁ loc;
}
