/*
 * Copyright © 2013,2021 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

#شامل "config.h"

#شامل "xkbcommon/xkbcommon-compose.h"
#شامل "src/utils.h"
#شامل "src/context.h"

/*
 * The compose table data structure is a ternary search tree.
 *
 * Reference: https://www.drdobbs.com/database/ternary-search-trees/184410528
 * Visualization: https://www.cs.usfca.edu/~galles/visualization/TST.html
 *
 * Short example. Given these sequences:
 *
 *      <B> <C>        : "first"  dead_a
 *      <B> <D> <E>    : "second" dead_b
 *      <A> <F>        : "third"  dead_c
 *
 * the tree would look like:
 *
 *          -------- [<B>]---------
 *          |          |          #
 *          v          V
 *     -- [<A>] --   [<C>] --------
 *     #    |    #     |          |
 *          v          #     -- [<D>] --
 *     -- [<F>] --           #    |    #
 *     #    |    #                v
 *          #                -- [<E>] --
 *                           #    |    #
 *                                #
 *
 * where:
 * - [<X>] is a node for a sequence keysym <X>.
 * - right arrows are `hikid` pointers.
 * - left arrows are `lokid` pointers.
 * - down arrows are `eqkid` pointers.
 * - # is a nil pointer.
 *
 * The nodes are all kept in a contiguous array.  Pointers are represented
 * as integer offsets into this array.  A nil pointer is represented as 0
 * (which, helpfully, is the offset of an empty dummy node).
 *
 * Nodes without an eqkid are leaf nodes.  Since a sequence cannot be a
 * prefix of another, these are exactly the nodes which terminate the
 * sequences (in a bijective manner).
 *
 * A leaf contains the result data of its sequence.  The result keysym is
 * contained in the node struct itself; the result UTF-8 string is a byte
 * offset into an array of the form "\0first\0second\0third" (the initial
 * \0 is so offset 0 points to an empty string).
 */

/* 7 nodes for every potential Unicode character and then some should be
 * enough for all purposes. */
#تعریف_کرو MAX_COMPOSE_NODES_LOG2 23
#تعریف_کرو MAX_COMPOSE_NODES (1u << MAX_COMPOSE_NODES_LOG2)

ساخت compose_گرہ {
    xkb_keysym_t keysym;

    /* Offset into xkb_compose_table::nodes or 0. */
    uint32_t lokid;
    /* Offset into xkb_compose_table::nodes or 0. */
    uint32_t hikid;

    اتحاد {
        ساخت {
            uint32_t _pad:31;
            bool is_برگ:1;
        };
        ساخت {
            uint32_t _pad:31;
            bool is_برگ:1;
            /* Offset into xkb_compose_table::nodes or 0. */
            uint32_t eqkid;
        } اندرونی_2;
        ساخت {
            /* Offset into xkb_compose_table::utf8. */
            uint32_t utf8:31;
            bool is_برگ:1;
            xkb_keysym_t keysym;
        } برگ;
    };
};

ساخت xkb_compose_جدول {
    صحیح refcnt;
    شمار xkb_compose_format format;
    شمار xkb_compose_compile_flags flags;
    ساخت xkb_context *ctx;

    حرف *locale;

    darray_char utf8;
    darray(ساخت compose_گرہ) nodes;
};

ساخت xkb_compose_جدول_اندراج {
    سائز_t sequence_لمبائی;
    xkb_keysym_t *sequence ATTR_COUNTED_BY(sequence_لمبائی);
    xkb_keysym_t keysym;
    ثابت حرف *utf8;
};
