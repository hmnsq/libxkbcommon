/*
 * Copyright © 2025 Ran Benita <ran@unusedvar.com>
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव "scanner-utils.h"

संरचना scanner_loc
scanner_token_location(संरचना scanner *s)
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
    size_t ओळ, column;
    size_t ओळ_pos = 0;
    यदि (s->cached_pos > s->token_pos) {
        s->cached_pos = 0;
        s->cached_loc.ओळ = s->cached_loc.column = 1;
    }
    ओळ = s->cached_loc.ओळ;
    ध्रुव वर्ण *ptr = s->s + s->cached_pos;
    ध्रुव वर्ण *last = s->s + s->token_pos;
    यावत् ((ptr = memchr(ptr, '\n', (size_t)(last - ptr)))) {
        ओळ++;
        ptr++;
        ओळ_pos = (size_t)(ptr - s->s);
    }
    यदि (ओळ == s->cached_loc.ओळ) {
        column = s->cached_loc.column + (s->token_pos - s->cached_pos);
    } अन्यथा {
        column = s->token_pos - ओळ_pos + 1;
    }
    संरचना scanner_loc loc = {.ओळ = ओळ, .column = column};
    s->cached_pos = s->token_pos;
    s->cached_loc = loc;
    प्रत्यागम loc;
}
