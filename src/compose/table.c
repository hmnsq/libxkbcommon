/*
 * Copyright © 2013,2021 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن <assert.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "features/enums.h"
#ضمن "constants.h"
#ضمن "messages-codes.h"
#ضمن "parser.h"
#ضمن "paths.h"
#ضمن "table.h"
#ضمن "utils.h"

ساكن بنية xkb_compose_جدول *
xkb_compose_جدول_new(بنية xkb_context *ctx, ثابت حرف *func,
                      ثابت حرف *locale,
                      تعداد xkb_compose_format format,
                      تعداد xkb_compose_compile_flags flags)
{
    ثابت تعداد xkb_compose_compile_flags invalid_flags = (
        flags & ~(تعداد xkb_compose_compile_flags)XKB_COMPOSE_COMPILE_FLAGS_اقدار
    );
    إذا (invalid_flags) {
        log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                "%s: unrecognized flags: %#x\n", func, invalid_flags);
        أرجع عدم;
    }

    إذا (format != XKB_COMPOSE_FORMAT_متن_V1) {
        log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                "%s: unsupported compose format: %d\n", func, format);
        أرجع عدم;
    }

    حرف *resolved_locale;
    بنية xkb_compose_جدول *جدول;
    بنية compose_گرہ dummy = {0};

    resolved_locale = resolve_locale(ctx, locale);
    إذا (!resolved_locale)
        أرجع عدم;

    جدول = صفر_حافظہ_دیو(1, حجم(*جدول));
    إذا (!جدول) {
        حرر(resolved_locale);
        أرجع عدم;
    }

    جدول->refcnt = 1;
    جدول->ctx = xkb_context_ref(ctx);

    جدول->locale = resolved_locale;
    جدول->format = format;
    جدول->flags = flags;

    darray_init(جدول->nodes);
    darray_init(جدول->utf8);

    dummy.keysym = XKB_کلید_No_علامت;
    dummy.پتی.is_پتی = true;
    dummy.پتی.utf8 = 0;
    dummy.پتی.keysym = XKB_کلید_No_علامت;
    darray_append(جدول->nodes, dummy);

    darray_append(جدول->utf8, '\0');

    أرجع جدول;
}

بنية xkb_compose_جدول *
xkb_compose_جدول_ref(بنية xkb_compose_جدول *جدول)
{
    أكد(جدول->refcnt > 0);
    جدول->refcnt++;
    أرجع جدول;
}

فراغ
xkb_compose_جدول_unref(بنية xkb_compose_جدول *جدول)
{
    أكد(!جدول || جدول->refcnt > 0);
    إذا (!جدول || --جدول->refcnt > 0)
        أرجع;
    حرر(جدول->locale);
    darray_free(جدول->nodes);
    darray_free(جدول->utf8);
    xkb_context_unref(جدول->ctx);
    حرر(جدول);
}

بنية xkb_compose_جدول *
xkb_compose_جدول_new_from_file(بنية xkb_context *ctx,
                                FILE *file,
                                ثابت حرف *locale,
                                تعداد xkb_compose_format format,
                                تعداد xkb_compose_compile_flags flags)
{
    بنية xkb_compose_جدول * ثابت جدول =
        xkb_compose_جدول_new(ctx, __func__, locale, format, flags);
    إذا (!جدول)
        أرجع عدم;

    إذا (!parse_file(جدول, file, "(unknown file)")) {
        xkb_compose_جدول_unref(جدول);
        أرجع عدم;
    }

    أرجع جدول;
}

بنية xkb_compose_جدول *
xkb_compose_جدول_new_from_بفر(بنية xkb_context *ctx,
                                  ثابت حرف *بفر, ناپ_t لمائی,
                                  ثابت حرف *locale,
                                  تعداد xkb_compose_format format,
                                  تعداد xkb_compose_compile_flags flags)
{
    بنية xkb_compose_جدول * ثابت جدول =
        xkb_compose_جدول_new(ctx, __func__, locale, format, flags);
    إذا (!جدول)
        أرجع عدم;

    إذا (!parse_string(جدول, بفر, لمائی, "(input string)")) {
        xkb_compose_جدول_unref(جدول);
        أرجع عدم;
    }

    أرجع جدول;
}

بنية xkb_compose_جدول *
xkb_compose_جدول_new_from_locale(بنية xkb_context *ctx,
                                  ثابت حرف *locale,
                                  تعداد xkb_compose_compile_flags flags)
{
    ساكن ثابت تعداد xkb_compose_format format = XKB_COMPOSE_FORMAT_متن_V1;
    بنية xkb_compose_جدول * ثابت جدول =
        xkb_compose_جدول_new(ctx, __func__, locale, format, flags);
    إذا (!جدول)
        أرجع عدم;

    حرف *رستہ = get_xcomposefile_رستہ(ctx);
    FILE *file = open_file(رستہ);
    إذا (file)
        اذهب لبھیا_رستہ;
    حرر(رستہ);

    رستہ = get_xdg_xcompose_file_رستہ(ctx);
    file = open_file(رستہ);
    إذا (file)
        اذهب لبھیا_رستہ;
    حرر(رستہ);

    رستہ = get_home_xcompose_file_رستہ(ctx);
    file = open_file(رستہ);
    إذا (file)
        اذهب لبھیا_رستہ;
    حرر(رستہ);

    رستہ = get_locale_compose_file_رستہ(ctx, جدول->locale);
    file = open_file(رستہ);
    إذا (file)
        اذهب لبھیا_رستہ;
    حرر(رستہ);

    log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
            "couldn't find a Compose file for locale \"%s\" (mapped to \"%s\")\n",
            locale, جدول->locale);
    xkb_compose_جدول_unref(جدول);
    أرجع عدم;

لبھیا_رستہ:
    {} /* Label followed by a declaration is a C23 extension */
    ثابت bool ok = parse_file(جدول, file, رستہ);
    fclose(file);
    إذا (!ok) {
        حرر(رستہ);
        xkb_compose_جدول_unref(جدول);
        أرجع عدم;
    }

    log_dbg(ctx, XKB_LOG_سنیہا_NO_ID,
            "created compose table from locale %s with path %s\n",
            جدول->locale, رستہ);

    حرر(رستہ);
    أرجع جدول;
}

ثابت xkb_keysym_t *
xkb_compose_جدول_اندراج_sequence(بنية xkb_compose_جدول_اندراج *اندراج,
                                 ناپ_t *sequence_لمائی)
{
    *sequence_لمائی = اندراج->sequence_لمائی;
    أرجع اندراج->sequence;
}

xkb_keysym_t
xkb_compose_جدول_اندراج_keysym(بنية xkb_compose_جدول_اندراج *اندراج)
{
    أرجع اندراج->keysym;
}

ثابت حرف *
xkb_compose_جدول_اندراج_utf8(بنية xkb_compose_جدول_اندراج *اندراج)
{
    أرجع اندراج->utf8;
}

#إذا MAX_COMPOSE_NODES_LOG2 > 31
#غلطی "Cannot implement bit field xkb_compose_table_iterator_pending_node.offset"
#جے_انت

بنية xkb_compose_جدول_iterator_pending_گرہ {
    uint32_t وتھ:31;
    bool processed:1;
};

بنية xkb_compose_جدول_iterator {
    بنية xkb_compose_جدول *جدول;
    /* Current entry */
    بنية xkb_compose_جدول_اندراج اندراج;
    /* Stack of pending nodes to process */
    darray(بنية xkb_compose_جدول_iterator_pending_گرہ) pending_nodes;
};

بنية xkb_compose_جدول_iterator *
xkb_compose_جدول_iterator_new(بنية xkb_compose_جدول *جدول)
{
    بنية xkb_compose_جدول_iterator *iter;
    xkb_keysym_t *sequence;

    iter = صفر_حافظہ_دیو(1, حجم(*iter));
    إذا (!iter) {
        أرجع عدم;
    }
    iter->جدول = xkb_compose_جدول_ref(جدول);
    sequence = صفر_حافظہ_دیو(COMPOSE_MAX_LHS_LEN, حجم(xkb_keysym_t));
    إذا (!sequence) {
        حرر(iter);
        أرجع عدم;
    }
    iter->اندراج.sequence = sequence;
    iter->اندراج.sequence_لمائی = 0;

    darray_init(iter->pending_nodes);
    /* Short-circuit if table contains only the dummy entry */
    إذا (darray_ناپ(جدول->nodes) == 1) {
        أرجع iter;
    }
    /* Add root node */
    بنية xkb_compose_جدول_iterator_pending_گرہ pending = {
        .وتھ = 1,
        .processed = false
    };
    darray_append(iter->pending_nodes, pending);
    ثابت بنية compose_گرہ *گرہ = &darray_شے(iter->جدول->nodes,
                                                   pending.وتھ);

    /* Find the first left-most node and store intermediate nodes as pending */
    طالما (گرہ->lokid) {
        pending.وتھ = گرہ->lokid;
        darray_append(iter->pending_nodes, pending);
        گرہ = &darray_شے(iter->جدول->nodes, pending.وتھ);
    };

    أرجع iter;
}

فراغ
xkb_compose_جدول_iterator_free(بنية xkb_compose_جدول_iterator *iter)
{
    xkb_compose_جدول_unref(iter->جدول);
    darray_free(iter->pending_nodes);
    حرر(iter->اندراج.sequence);
    حرر(iter);
}

بنية xkb_compose_جدول_اندراج *
xkb_compose_جدول_iterator_اگلا(بنية xkb_compose_جدول_iterator *iter)
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

    بنية xkb_compose_جدول_iterator_pending_گرہ *pending;
    ثابت بنية compose_گرہ *گرہ;

    /* Iterator is empty if there is no pending nodes */
    إذا (unlikely(darray_خالی_تھاواں(iter->pending_nodes))) {
        أرجع عدم;
    }

    /* Resume to the last element in the stack */
    pending = &darray_شے(iter->pending_nodes,
                           darray_ناپ(iter->pending_nodes) - 1);
    گرہ = &darray_شے(iter->جدول->nodes, pending->وتھ);

    بنية xkb_compose_جدول_iterator_pending_گرہ new = {
        .processed = false,
        .وتھ = 0
    };

    /* Remove processed leaves until an unprocessed right arrow or parent
     * is found, then update entry accordingly */
    طالما (pending->processed) {
        /* Remove last keysym */
        iter->اندراج.sequence_لمائی--;
        إذا (گرہ->hikid) {
            /* Follow right arrow: replace current pending node */
            pending->processed = false;
            pending->وتھ = گرہ->hikid;
            /* Process child’s left arrow */
            اذهب گرہ_بایاں;
        } وإلا {
            /* Remove processed node */
            darray_remove_آخری(iter->pending_nodes);
            إذا (unlikely(darray_خالی_تھاواں(iter->pending_nodes))) {
                /* No more nodes to process */
                أرجع عدم;
            }
            /* Get parent node */
            pending = &darray_شے(iter->pending_nodes,
                                   darray_ناپ(iter->pending_nodes) - 1);
            گرہ = &darray_شے(iter->جدول->nodes, pending->وتھ);
        }
    }

    طالما (1) {
        /* Follow down arrow */
        pending->processed = true;
        iter->اندراج.sequence_لمائی++;
        iter->اندراج.sequence[iter->اندراج.sequence_لمائی - 1] = گرہ->keysym;
        إذا (گرہ->is_پتی) {
            /* Leaf: return entry */
            iter->اندراج.keysym = گرہ->پتی.keysym;
            iter->اندراج.utf8 = &darray_شے(iter->جدول->utf8, گرہ->پتی.utf8);
            أرجع &iter->اندراج;
        }
        /* Not a leaf: process child node */
        new.وتھ = گرہ->اندرونی.eqkid;
        darray_append(iter->pending_nodes, new);
        pending = &darray_شے(iter->pending_nodes,
                               darray_ناپ(iter->pending_nodes) - 1);
گرہ_بایاں:
        گرہ = &darray_شے(iter->جدول->nodes, pending->وتھ);
        /* Find the next left-most arrow and store intermediate pending nodes */
        طالما (گرہ->lokid) {
            /* Follow left arrow */
            new.وتھ = گرہ->lokid;
            darray_append(iter->pending_nodes, new);
            pending = &darray_شے(iter->pending_nodes,
                                   darray_ناپ(iter->pending_nodes) - 1);
            گرہ = &darray_شے(iter->جدول->nodes, new.وتھ);
        }
    }
}
