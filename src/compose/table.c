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
        flags & ~(تعداد xkb_compose_compile_flags)XKB_COMPOSE_COMPILE_FLAGS_مقادیر
    );
    إذا (invalid_flags) {
        log_err(ctx, XKB_LOG_پیام_NO_ID,
                "%s: unrecognized flags: %#x\n", func, invalid_flags);
        أرجع عدم;
    }

    إذا (format != XKB_COMPOSE_FORMAT_متن_V1) {
        log_err(ctx, XKB_LOG_پیام_NO_ID,
                "%s: unsupported compose format: %d\n", func, format);
        أرجع عدم;
    }

    حرف *resolved_locale;
    بنية xkb_compose_جدول *جدول;
    بنية compose_گره dummy = {0};

    resolved_locale = resolve_locale(ctx, locale);
    إذا (!resolved_locale)
        أرجع عدم;

    جدول = حافظه_صفر_بده(1, حجم(*جدول));
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

    dummy.keysym = XKB_کلید_No_نماد;
    dummy.برگ.is_برگ = true;
    dummy.برگ.utf8 = 0;
    dummy.برگ.keysym = XKB_کلید_No_نماد;
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
xkb_compose_جدول_new_from_میانگیر(بنية xkb_context *ctx,
                                  ثابت حرف *میانگیر, اندازه_t درازی,
                                  ثابت حرف *locale,
                                  تعداد xkb_compose_format format,
                                  تعداد xkb_compose_compile_flags flags)
{
    بنية xkb_compose_جدول * ثابت جدول =
        xkb_compose_جدول_new(ctx, __func__, locale, format, flags);
    إذا (!جدول)
        أرجع عدم;

    إذا (!parse_string(جدول, میانگیر, درازی, "(input string)")) {
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

    حرف *مسیر = get_xcomposefile_مسیر(ctx);
    FILE *file = open_file(مسیر);
    إذا (file)
        اذهب یافت_مسیر;
    حرر(مسیر);

    مسیر = get_xdg_xcompose_file_مسیر(ctx);
    file = open_file(مسیر);
    إذا (file)
        اذهب یافت_مسیر;
    حرر(مسیر);

    مسیر = get_home_xcompose_file_مسیر(ctx);
    file = open_file(مسیر);
    إذا (file)
        اذهب یافت_مسیر;
    حرر(مسیر);

    مسیر = get_locale_compose_file_مسیر(ctx, جدول->locale);
    file = open_file(مسیر);
    إذا (file)
        اذهب یافت_مسیر;
    حرر(مسیر);

    log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
            "couldn't find a Compose file for locale \"%s\" (mapped to \"%s\")\n",
            locale, جدول->locale);
    xkb_compose_جدول_unref(جدول);
    أرجع عدم;

یافت_مسیر:
    {} /* Label followed by a declaration is a C23 extension */
    ثابت bool ok = parse_file(جدول, file, مسیر);
    fclose(file);
    إذا (!ok) {
        حرر(مسیر);
        xkb_compose_جدول_unref(جدول);
        أرجع عدم;
    }

    log_dbg(ctx, XKB_LOG_پیام_NO_ID,
            "created compose table from locale %s with path %s\n",
            جدول->locale, مسیر);

    حرر(مسیر);
    أرجع جدول;
}

ثابت xkb_keysym_t *
xkb_compose_جدول_مدخل_sequence(بنية xkb_compose_جدول_مدخل *مدخل,
                                 اندازه_t *sequence_درازی)
{
    *sequence_درازی = مدخل->sequence_درازی;
    أرجع مدخل->sequence;
}

xkb_keysym_t
xkb_compose_جدول_مدخل_keysym(بنية xkb_compose_جدول_مدخل *مدخل)
{
    أرجع مدخل->keysym;
}

ثابت حرف *
xkb_compose_جدول_مدخل_utf8(بنية xkb_compose_جدول_مدخل *مدخل)
{
    أرجع مدخل->utf8;
}

#إذا MAX_COMPOSE_NODES_LOG2 > 31
#خطا "Cannot implement bit field xkb_compose_table_iterator_pending_node.offset"
#پایان_اگر

بنية xkb_compose_جدول_iterator_pending_گره {
    uint32_t جابه_جایی:31;
    bool processed:1;
};

بنية xkb_compose_جدول_iterator {
    بنية xkb_compose_جدول *جدول;
    /* Current entry */
    بنية xkb_compose_جدول_مدخل مدخل;
    /* Stack of pending nodes to process */
    darray(بنية xkb_compose_جدول_iterator_pending_گره) pending_nodes;
};

بنية xkb_compose_جدول_iterator *
xkb_compose_جدول_iterator_new(بنية xkb_compose_جدول *جدول)
{
    بنية xkb_compose_جدول_iterator *iter;
    xkb_keysym_t *sequence;

    iter = حافظه_صفر_بده(1, حجم(*iter));
    إذا (!iter) {
        أرجع عدم;
    }
    iter->جدول = xkb_compose_جدول_ref(جدول);
    sequence = حافظه_صفر_بده(COMPOSE_MAX_LHS_LEN, حجم(xkb_keysym_t));
    إذا (!sequence) {
        حرر(iter);
        أرجع عدم;
    }
    iter->مدخل.sequence = sequence;
    iter->مدخل.sequence_درازی = 0;

    darray_init(iter->pending_nodes);
    /* Short-circuit if table contains only the dummy entry */
    إذا (darray_اندازه(جدول->nodes) == 1) {
        أرجع iter;
    }
    /* Add root node */
    بنية xkb_compose_جدول_iterator_pending_گره pending = {
        .جابه_جایی = 1,
        .processed = false
    };
    darray_append(iter->pending_nodes, pending);
    ثابت بنية compose_گره *گره = &darray_قلم(iter->جدول->nodes,
                                                   pending.جابه_جایی);

    /* Find the first left-most node and store intermediate nodes as pending */
    طالما (گره->lokid) {
        pending.جابه_جایی = گره->lokid;
        darray_append(iter->pending_nodes, pending);
        گره = &darray_قلم(iter->جدول->nodes, pending.جابه_جایی);
    };

    أرجع iter;
}

فراغ
xkb_compose_جدول_iterator_free(بنية xkb_compose_جدول_iterator *iter)
{
    xkb_compose_جدول_unref(iter->جدول);
    darray_free(iter->pending_nodes);
    حرر(iter->مدخل.sequence);
    حرر(iter);
}

بنية xkb_compose_جدول_مدخل *
xkb_compose_جدول_iterator_بعدی(بنية xkb_compose_جدول_iterator *iter)
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

    بنية xkb_compose_جدول_iterator_pending_گره *pending;
    ثابت بنية compose_گره *گره;

    /* Iterator is empty if there is no pending nodes */
    إذا (unlikely(darray_خالی(iter->pending_nodes))) {
        أرجع عدم;
    }

    /* Resume to the last element in the stack */
    pending = &darray_قلم(iter->pending_nodes,
                           darray_اندازه(iter->pending_nodes) - 1);
    گره = &darray_قلم(iter->جدول->nodes, pending->جابه_جایی);

    بنية xkb_compose_جدول_iterator_pending_گره new = {
        .processed = false,
        .جابه_جایی = 0
    };

    /* Remove processed leaves until an unprocessed right arrow or parent
     * is found, then update entry accordingly */
    طالما (pending->processed) {
        /* Remove last keysym */
        iter->مدخل.sequence_درازی--;
        إذا (گره->hikid) {
            /* Follow right arrow: replace current pending node */
            pending->processed = false;
            pending->جابه_جایی = گره->hikid;
            /* Process child’s left arrow */
            اذهب گره_چپ;
        } وإلا {
            /* Remove processed node */
            darray_remove_آخرین(iter->pending_nodes);
            إذا (unlikely(darray_خالی(iter->pending_nodes))) {
                /* No more nodes to process */
                أرجع عدم;
            }
            /* Get parent node */
            pending = &darray_قلم(iter->pending_nodes,
                                   darray_اندازه(iter->pending_nodes) - 1);
            گره = &darray_قلم(iter->جدول->nodes, pending->جابه_جایی);
        }
    }

    طالما (1) {
        /* Follow down arrow */
        pending->processed = true;
        iter->مدخل.sequence_درازی++;
        iter->مدخل.sequence[iter->مدخل.sequence_درازی - 1] = گره->keysym;
        إذا (گره->is_برگ) {
            /* Leaf: return entry */
            iter->مدخل.keysym = گره->برگ.keysym;
            iter->مدخل.utf8 = &darray_قلم(iter->جدول->utf8, گره->برگ.utf8);
            أرجع &iter->مدخل;
        }
        /* Not a leaf: process child node */
        new.جابه_جایی = گره->داخلی.eqkid;
        darray_append(iter->pending_nodes, new);
        pending = &darray_قلم(iter->pending_nodes,
                               darray_اندازه(iter->pending_nodes) - 1);
گره_چپ:
        گره = &darray_قلم(iter->جدول->nodes, pending->جابه_جایی);
        /* Find the next left-most arrow and store intermediate pending nodes */
        طالما (گره->lokid) {
            /* Follow left arrow */
            new.جابه_جایی = گره->lokid;
            darray_append(iter->pending_nodes, new);
            pending = &darray_قلم(iter->pending_nodes,
                                   darray_اندازه(iter->pending_nodes) - 1);
            گره = &darray_قلم(iter->جدول->nodes, new.جابه_جایی);
        }
    }
}
