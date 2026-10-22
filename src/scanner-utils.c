/*
 * Copyright © 2025 Ran Benita <ran@unusedvar.com>
 * SPDX-License-Identifier: MIT
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ "scanner-utils.h"

ഘടന scanner_loc
scanner_token_location(ഘടന scanner *s)
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
    size_t വരി, column;
    size_t വരി_pos = 0;
    എങ്കിൽ (s->cached_pos > s->token_pos) {
        s->cached_pos = 0;
        s->cached_loc.വരി = s->cached_loc.column = 1;
    }
    വരി = s->cached_loc.വരി;
    സ്ഥിരം അക്ഷരം *ptr = s->s + s->cached_pos;
    സ്ഥിരം അക്ഷരം *last = s->s + s->token_pos;
    ആകുമ്പോൾ ((ptr = memchr(ptr, '\n', (size_t)(last - ptr)))) {
        വരി++;
        ptr++;
        വരി_pos = (size_t)(ptr - s->s);
    }
    എങ്കിൽ (വരി == s->cached_loc.വരി) {
        column = s->cached_loc.column + (s->token_pos - s->cached_pos);
    } അല്ലെങ്കിൽ {
        column = s->token_pos - വരി_pos + 1;
    }
    ഘടന scanner_loc loc = {.വരി = വരി, .column = column};
    s->cached_pos = s->token_pos;
    s->cached_loc = loc;
    തിരികെ loc;
}
