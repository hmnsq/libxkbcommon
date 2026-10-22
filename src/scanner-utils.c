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
    आकार_t पङ्क्तिः, column;
    आकार_t पङ्क्तिः_pos = 0;
    यदि (s->cached_pos > s->token_pos) {
        s->cached_pos = 0;
        s->cached_loc.पङ्क्तिः = s->cached_loc.column = 1;
    }
    पङ्क्तिः = s->cached_loc.पङ्क्तिः;
    ध्रुव वर्ण *ptr = s->s + s->cached_pos;
    ध्रुव वर्ण *चरमः = s->s + s->token_pos;
    यावत् ((ptr = memchr(ptr, '\n', (आकार_t)(चरमः - ptr)))) {
        पङ्क्तिः++;
        ptr++;
        पङ्क्तिः_pos = (आकार_t)(ptr - s->s);
    }
    यदि (पङ्क्तिः == s->cached_loc.पङ्क्तिः) {
        column = s->cached_loc.column + (s->token_pos - s->cached_pos);
    } अन्यथा {
        column = s->token_pos - पङ्क्तिः_pos + 1;
    }
    संरचना scanner_loc loc = {.पङ्क्तिः = पङ्क्तिः, .column = column};
    s->cached_pos = s->token_pos;
    s->cached_loc = loc;
    प्रत्यागम loc;
}
