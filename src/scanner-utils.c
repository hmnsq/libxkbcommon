/*
 * Copyright © 2025 Ran Benita <ran@unusedvar.com>
 * SPDX-License-Identifier: MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "scanner-utils.h"

ਬਣਤਰ scanner_loc
scanner_token_location(ਬਣਤਰ scanner *s)
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
    ਮਾਪ_t ਸਤਰ, ਕਾਲਮ;
    ਮਾਪ_t ਸਤਰ_pos = 0;
    ਜੇ (s->cached_pos > s->token_pos) {
        s->cached_pos = 0;
        s->cached_loc.ਸਤਰ = s->cached_loc.ਕਾਲਮ = 1;
    }
    ਸਤਰ = s->cached_loc.ਸਤਰ;
    ਅਚਲ ਅੱਖਰ *ptr = s->s + s->cached_pos;
    ਅਚਲ ਅੱਖਰ *ਆਖਰੀ = s->s + s->token_pos;
    ਜਦੋਂ ((ptr = memchr(ptr, '\n', (ਮਾਪ_t)(ਆਖਰੀ - ptr)))) {
        ਸਤਰ++;
        ptr++;
        ਸਤਰ_pos = (ਮਾਪ_t)(ptr - s->s);
    }
    ਜੇ (ਸਤਰ == s->cached_loc.ਸਤਰ) {
        ਕਾਲਮ = s->cached_loc.ਕਾਲਮ + (s->token_pos - s->cached_pos);
    } ਨਹੀਂਤਾਂ {
        ਕਾਲਮ = s->token_pos - ਸਤਰ_pos + 1;
    }
    ਬਣਤਰ scanner_loc loc = {.ਸਤਰ = ਸਤਰ, .ਕਾਲਮ = ਕਾਲਮ};
    s->cached_pos = s->token_pos;
    s->cached_loc = loc;
    ਵਾਪਸ loc;
}
