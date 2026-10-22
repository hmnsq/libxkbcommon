/*
 * Copyright © 2013,2021 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل <assert.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "features/enums.h"
#شامل "constants.h"
#شامل "messages-codes.h"
#شامل "parser.h"
#شامل "paths.h"
#شامل "table.h"
#شامل "utils.h"

ساکن ساخت xkb_compose_جدول *
xkb_compose_جدول_new(ساخت xkb_context *ctx, ثابت حرف *func,
                      ثابت حرف *locale,
                      شمار xkb_compose_format format,
                      شمار xkb_compose_compile_flags flags)
{
    ثابت شمار xkb_compose_compile_flags invalid_flags = (
        flags & ~(شمار xkb_compose_compile_flags)XKB_COMPOSE_COMPILE_FLAGS_اقدار
    );
    اگر (invalid_flags) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "%s: unrecognized flags: %#x\n", func, invalid_flags);
        واپس تہی;
    }

    اگر (format != XKB_COMPOSE_FORMAT_متن_V1) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "%s: unsupported compose format: %d\n", func, format);
        واپس تہی;
    }

    حرف *resolved_locale;
    ساخت xkb_compose_جدول *جدول;
    ساخت compose_گرہ dummy = {0};

    resolved_locale = resolve_locale(ctx, locale);
    اگر (!resolved_locale)
        واپس تہی;

    جدول = صفر_حافظہ_دو(1, حجم(*جدول));
    اگر (!جدول) {
        آزاد(resolved_locale);
        واپس تہی;
    }

    جدول->refcnt = 1;
    جدول->ctx = xkb_context_ref(ctx);

    جدول->locale = resolved_locale;
    جدول->format = format;
    جدول->flags = flags;

    darray_init(جدول->nodes);
    darray_init(جدول->utf8);

    dummy.keysym = XKB_کلید_No_علامت;
    dummy.برگ.is_برگ = true;
    dummy.برگ.utf8 = 0;
    dummy.برگ.keysym = XKB_کلید_No_علامت;
    darray_append(جدول->nodes, dummy);

    darray_append(جدول->utf8, '\0');

    واپس جدول;
}

ساخت xkb_compose_جدول *
xkb_compose_جدول_ref(ساخت xkb_compose_جدول *جدول)
{
    أكد(جدول->refcnt > 0);
    جدول->refcnt++;
    واپس جدول;
}

خالی
xkb_compose_جدول_unref(ساخت xkb_compose_جدول *جدول)
{
    أكد(!جدول || جدول->refcnt > 0);
    اگر (!جدول || --جدول->refcnt > 0)
        واپس;
    آزاد(جدول->locale);
    darray_free(جدول->nodes);
    darray_free(جدول->utf8);
    xkb_context_unref(جدول->ctx);
    آزاد(جدول);
}

ساخت xkb_compose_جدول *
xkb_compose_جدول_new_from_file(ساخت xkb_context *ctx,
                                FILE *file,
                                ثابت حرف *locale,
                                شمار xkb_compose_format format,
                                شمار xkb_compose_compile_flags flags)
{
    ساخت xkb_compose_جدول * ثابت جدول =
        xkb_compose_جدول_new(ctx, __func__, locale, format, flags);
    اگر (!جدول)
        واپس تہی;

    اگر (!parse_file(جدول, file, "(unknown file)")) {
        xkb_compose_جدول_unref(جدول);
        واپس تہی;
    }

    واپس جدول;
}

ساخت xkb_compose_جدول *
xkb_compose_جدول_new_from_بفر(ساخت xkb_context *ctx,
                                  ثابت حرف *بفر, سائز_t لمبائی,
                                  ثابت حرف *locale,
                                  شمار xkb_compose_format format,
                                  شمار xkb_compose_compile_flags flags)
{
    ساخت xkb_compose_جدول * ثابت جدول =
        xkb_compose_جدول_new(ctx, __func__, locale, format, flags);
    اگر (!جدول)
        واپس تہی;

    اگر (!parse_string(جدول, بفر, لمبائی, "(input string)")) {
        xkb_compose_جدول_unref(جدول);
        واپس تہی;
    }

    واپس جدول;
}

ساخت xkb_compose_جدول *
xkb_compose_جدول_new_from_locale(ساخت xkb_context *ctx,
                                  ثابت حرف *locale,
                                  شمار xkb_compose_compile_flags flags)
{
    ساکن ثابت شمار xkb_compose_format format = XKB_COMPOSE_FORMAT_متن_V1;
    ساخت xkb_compose_جدول * ثابت جدول =
        xkb_compose_جدول_new(ctx, __func__, locale, format, flags);
    اگر (!جدول)
        واپس تہی;

    حرف *راستہ = get_xcomposefile_راستہ(ctx);
    FILE *file = open_file(راستہ);
    اگر (file)
        جاؤ ملا_راستہ;
    آزاد(راستہ);

    راستہ = get_xdg_xcompose_file_راستہ(ctx);
    file = open_file(راستہ);
    اگر (file)
        جاؤ ملا_راستہ;
    آزاد(راستہ);

    راستہ = get_home_xcompose_file_راستہ(ctx);
    file = open_file(راستہ);
    اگر (file)
        جاؤ ملا_راستہ;
    آزاد(راستہ);

    راستہ = get_locale_compose_file_راستہ(ctx, جدول->locale);
    file = open_file(راستہ);
    اگر (file)
        جاؤ ملا_راستہ;
    آزاد(راستہ);

    log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
            "couldn't find a Compose file for locale \"%s\" (mapped to \"%s\")\n",
            locale, جدول->locale);
    xkb_compose_جدول_unref(جدول);
    واپس تہی;

ملا_راستہ:
    {} /* Label followed by a declaration is a C23 extension */
    ثابت bool ok = parse_file(جدول, file, راستہ);
    fclose(file);
    اگر (!ok) {
        آزاد(راستہ);
        xkb_compose_جدول_unref(جدول);
        واپس تہی;
    }

    log_dbg(ctx, XKB_LOG_پیغام_NO_ID,
            "created compose table from locale %s with path %s\n",
            جدول->locale, راستہ);

    آزاد(راستہ);
    واپس جدول;
}

ثابت xkb_keysym_t *
xkb_compose_جدول_اندراج_sequence(ساخت xkb_compose_جدول_اندراج *اندراج_2,
                                 سائز_t *sequence_لمبائی)
{
    *sequence_لمبائی = اندراج_2->sequence_لمبائی;
    واپس اندراج_2->sequence;
}

xkb_keysym_t
xkb_compose_جدول_اندراج_keysym(ساخت xkb_compose_جدول_اندراج *اندراج_2)
{
    واپس اندراج_2->keysym;
}

ثابت حرف *
xkb_compose_جدول_اندراج_utf8(ساخت xkb_compose_جدول_اندراج *اندراج_2)
{
    واپس اندراج_2->utf8;
}

#اگر MAX_COMPOSE_NODES_LOG2 > 31
#غلطی "Cannot implement bit field xkb_compose_table_iterator_pending_node.offset"
#اگر_ختم

ساخت xkb_compose_جدول_iterator_pending_گرہ {
    uint32_t انحراف:31;
    bool processed:1;
};

ساخت xkb_compose_جدول_iterator {
    ساخت xkb_compose_جدول *جدول;
    /* Current entry */
    ساخت xkb_compose_جدول_اندراج اندراج_2;
    /* Stack of pending nodes to process */
    darray(ساخت xkb_compose_جدول_iterator_pending_گرہ) pending_nodes;
};

ساخت xkb_compose_جدول_iterator *
xkb_compose_جدول_iterator_new(ساخت xkb_compose_جدول *جدول)
{
    ساخت xkb_compose_جدول_iterator *iter;
    xkb_keysym_t *sequence;

    iter = صفر_حافظہ_دو(1, حجم(*iter));
    اگر (!iter) {
        واپس تہی;
    }
    iter->جدول = xkb_compose_جدول_ref(جدول);
    sequence = صفر_حافظہ_دو(COMPOSE_MAX_LHS_LEN, حجم(xkb_keysym_t));
    اگر (!sequence) {
        آزاد(iter);
        واپس تہی;
    }
    iter->اندراج_2.sequence = sequence;
    iter->اندراج_2.sequence_لمبائی = 0;

    darray_init(iter->pending_nodes);
    /* Short-circuit if table contains only the dummy entry */
    اگر (darray_سائز(جدول->nodes) == 1) {
        واپس iter;
    }
    /* Add root node */
    ساخت xkb_compose_جدول_iterator_pending_گرہ pending = {
        .انحراف = 1,
        .processed = false
    };
    darray_append(iter->pending_nodes, pending);
    ثابت ساخت compose_گرہ *گرہ = &darray_شے(iter->جدول->nodes,
                                                   pending.انحراف);

    /* Find the first left-most node and store intermediate nodes as pending */
    جب (گرہ->lokid) {
        pending.انحراف = گرہ->lokid;
        darray_append(iter->pending_nodes, pending);
        گرہ = &darray_شے(iter->جدول->nodes, pending.انحراف);
    };

    واپس iter;
}

خالی
xkb_compose_جدول_iterator_free(ساخت xkb_compose_جدول_iterator *iter)
{
    xkb_compose_جدول_unref(iter->جدول);
    darray_free(iter->pending_nodes);
    آزاد(iter->اندراج_2.sequence);
    آزاد(iter);
}

ساخت xkb_compose_جدول_اندراج *
xkb_compose_جدول_iterator_اگلا(ساخت xkb_compose_جدول_iterator *iter)
{
    /* Traversal algorithm (simplified):
     * 1. Resume last pending node from the stack as the current pending node.
     * 2. If the node is not yet processed, go to 5.
     * 3. Remove node from the stack and remove last keysym from the entry.
     * 4. If there is a right arrow, set it as the current pending node
     *    (unprocessed) and go to 6; else go to 1.
     * 5. Follow down arrow: set the pending node as processed, then:
     *      a) if it is a leaf, complete the entry and return it.
     *      b) else append the child node to the stack and set it as the current
     *         pending node.
     * 6. Find the next left-most arrow and store intermediate pending nodes.
     * 7. Go to 5.
     */

    ساخت xkb_compose_جدول_iterator_pending_گرہ *pending;
    ثابت ساخت compose_گرہ *گرہ;

    /* Iterator is empty if there is no pending nodes */
    اگر (unlikely(darray_خالی_خانے(iter->pending_nodes))) {
        واپس تہی;
    }

    /* Resume to the last element in the stack */
    pending = &darray_شے(iter->pending_nodes,
                           darray_سائز(iter->pending_nodes) - 1);
    گرہ = &darray_شے(iter->جدول->nodes, pending->انحراف);

    ساخت xkb_compose_جدول_iterator_pending_گرہ new = {
        .processed = false,
        .انحراف = 0
    };

    /* Remove processed leaves until an unprocessed right arrow or parent
     * is found, then update entry accordingly */
    جب (pending->processed) {
        /* Remove last keysym */
        iter->اندراج_2.sequence_لمبائی--;
        اگر (گرہ->hikid) {
            /* Follow right arrow: replace current pending node */
            pending->processed = false;
            pending->انحراف = گرہ->hikid;
            /* Process child’s left arrow */
            جاؤ گرہ_بایاں;
        } ورنہ {
            /* Remove processed node */
            darray_remove_آخری(iter->pending_nodes);
            اگر (unlikely(darray_خالی_خانے(iter->pending_nodes))) {
                /* No more nodes to process */
                واپس تہی;
            }
            /* Get parent node */
            pending = &darray_شے(iter->pending_nodes,
                                   darray_سائز(iter->pending_nodes) - 1);
            گرہ = &darray_شے(iter->جدول->nodes, pending->انحراف);
        }
    }

    جب (1) {
        /* Follow down arrow */
        pending->processed = true;
        iter->اندراج_2.sequence_لمبائی++;
        iter->اندراج_2.sequence[iter->اندراج_2.sequence_لمبائی - 1] = گرہ->keysym;
        اگر (گرہ->is_برگ) {
            /* Leaf: return entry */
            iter->اندراج_2.keysym = گرہ->برگ.keysym;
            iter->اندراج_2.utf8 = &darray_شے(iter->جدول->utf8, گرہ->برگ.utf8);
            واپس &iter->اندراج_2;
        }
        /* Not a leaf: process child node */
        new.انحراف = گرہ->اندرونی_2.eqkid;
        darray_append(iter->pending_nodes, new);
        pending = &darray_شے(iter->pending_nodes,
                               darray_سائز(iter->pending_nodes) - 1);
گرہ_بایاں:
        گرہ = &darray_شے(iter->جدول->nodes, pending->انحراف);
        /* Find the next left-most arrow and store intermediate pending nodes */
        جب (گرہ->lokid) {
            /* Follow left arrow */
            new.انحراف = گرہ->lokid;
            darray_append(iter->pending_nodes, new);
            pending = &darray_شے(iter->pending_nodes,
                                   darray_سائز(iter->pending_nodes) - 1);
            گرہ = &darray_شے(iter->جدول->nodes, new.انحراف);
        }
    }
}
