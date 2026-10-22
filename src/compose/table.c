/*
 * Copyright © 2013,2021 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#समावेश "config.h"

#समावेश <assert.h>

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "features/enums.h"
#समावेश "constants.h"
#समावेश "messages-codes.h"
#समावेश "parser.h"
#समावेश "paths.h"
#समावेश "table.h"
#समावेश "utils.h"

अटल काष्ठा xkb_compose_table *
xkb_compose_table_new(काष्ठा xkb_context *ctx, स्थिर अक्षर *func,
                      स्थिर अक्षर *locale,
                      क्रमागत xkb_compose_format format,
                      क्रमागत xkb_compose_compile_flags flags)
{
    स्थिर क्रमागत xkb_compose_compile_flags invalid_flags = (
        flags & ~(क्रमागत xkb_compose_compile_flags)XKB_COMPOSE_COMPILE_FLAGS_मूल्यावली
    );
    अगर (invalid_flags) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unrecognized flags: %#x\n", func, invalid_flags);
        वापस शून्य;
    }

    अगर (format != XKB_COMPOSE_FORMAT_TEXT_V1) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unsupported compose format: %d\n", func, format);
        वापस शून्य;
    }

    अक्षर *resolved_locale;
    काष्ठा xkb_compose_table *table;
    काष्ठा compose_गाँठ dummy = {0};

    resolved_locale = resolve_locale(ctx, locale);
    अगर (!resolved_locale)
        वापस शून्य;

    table = सुस्मृति(1, माप(*table));
    अगर (!table) {
        मुक्त(resolved_locale);
        वापस शून्य;
    }

    table->refcnt = 1;
    table->ctx = xkb_context_ref(ctx);

    table->locale = resolved_locale;
    table->format = format;
    table->flags = flags;

    darray_init(table->nodes);
    darray_init(table->utf8);

    dummy.keysym = XKB_कुंजी_No_प्रतीक;
    dummy.leaf.is_leaf = true;
    dummy.leaf.utf8 = 0;
    dummy.leaf.keysym = XKB_कुंजी_No_प्रतीक;
    darray_append(table->nodes, dummy);

    darray_append(table->utf8, '\0');

    वापस table;
}

काष्ठा xkb_compose_table *
xkb_compose_table_ref(काष्ठा xkb_compose_table *table)
{
    निश्चित(table->refcnt > 0);
    table->refcnt++;
    वापस table;
}

व्योम
xkb_compose_table_unref(काष्ठा xkb_compose_table *table)
{
    निश्चित(!table || table->refcnt > 0);
    अगर (!table || --table->refcnt > 0)
        वापस;
    मुक्त(table->locale);
    darray_free(table->nodes);
    darray_free(table->utf8);
    xkb_context_unref(table->ctx);
    मुक्त(table);
}

काष्ठा xkb_compose_table *
xkb_compose_table_new_from_file(काष्ठा xkb_context *ctx,
                                खाता *file,
                                स्थिर अक्षर *locale,
                                क्रमागत xkb_compose_format format,
                                क्रमागत xkb_compose_compile_flags flags)
{
    काष्ठा xkb_compose_table * स्थिर table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    अगर (!table)
        वापस शून्य;

    अगर (!parse_file(table, file, "(unknown file)")) {
        xkb_compose_table_unref(table);
        वापस शून्य;
    }

    वापस table;
}

काष्ठा xkb_compose_table *
xkb_compose_table_new_from_बफर(काष्ठा xkb_context *ctx,
                                  स्थिर अक्षर *बफर, माप_प्रकार लंबाई,
                                  स्थिर अक्षर *locale,
                                  क्रमागत xkb_compose_format format,
                                  क्रमागत xkb_compose_compile_flags flags)
{
    काष्ठा xkb_compose_table * स्थिर table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    अगर (!table)
        वापस शून्य;

    अगर (!parse_string(table, बफर, लंबाई, "(input string)")) {
        xkb_compose_table_unref(table);
        वापस शून्य;
    }

    वापस table;
}

काष्ठा xkb_compose_table *
xkb_compose_table_new_from_locale(काष्ठा xkb_context *ctx,
                                  स्थिर अक्षर *locale,
                                  क्रमागत xkb_compose_compile_flags flags)
{
    अटल स्थिर क्रमागत xkb_compose_format format = XKB_COMPOSE_FORMAT_TEXT_V1;
    काष्ठा xkb_compose_table * स्थिर table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    अगर (!table)
        वापस शून्य;

    अक्षर *path = get_xcomposefile_path(ctx);
    खाता *file = open_file(path);
    अगर (file)
        जाओ found_path;
    मुक्त(path);

    path = get_xdg_xcompose_file_path(ctx);
    file = open_file(path);
    अगर (file)
        जाओ found_path;
    मुक्त(path);

    path = get_home_xcompose_file_path(ctx);
    file = open_file(path);
    अगर (file)
        जाओ found_path;
    मुक्त(path);

    path = get_locale_compose_file_path(ctx, table->locale);
    file = open_file(path);
    अगर (file)
        जाओ found_path;
    मुक्त(path);

    log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
            "couldn't find a Compose file for locale \"%s\" (mapped to \"%s\")\n",
            locale, table->locale);
    xkb_compose_table_unref(table);
    वापस शून्य;

found_path:
    {} /* Label followed by a declaration is a C23 extension */
    स्थिर bool ok = parse_file(table, file, path);
    ख_बंद(file);
    अगर (!ok) {
        मुक्त(path);
        xkb_compose_table_unref(table);
        वापस शून्य;
    }

    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "created compose table from locale %s with path %s\n",
            table->locale, path);

    मुक्त(path);
    वापस table;
}

स्थिर xkb_keysym_t *
xkb_compose_table_entry_sequence(काष्ठा xkb_compose_table_entry *entry,
                                 माप_प्रकार *sequence_लंबाई)
{
    *sequence_लंबाई = entry->sequence_लंबाई;
    वापस entry->sequence;
}

xkb_keysym_t
xkb_compose_table_entry_keysym(काष्ठा xkb_compose_table_entry *entry)
{
    वापस entry->keysym;
}

स्थिर अक्षर *
xkb_compose_table_entry_utf8(काष्ठा xkb_compose_table_entry *entry)
{
    वापस entry->utf8;
}

#अगर MAX_COMPOSE_NODES_LOG2 > 31
#त्रुटि "Cannot implement bit field xkb_compose_table_iterator_pending_node.offset"
#पूर्ण_अगर

काष्ठा xkb_compose_table_iterator_pending_गाँठ {
    uint32_t offset:31;
    bool processed:1;
};

काष्ठा xkb_compose_table_iterator {
    काष्ठा xkb_compose_table *table;
    /* Current entry */
    काष्ठा xkb_compose_table_entry entry;
    /* Stack of pending nodes to process */
    darray(काष्ठा xkb_compose_table_iterator_pending_गाँठ) pending_nodes;
};

काष्ठा xkb_compose_table_iterator *
xkb_compose_table_iterator_new(काष्ठा xkb_compose_table *table)
{
    काष्ठा xkb_compose_table_iterator *iter;
    xkb_keysym_t *sequence;

    iter = सुस्मृति(1, माप(*iter));
    अगर (!iter) {
        वापस शून्य;
    }
    iter->table = xkb_compose_table_ref(table);
    sequence = सुस्मृति(COMPOSE_MAX_LHS_LEN, माप(xkb_keysym_t));
    अगर (!sequence) {
        मुक्त(iter);
        वापस शून्य;
    }
    iter->entry.sequence = sequence;
    iter->entry.sequence_लंबाई = 0;

    darray_init(iter->pending_nodes);
    /* Short-circuit if table contains only the dummy entry */
    अगर (darray_आकार(table->nodes) == 1) {
        वापस iter;
    }
    /* Add root node */
    काष्ठा xkb_compose_table_iterator_pending_गाँठ pending = {
        .offset = 1,
        .processed = false
    };
    darray_append(iter->pending_nodes, pending);
    स्थिर काष्ठा compose_गाँठ *गाँठ = &darray_item(iter->table->nodes,
                                                   pending.offset);

    /* Find the first left-most node and store intermediate nodes as pending */
    जबतक (गाँठ->lokid) {
        pending.offset = गाँठ->lokid;
        darray_append(iter->pending_nodes, pending);
        गाँठ = &darray_item(iter->table->nodes, pending.offset);
    };

    वापस iter;
}

व्योम
xkb_compose_table_iterator_free(काष्ठा xkb_compose_table_iterator *iter)
{
    xkb_compose_table_unref(iter->table);
    darray_free(iter->pending_nodes);
    मुक्त(iter->entry.sequence);
    मुक्त(iter);
}

काष्ठा xkb_compose_table_entry *
xkb_compose_table_iterator_अग्र(काष्ठा xkb_compose_table_iterator *iter)
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

    काष्ठा xkb_compose_table_iterator_pending_गाँठ *pending;
    स्थिर काष्ठा compose_गाँठ *गाँठ;

    /* Iterator is empty if there is no pending nodes */
    अगर (unlikely(darray_empty(iter->pending_nodes))) {
        वापस शून्य;
    }

    /* Resume to the last element in the stack */
    pending = &darray_item(iter->pending_nodes,
                           darray_आकार(iter->pending_nodes) - 1);
    गाँठ = &darray_item(iter->table->nodes, pending->offset);

    काष्ठा xkb_compose_table_iterator_pending_गाँठ new = {
        .processed = false,
        .offset = 0
    };

    /* Remove processed leaves until an unprocessed right arrow or parent
     * is found, then update entry accordingly */
    जबतक (pending->processed) {
        /* Remove last keysym */
        iter->entry.sequence_लंबाई--;
        अगर (गाँठ->hikid) {
            /* Follow right arrow: replace current pending node */
            pending->processed = false;
            pending->offset = गाँठ->hikid;
            /* Process child’s left arrow */
            जाओ गाँठ_बायाँ;
        } अन्यथा {
            /* Remove processed node */
            darray_remove_last(iter->pending_nodes);
            अगर (unlikely(darray_empty(iter->pending_nodes))) {
                /* No more nodes to process */
                वापस शून्य;
            }
            /* Get parent node */
            pending = &darray_item(iter->pending_nodes,
                                   darray_आकार(iter->pending_nodes) - 1);
            गाँठ = &darray_item(iter->table->nodes, pending->offset);
        }
    }

    जबतक (1) {
        /* Follow down arrow */
        pending->processed = true;
        iter->entry.sequence_लंबाई++;
        iter->entry.sequence[iter->entry.sequence_लंबाई - 1] = गाँठ->keysym;
        अगर (गाँठ->is_leaf) {
            /* Leaf: return entry */
            iter->entry.keysym = गाँठ->leaf.keysym;
            iter->entry.utf8 = &darray_item(iter->table->utf8, गाँठ->leaf.utf8);
            वापस &iter->entry;
        }
        /* Not a leaf: process child node */
        new.offset = गाँठ->internal.eqkid;
        darray_append(iter->pending_nodes, new);
        pending = &darray_item(iter->pending_nodes,
                               darray_आकार(iter->pending_nodes) - 1);
गाँठ_बायाँ:
        गाँठ = &darray_item(iter->table->nodes, pending->offset);
        /* Find the next left-most arrow and store intermediate pending nodes */
        जबतक (गाँठ->lokid) {
            /* Follow left arrow */
            new.offset = गाँठ->lokid;
            darray_append(iter->pending_nodes, new);
            pending = &darray_item(iter->pending_nodes,
                                   darray_आकार(iter->pending_nodes) - 1);
            गाँठ = &darray_item(iter->table->nodes, new.offset);
        }
    }
}
