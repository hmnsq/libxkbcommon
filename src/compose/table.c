/*
 * Copyright © 2013,2021 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "features/enums.h"
#अन्तर्भाव "constants.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "parser.h"
#अन्तर्भाव "paths.h"
#अन्तर्भाव "table.h"
#अन्तर्भाव "utils.h"

स्थिर संरचना xkb_compose_table *
xkb_compose_table_new(संरचना xkb_context *ctx, स्थिरांक अक्षर *func,
                      स्थिरांक अक्षर *locale,
                      गणना xkb_compose_format format,
                      गणना xkb_compose_compile_flags flags)
{
    स्थिरांक गणना xkb_compose_compile_flags invalid_flags = (
        flags & ~(गणना xkb_compose_compile_flags)XKB_COMPOSE_COMPILE_FLAGS_मूल्ये
    );
    जर (invalid_flags) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unrecognized flags: %#x\n", func, invalid_flags);
        परत रिक्त;
    }

    जर (format != XKB_COMPOSE_FORMAT_TEXT_V1) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unsupported compose format: %d\n", func, format);
        परत रिक्त;
    }

    अक्षर *resolved_locale;
    संरचना xkb_compose_table *table;
    संरचना compose_गाठ dummy = {0};

    resolved_locale = resolve_locale(ctx, locale);
    जर (!resolved_locale)
        परत रिक्त;

    table = सुस्मृति(1, आकार(*table));
    जर (!table) {
        मुक्त(resolved_locale);
        परत रिक्त;
    }

    table->refcnt = 1;
    table->ctx = xkb_context_ref(ctx);

    table->locale = resolved_locale;
    table->format = format;
    table->flags = flags;

    darray_init(table->nodes);
    darray_init(table->utf8);

    dummy.keysym = XKB_किल्ली_No_चिन्ह;
    dummy.leaf.is_leaf = true;
    dummy.leaf.utf8 = 0;
    dummy.leaf.keysym = XKB_किल्ली_No_चिन्ह;
    darray_append(table->nodes, dummy);

    darray_append(table->utf8, '\0');

    परत table;
}

संरचना xkb_compose_table *
xkb_compose_table_ref(संरचना xkb_compose_table *table)
{
    प्रतिज्ञा(table->refcnt > 0);
    table->refcnt++;
    परत table;
}

शून्य
xkb_compose_table_unref(संरचना xkb_compose_table *table)
{
    प्रतिज्ञा(!table || table->refcnt > 0);
    जर (!table || --table->refcnt > 0)
        परत;
    मुक्त(table->locale);
    darray_free(table->nodes);
    darray_free(table->utf8);
    xkb_context_unref(table->ctx);
    मुक्त(table);
}

संरचना xkb_compose_table *
xkb_compose_table_new_from_file(संरचना xkb_context *ctx,
                                खाता *file,
                                स्थिरांक अक्षर *locale,
                                गणना xkb_compose_format format,
                                गणना xkb_compose_compile_flags flags)
{
    संरचना xkb_compose_table * स्थिरांक table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    जर (!table)
        परत रिक्त;

    जर (!parse_file(table, file, "(unknown file)")) {
        xkb_compose_table_unref(table);
        परत रिक्त;
    }

    परत table;
}

संरचना xkb_compose_table *
xkb_compose_table_new_from_buffer(संरचना xkb_context *ctx,
                                  स्थिरांक अक्षर *buffer, माप_प्रकार length,
                                  स्थिरांक अक्षर *locale,
                                  गणना xkb_compose_format format,
                                  गणना xkb_compose_compile_flags flags)
{
    संरचना xkb_compose_table * स्थिरांक table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    जर (!table)
        परत रिक्त;

    जर (!parse_string(table, buffer, length, "(input string)")) {
        xkb_compose_table_unref(table);
        परत रिक्त;
    }

    परत table;
}

संरचना xkb_compose_table *
xkb_compose_table_new_from_locale(संरचना xkb_context *ctx,
                                  स्थिरांक अक्षर *locale,
                                  गणना xkb_compose_compile_flags flags)
{
    स्थिर स्थिरांक गणना xkb_compose_format format = XKB_COMPOSE_FORMAT_TEXT_V1;
    संरचना xkb_compose_table * स्थिरांक table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    जर (!table)
        परत रिक्त;

    अक्षर *path = get_xcomposefile_path(ctx);
    खाता *file = open_file(path);
    जर (file)
        जा found_path;
    मुक्त(path);

    path = get_xdg_xcompose_file_path(ctx);
    file = open_file(path);
    जर (file)
        जा found_path;
    मुक्त(path);

    path = get_home_xcompose_file_path(ctx);
    file = open_file(path);
    जर (file)
        जा found_path;
    मुक्त(path);

    path = get_locale_compose_file_path(ctx, table->locale);
    file = open_file(path);
    जर (file)
        जा found_path;
    मुक्त(path);

    log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
            "couldn't find a Compose file for locale \"%s\" (mapped to \"%s\")\n",
            locale, table->locale);
    xkb_compose_table_unref(table);
    परत रिक्त;

found_path:
    {} /* Label followed by a declaration is a C23 extension */
    स्थिरांक bool ok = parse_file(table, file, path);
    ख_बंद(file);
    जर (!ok) {
        मुक्त(path);
        xkb_compose_table_unref(table);
        परत रिक्त;
    }

    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "created compose table from locale %s with path %s\n",
            table->locale, path);

    मुक्त(path);
    परत table;
}

स्थिरांक xkb_keysym_t *
xkb_compose_table_entry_sequence(संरचना xkb_compose_table_entry *entry,
                                 माप_प्रकार *sequence_length)
{
    *sequence_length = entry->sequence_length;
    परत entry->sequence;
}

xkb_keysym_t
xkb_compose_table_entry_keysym(संरचना xkb_compose_table_entry *entry)
{
    परत entry->keysym;
}

स्थिरांक अक्षर *
xkb_compose_table_entry_utf8(संरचना xkb_compose_table_entry *entry)
{
    परत entry->utf8;
}

#जर MAX_COMPOSE_NODES_LOG2 > 31
#error "Cannot implement bit field xkb_compose_table_iterator_pending_node.offset"
#endif

संरचना xkb_compose_table_iterator_pending_गाठ {
    uint32_t offset:31;
    bool processed:1;
};

संरचना xkb_compose_table_iterator {
    संरचना xkb_compose_table *table;
    /* Current entry */
    संरचना xkb_compose_table_entry entry;
    /* Stack of pending nodes to process */
    darray(संरचना xkb_compose_table_iterator_pending_गाठ) pending_nodes;
};

संरचना xkb_compose_table_iterator *
xkb_compose_table_iterator_new(संरचना xkb_compose_table *table)
{
    संरचना xkb_compose_table_iterator *iter;
    xkb_keysym_t *sequence;

    iter = सुस्मृति(1, आकार(*iter));
    जर (!iter) {
        परत रिक्त;
    }
    iter->table = xkb_compose_table_ref(table);
    sequence = सुस्मृति(COMPOSE_MAX_LHS_LEN, आकार(xkb_keysym_t));
    जर (!sequence) {
        मुक्त(iter);
        परत रिक्त;
    }
    iter->entry.sequence = sequence;
    iter->entry.sequence_length = 0;

    darray_init(iter->pending_nodes);
    /* Short-circuit if table contains only the dummy entry */
    जर (darray_size(table->nodes) == 1) {
        परत iter;
    }
    /* Add root node */
    संरचना xkb_compose_table_iterator_pending_गाठ pending = {
        .offset = 1,
        .processed = false
    };
    darray_append(iter->pending_nodes, pending);
    स्थिरांक संरचना compose_गाठ *गाठ = &darray_item(iter->table->nodes,
                                                   pending.offset);

    /* Find the first left-most node and store intermediate nodes as pending */
    पर्यंत (गाठ->lokid) {
        pending.offset = गाठ->lokid;
        darray_append(iter->pending_nodes, pending);
        गाठ = &darray_item(iter->table->nodes, pending.offset);
    };

    परत iter;
}

शून्य
xkb_compose_table_iterator_free(संरचना xkb_compose_table_iterator *iter)
{
    xkb_compose_table_unref(iter->table);
    darray_free(iter->pending_nodes);
    मुक्त(iter->entry.sequence);
    मुक्त(iter);
}

संरचना xkb_compose_table_entry *
xkb_compose_table_iterator_next(संरचना xkb_compose_table_iterator *iter)
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

    संरचना xkb_compose_table_iterator_pending_गाठ *pending;
    स्थिरांक संरचना compose_गाठ *गाठ;

    /* Iterator is empty if there is no pending nodes */
    जर (unlikely(darray_empty(iter->pending_nodes))) {
        परत रिक्त;
    }

    /* Resume to the last element in the stack */
    pending = &darray_item(iter->pending_nodes,
                           darray_size(iter->pending_nodes) - 1);
    गाठ = &darray_item(iter->table->nodes, pending->offset);

    संरचना xkb_compose_table_iterator_pending_गाठ new = {
        .processed = false,
        .offset = 0
    };

    /* Remove processed leaves until an unprocessed right arrow or parent
     * is found, then update entry accordingly */
    पर्यंत (pending->processed) {
        /* Remove last keysym */
        iter->entry.sequence_length--;
        जर (गाठ->hikid) {
            /* Follow right arrow: replace current pending node */
            pending->processed = false;
            pending->offset = गाठ->hikid;
            /* Process child’s left arrow */
            जा गाठ_डावा;
        } अन्यथा {
            /* Remove processed node */
            darray_remove_last(iter->pending_nodes);
            जर (unlikely(darray_empty(iter->pending_nodes))) {
                /* No more nodes to process */
                परत रिक्त;
            }
            /* Get parent node */
            pending = &darray_item(iter->pending_nodes,
                                   darray_size(iter->pending_nodes) - 1);
            गाठ = &darray_item(iter->table->nodes, pending->offset);
        }
    }

    पर्यंत (1) {
        /* Follow down arrow */
        pending->processed = true;
        iter->entry.sequence_length++;
        iter->entry.sequence[iter->entry.sequence_length - 1] = गाठ->keysym;
        जर (गाठ->is_leaf) {
            /* Leaf: return entry */
            iter->entry.keysym = गाठ->leaf.keysym;
            iter->entry.utf8 = &darray_item(iter->table->utf8, गाठ->leaf.utf8);
            परत &iter->entry;
        }
        /* Not a leaf: process child node */
        new.offset = गाठ->internal.eqkid;
        darray_append(iter->pending_nodes, new);
        pending = &darray_item(iter->pending_nodes,
                               darray_size(iter->pending_nodes) - 1);
गाठ_डावा:
        गाठ = &darray_item(iter->table->nodes, pending->offset);
        /* Find the next left-most arrow and store intermediate pending nodes */
        पर्यंत (गाठ->lokid) {
            /* Follow left arrow */
            new.offset = गाठ->lokid;
            darray_append(iter->pending_nodes, new);
            pending = &darray_item(iter->pending_nodes,
                                   darray_size(iter->pending_nodes) - 1);
            गाठ = &darray_item(iter->table->nodes, new.offset);
        }
    }
}
