/*
 * Copyright © 2013,2021 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "features/enums.h"
#അന്തര്ഭാവ "constants.h"
#അന്തര്ഭാവ "messages-codes.h"
#അന്തര്ഭാവ "parser.h"
#അന്തര്ഭാവ "paths.h"
#അന്തര്ഭാവ "table.h"
#അന്തര്ഭാവ "utils.h"

സ്ഥിതം ഘടന xkb_compose_table *
xkb_compose_table_new(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം *func,
                      സ്ഥിരം അക്ഷരം *locale,
                      എണ്ണൽ xkb_compose_format format,
                      എണ്ണൽ xkb_compose_compile_flags flags)
{
    സ്ഥിരം എണ്ണൽ xkb_compose_compile_flags invalid_flags = (
        flags & ~(എണ്ണൽ xkb_compose_compile_flags)XKB_COMPOSE_COMPILE_FLAGS_മൂല്യങ്ങൾ
    );
    എങ്കിൽ (invalid_flags) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unrecognized flags: %#x\n", func, invalid_flags);
        തിരികെ ശൂന്യ;
    }

    എങ്കിൽ (format != XKB_COMPOSE_FORMAT_TEXT_V1) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unsupported compose format: %d\n", func, format);
        തിരികെ ശൂന്യ;
    }

    അക്ഷരം *resolved_locale;
    ഘടന xkb_compose_table *table;
    ഘടന compose_കണ്ണി dummy = {0};

    resolved_locale = resolve_locale(ctx, locale);
    എങ്കിൽ (!resolved_locale)
        തിരികെ ശൂന്യ;

    table = calloc(1, വലുപ്പം(*table));
    എങ്കിൽ (!table) {
        സ്വതന്ത്രം(resolved_locale);
        തിരികെ ശൂന്യ;
    }

    table->refcnt = 1;
    table->ctx = xkb_context_ref(ctx);

    table->locale = resolved_locale;
    table->format = format;
    table->flags = flags;

    darray_init(table->nodes);
    darray_init(table->utf8);

    dummy.keysym = XKB_താക്കോൽ_No_ചിഹ്നം;
    dummy.leaf.is_leaf = true;
    dummy.leaf.utf8 = 0;
    dummy.leaf.keysym = XKB_താക്കോൽ_No_ചിഹ്നം;
    darray_append(table->nodes, dummy);

    darray_append(table->utf8, '\0');

    തിരികെ table;
}

ഘടന xkb_compose_table *
xkb_compose_table_ref(ഘടന xkb_compose_table *table)
{
    പ്രതിജ്ഞാ(table->refcnt > 0);
    table->refcnt++;
    തിരികെ table;
}

ശൂന്യം
xkb_compose_table_unref(ഘടന xkb_compose_table *table)
{
    പ്രതിജ്ഞാ(!table || table->refcnt > 0);
    എങ്കിൽ (!table || --table->refcnt > 0)
        തിരികെ;
    സ്വതന്ത്രം(table->locale);
    darray_free(table->nodes);
    darray_free(table->utf8);
    xkb_context_unref(table->ctx);
    സ്വതന്ത്രം(table);
}

ഘടന xkb_compose_table *
xkb_compose_table_new_from_file(ഘടന xkb_context *ctx,
                                FILE *file,
                                സ്ഥിരം അക്ഷരം *locale,
                                എണ്ണൽ xkb_compose_format format,
                                എണ്ണൽ xkb_compose_compile_flags flags)
{
    ഘടന xkb_compose_table * സ്ഥിരം table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    എങ്കിൽ (!table)
        തിരികെ ശൂന്യ;

    എങ്കിൽ (!parse_file(table, file, "(unknown file)")) {
        xkb_compose_table_unref(table);
        തിരികെ ശൂന്യ;
    }

    തിരികെ table;
}

ഘടന xkb_compose_table *
xkb_compose_table_new_from_buffer(ഘടന xkb_context *ctx,
                                  സ്ഥിരം അക്ഷരം *buffer, size_t length,
                                  സ്ഥിരം അക്ഷരം *locale,
                                  എണ്ണൽ xkb_compose_format format,
                                  എണ്ണൽ xkb_compose_compile_flags flags)
{
    ഘടന xkb_compose_table * സ്ഥിരം table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    എങ്കിൽ (!table)
        തിരികെ ശൂന്യ;

    എങ്കിൽ (!parse_string(table, buffer, length, "(input string)")) {
        xkb_compose_table_unref(table);
        തിരികെ ശൂന്യ;
    }

    തിരികെ table;
}

ഘടന xkb_compose_table *
xkb_compose_table_new_from_locale(ഘടന xkb_context *ctx,
                                  സ്ഥിരം അക്ഷരം *locale,
                                  എണ്ണൽ xkb_compose_compile_flags flags)
{
    സ്ഥിതം സ്ഥിരം എണ്ണൽ xkb_compose_format format = XKB_COMPOSE_FORMAT_TEXT_V1;
    ഘടന xkb_compose_table * സ്ഥിരം table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    എങ്കിൽ (!table)
        തിരികെ ശൂന്യ;

    അക്ഷരം *path = get_xcomposefile_path(ctx);
    FILE *file = open_file(path);
    എങ്കിൽ (file)
        പോകുക found_path;
    സ്വതന്ത്രം(path);

    path = get_xdg_xcompose_file_path(ctx);
    file = open_file(path);
    എങ്കിൽ (file)
        പോകുക found_path;
    സ്വതന്ത്രം(path);

    path = get_home_xcompose_file_path(ctx);
    file = open_file(path);
    എങ്കിൽ (file)
        പോകുക found_path;
    സ്വതന്ത്രം(path);

    path = get_locale_compose_file_path(ctx, table->locale);
    file = open_file(path);
    എങ്കിൽ (file)
        പോകുക found_path;
    സ്വതന്ത്രം(path);

    log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
            "couldn't find a Compose file for locale \"%s\" (mapped to \"%s\")\n",
            locale, table->locale);
    xkb_compose_table_unref(table);
    തിരികെ ശൂന്യ;

found_path:
    {} /* Label followed by a declaration is a C23 extension */
    സ്ഥിരം bool ok = parse_file(table, file, path);
    fclose(file);
    എങ്കിൽ (!ok) {
        സ്വതന്ത്രം(path);
        xkb_compose_table_unref(table);
        തിരികെ ശൂന്യ;
    }

    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "created compose table from locale %s with path %s\n",
            table->locale, path);

    സ്വതന്ത്രം(path);
    തിരികെ table;
}

സ്ഥിരം xkb_keysym_t *
xkb_compose_table_entry_sequence(ഘടന xkb_compose_table_entry *entry,
                                 size_t *sequence_length)
{
    *sequence_length = entry->sequence_length;
    തിരികെ entry->sequence;
}

xkb_keysym_t
xkb_compose_table_entry_keysym(ഘടന xkb_compose_table_entry *entry)
{
    തിരികെ entry->keysym;
}

സ്ഥിരം അക്ഷരം *
xkb_compose_table_entry_utf8(ഘടന xkb_compose_table_entry *entry)
{
    തിരികെ entry->utf8;
}

#എങ്കിൽ MAX_COMPOSE_NODES_LOG2 > 31
#error "Cannot implement bit field xkb_compose_table_iterator_pending_node.offset"
#endif

ഘടന xkb_compose_table_iterator_pending_കണ്ണി {
    uint32_t offset:31;
    bool processed:1;
};

ഘടന xkb_compose_table_iterator {
    ഘടന xkb_compose_table *table;
    /* Current entry */
    ഘടന xkb_compose_table_entry entry;
    /* Stack of pending nodes to process */
    darray(ഘടന xkb_compose_table_iterator_pending_കണ്ണി) pending_nodes;
};

ഘടന xkb_compose_table_iterator *
xkb_compose_table_iterator_new(ഘടന xkb_compose_table *table)
{
    ഘടന xkb_compose_table_iterator *iter;
    xkb_keysym_t *sequence;

    iter = calloc(1, വലുപ്പം(*iter));
    എങ്കിൽ (!iter) {
        തിരികെ ശൂന്യ;
    }
    iter->table = xkb_compose_table_ref(table);
    sequence = calloc(COMPOSE_MAX_LHS_LEN, വലുപ്പം(xkb_keysym_t));
    എങ്കിൽ (!sequence) {
        സ്വതന്ത്രം(iter);
        തിരികെ ശൂന്യ;
    }
    iter->entry.sequence = sequence;
    iter->entry.sequence_length = 0;

    darray_init(iter->pending_nodes);
    /* Short-circuit if table contains only the dummy entry */
    എങ്കിൽ (darray_size(table->nodes) == 1) {
        തിരികെ iter;
    }
    /* Add root node */
    ഘടന xkb_compose_table_iterator_pending_കണ്ണി pending = {
        .offset = 1,
        .processed = false
    };
    darray_append(iter->pending_nodes, pending);
    സ്ഥിരം ഘടന compose_കണ്ണി *കണ്ണി = &darray_item(iter->table->nodes,
                                                   pending.offset);

    /* Find the first left-most node and store intermediate nodes as pending */
    ആകുമ്പോൾ (കണ്ണി->lokid) {
        pending.offset = കണ്ണി->lokid;
        darray_append(iter->pending_nodes, pending);
        കണ്ണി = &darray_item(iter->table->nodes, pending.offset);
    };

    തിരികെ iter;
}

ശൂന്യം
xkb_compose_table_iterator_free(ഘടന xkb_compose_table_iterator *iter)
{
    xkb_compose_table_unref(iter->table);
    darray_free(iter->pending_nodes);
    സ്വതന്ത്രം(iter->entry.sequence);
    സ്വതന്ത്രം(iter);
}

ഘടന xkb_compose_table_entry *
xkb_compose_table_iterator_next(ഘടന xkb_compose_table_iterator *iter)
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

    ഘടന xkb_compose_table_iterator_pending_കണ്ണി *pending;
    സ്ഥിരം ഘടന compose_കണ്ണി *കണ്ണി;

    /* Iterator is empty if there is no pending nodes */
    എങ്കിൽ (unlikely(darray_empty(iter->pending_nodes))) {
        തിരികെ ശൂന്യ;
    }

    /* Resume to the last element in the stack */
    pending = &darray_item(iter->pending_nodes,
                           darray_size(iter->pending_nodes) - 1);
    കണ്ണി = &darray_item(iter->table->nodes, pending->offset);

    ഘടന xkb_compose_table_iterator_pending_കണ്ണി new = {
        .processed = false,
        .offset = 0
    };

    /* Remove processed leaves until an unprocessed right arrow or parent
     * is found, then update entry accordingly */
    ആകുമ്പോൾ (pending->processed) {
        /* Remove last keysym */
        iter->entry.sequence_length--;
        എങ്കിൽ (കണ്ണി->hikid) {
            /* Follow right arrow: replace current pending node */
            pending->processed = false;
            pending->offset = കണ്ണി->hikid;
            /* Process child’s left arrow */
            പോകുക കണ്ണി_ഇടത്;
        } അല്ലെങ്കിൽ {
            /* Remove processed node */
            darray_remove_last(iter->pending_nodes);
            എങ്കിൽ (unlikely(darray_empty(iter->pending_nodes))) {
                /* No more nodes to process */
                തിരികെ ശൂന്യ;
            }
            /* Get parent node */
            pending = &darray_item(iter->pending_nodes,
                                   darray_size(iter->pending_nodes) - 1);
            കണ്ണി = &darray_item(iter->table->nodes, pending->offset);
        }
    }

    ആകുമ്പോൾ (1) {
        /* Follow down arrow */
        pending->processed = true;
        iter->entry.sequence_length++;
        iter->entry.sequence[iter->entry.sequence_length - 1] = കണ്ണി->keysym;
        എങ്കിൽ (കണ്ണി->is_leaf) {
            /* Leaf: return entry */
            iter->entry.keysym = കണ്ണി->leaf.keysym;
            iter->entry.utf8 = &darray_item(iter->table->utf8, കണ്ണി->leaf.utf8);
            തിരികെ &iter->entry;
        }
        /* Not a leaf: process child node */
        new.offset = കണ്ണി->internal.eqkid;
        darray_append(iter->pending_nodes, new);
        pending = &darray_item(iter->pending_nodes,
                               darray_size(iter->pending_nodes) - 1);
കണ്ണി_ഇടത്:
        കണ്ണി = &darray_item(iter->table->nodes, pending->offset);
        /* Find the next left-most arrow and store intermediate pending nodes */
        ആകുമ്പോൾ (കണ്ണി->lokid) {
            /* Follow left arrow */
            new.offset = കണ്ണി->lokid;
            darray_append(iter->pending_nodes, new);
            pending = &darray_item(iter->pending_nodes,
                                   darray_size(iter->pending_nodes) - 1);
            കണ്ണി = &darray_item(iter->table->nodes, new.offset);
        }
    }
}
