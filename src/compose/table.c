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

स्थिर संरचना xkb_compose_सारणी *
xkb_compose_सारणी_new(संरचना xkb_context *ctx, ध्रुव वर्ण *func,
                      ध्रुव वर्ण *locale,
                      गणना xkb_compose_format format,
                      गणना xkb_compose_compile_flags flags)
{
    ध्रुव गणना xkb_compose_compile_flags invalid_flags = (
        flags & ~(गणना xkb_compose_compile_flags)XKB_COMPOSE_COMPILE_FLAGS_मानानि
    );
    यदि (invalid_flags) {
        log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                "%s: unrecognized flags: %#x\n", func, invalid_flags);
        प्रत्यागम रिक्त;
    }

    यदि (format != XKB_COMPOSE_FORMAT_TEXT_V1) {
        log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                "%s: unsupported compose format: %d\n", func, format);
        प्रत्यागम रिक्त;
    }

    वर्ण *resolved_locale;
    संरचना xkb_compose_सारणी *सारणी;
    संरचना compose_ग्रन्थिः dummy = {0};

    resolved_locale = resolve_locale(ctx, locale);
    यदि (!resolved_locale)
        प्रत्यागम रिक्त;

    सारणी = शून्यस्मृतिं_देहि(1, परिमाण(*सारणी));
    यदि (!सारणी) {
        मुक्त(resolved_locale);
        प्रत्यागम रिक्त;
    }

    सारणी->refcnt = 1;
    सारणी->ctx = xkb_context_ref(ctx);

    सारणी->locale = resolved_locale;
    सारणी->format = format;
    सारणी->flags = flags;

    darray_init(सारणी->nodes);
    darray_init(सारणी->utf8);

    dummy.keysym = XKB_कुञ्चिका_No_चिह्नम्;
    dummy.पर्णम्.is_पर्णम् = true;
    dummy.पर्णम्.utf8 = 0;
    dummy.पर्णम्.keysym = XKB_कुञ्चिका_No_चिह्नम्;
    darray_append(सारणी->nodes, dummy);

    darray_append(सारणी->utf8, '\0');

    प्रत्यागम सारणी;
}

संरचना xkb_compose_सारणी *
xkb_compose_सारणी_ref(संरचना xkb_compose_सारणी *सारणी)
{
    प्रतिज्ञा(सारणी->refcnt > 0);
    सारणी->refcnt++;
    प्रत्यागम सारणी;
}

शून्य
xkb_compose_सारणी_unref(संरचना xkb_compose_सारणी *सारणी)
{
    प्रतिज्ञा(!सारणी || सारणी->refcnt > 0);
    यदि (!सारणी || --सारणी->refcnt > 0)
        प्रत्यागम;
    मुक्त(सारणी->locale);
    darray_free(सारणी->nodes);
    darray_free(सारणी->utf8);
    xkb_context_unref(सारणी->ctx);
    मुक्त(सारणी);
}

संरचना xkb_compose_सारणी *
xkb_compose_सारणी_new_from_file(संरचना xkb_context *ctx,
                                FILE *file,
                                ध्रुव वर्ण *locale,
                                गणना xkb_compose_format format,
                                गणना xkb_compose_compile_flags flags)
{
    संरचना xkb_compose_सारणी * ध्रुव सारणी =
        xkb_compose_सारणी_new(ctx, __func__, locale, format, flags);
    यदि (!सारणी)
        प्रत्यागम रिक्त;

    यदि (!parse_file(सारणी, file, "(unknown file)")) {
        xkb_compose_सारणी_unref(सारणी);
        प्रत्यागम रिक्त;
    }

    प्रत्यागम सारणी;
}

संरचना xkb_compose_सारणी *
xkb_compose_सारणी_new_from_बफर(संरचना xkb_context *ctx,
                                  ध्रुव वर्ण *बफर, आकार_t दीर्घता,
                                  ध्रुव वर्ण *locale,
                                  गणना xkb_compose_format format,
                                  गणना xkb_compose_compile_flags flags)
{
    संरचना xkb_compose_सारणी * ध्रुव सारणी =
        xkb_compose_सारणी_new(ctx, __func__, locale, format, flags);
    यदि (!सारणी)
        प्रत्यागम रिक्त;

    यदि (!parse_string(सारणी, बफर, दीर्घता, "(input string)")) {
        xkb_compose_सारणी_unref(सारणी);
        प्रत्यागम रिक्त;
    }

    प्रत्यागम सारणी;
}

संरचना xkb_compose_सारणी *
xkb_compose_सारणी_new_from_locale(संरचना xkb_context *ctx,
                                  ध्रुव वर्ण *locale,
                                  गणना xkb_compose_compile_flags flags)
{
    स्थिर ध्रुव गणना xkb_compose_format format = XKB_COMPOSE_FORMAT_TEXT_V1;
    संरचना xkb_compose_सारणी * ध्रुव सारणी =
        xkb_compose_सारणी_new(ctx, __func__, locale, format, flags);
    यदि (!सारणी)
        प्रत्यागम रिक्त;

    वर्ण *मार्गः = get_xcomposefile_मार्गः(ctx);
    FILE *file = open_file(मार्गः);
    यदि (file)
        गच्छ प्राप्तम्_मार्गः;
    मुक्त(मार्गः);

    मार्गः = get_xdg_xcompose_file_मार्गः(ctx);
    file = open_file(मार्गः);
    यदि (file)
        गच्छ प्राप्तम्_मार्गः;
    मुक्त(मार्गः);

    मार्गः = get_home_xcompose_file_मार्गः(ctx);
    file = open_file(मार्गः);
    यदि (file)
        गच्छ प्राप्तम्_मार्गः;
    मुक्त(मार्गः);

    मार्गः = get_locale_compose_file_मार्गः(ctx, सारणी->locale);
    file = open_file(मार्गः);
    यदि (file)
        गच्छ प्राप्तम्_मार्गः;
    मुक्त(मार्गः);

    log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
            "couldn't find a Compose file for locale \"%s\" (mapped to \"%s\")\n",
            locale, सारणी->locale);
    xkb_compose_सारणी_unref(सारणी);
    प्रत्यागम रिक्त;

प्राप्तम्_मार्गः:
    {} /* Label followed by a declaration is a C23 extension */
    ध्रुव bool ok = parse_file(सारणी, file, मार्गः);
    fclose(file);
    यदि (!ok) {
        मुक्त(मार्गः);
        xkb_compose_सारणी_unref(सारणी);
        प्रत्यागम रिक्त;
    }

    log_dbg(ctx, XKB_LOG_सन्देशः_NO_ID,
            "created compose table from locale %s with path %s\n",
            सारणी->locale, मार्गः);

    मुक्त(मार्गः);
    प्रत्यागम सारणी;
}

ध्रुव xkb_keysym_t *
xkb_compose_सारणी_प्रविष्टिः_sequence(संरचना xkb_compose_सारणी_प्रविष्टिः *प्रविष्टिः,
                                 आकार_t *sequence_दीर्घता)
{
    *sequence_दीर्घता = प्रविष्टिः->sequence_दीर्घता;
    प्रत्यागम प्रविष्टिः->sequence;
}

xkb_keysym_t
xkb_compose_सारणी_प्रविष्टिः_keysym(संरचना xkb_compose_सारणी_प्रविष्टिः *प्रविष्टिः)
{
    प्रत्यागम प्रविष्टिः->keysym;
}

ध्रुव वर्ण *
xkb_compose_सारणी_प्रविष्टिः_utf8(संरचना xkb_compose_सारणी_प्रविष्टिः *प्रविष्टिः)
{
    प्रत्यागम प्रविष्टिः->utf8;
}

#यदि MAX_COMPOSE_NODES_LOG2 > 31
#दोषः "Cannot implement bit field xkb_compose_table_iterator_pending_node.offset"
#यदि_अन्तः

संरचना xkb_compose_सारणी_iterator_pending_ग्रन्थिः {
    uint32_t विस्थापनम्:31;
    bool processed:1;
};

संरचना xkb_compose_सारणी_iterator {
    संरचना xkb_compose_सारणी *सारणी;
    /* Current entry */
    संरचना xkb_compose_सारणी_प्रविष्टिः प्रविष्टिः;
    /* Stack of pending nodes to process */
    darray(संरचना xkb_compose_सारणी_iterator_pending_ग्रन्थिः) pending_nodes;
};

संरचना xkb_compose_सारणी_iterator *
xkb_compose_सारणी_iterator_new(संरचना xkb_compose_सारणी *सारणी)
{
    संरचना xkb_compose_सारणी_iterator *iter;
    xkb_keysym_t *sequence;

    iter = शून्यस्मृतिं_देहि(1, परिमाण(*iter));
    यदि (!iter) {
        प्रत्यागम रिक्त;
    }
    iter->सारणी = xkb_compose_सारणी_ref(सारणी);
    sequence = शून्यस्मृतिं_देहि(COMPOSE_MAX_LHS_LEN, परिमाण(xkb_keysym_t));
    यदि (!sequence) {
        मुक्त(iter);
        प्रत्यागम रिक्त;
    }
    iter->प्रविष्टिः.sequence = sequence;
    iter->प्रविष्टिः.sequence_दीर्घता = 0;

    darray_init(iter->pending_nodes);
    /* Short-circuit if table contains only the dummy entry */
    यदि (darray_आकार(सारणी->nodes) == 1) {
        प्रत्यागम iter;
    }
    /* Add root node */
    संरचना xkb_compose_सारणी_iterator_pending_ग्रन्थिः pending = {
        .विस्थापनम् = 1,
        .processed = false
    };
    darray_append(iter->pending_nodes, pending);
    ध्रुव संरचना compose_ग्रन्थिः *ग्रन्थिः = &darray_item(iter->सारणी->nodes,
                                                   pending.विस्थापनम्);

    /* Find the first left-most node and store intermediate nodes as pending */
    यावत् (ग्रन्थिः->lokid) {
        pending.विस्थापनम् = ग्रन्थिः->lokid;
        darray_append(iter->pending_nodes, pending);
        ग्रन्थिः = &darray_item(iter->सारणी->nodes, pending.विस्थापनम्);
    };

    प्रत्यागम iter;
}

शून्य
xkb_compose_सारणी_iterator_free(संरचना xkb_compose_सारणी_iterator *iter)
{
    xkb_compose_सारणी_unref(iter->सारणी);
    darray_free(iter->pending_nodes);
    मुक्त(iter->प्रविष्टिः.sequence);
    मुक्त(iter);
}

संरचना xkb_compose_सारणी_प्रविष्टिः *
xkb_compose_सारणी_iterator_अग्र(संरचना xkb_compose_सारणी_iterator *iter)
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

    संरचना xkb_compose_सारणी_iterator_pending_ग्रन्थिः *pending;
    ध्रुव संरचना compose_ग्रन्थिः *ग्रन्थिः;

    /* Iterator is empty if there is no pending nodes */
    यदि (unlikely(darray_रिक्तानि(iter->pending_nodes))) {
        प्रत्यागम रिक्त;
    }

    /* Resume to the last element in the stack */
    pending = &darray_item(iter->pending_nodes,
                           darray_आकार(iter->pending_nodes) - 1);
    ग्रन्थिः = &darray_item(iter->सारणी->nodes, pending->विस्थापनम्);

    संरचना xkb_compose_सारणी_iterator_pending_ग्रन्थिः new = {
        .processed = false,
        .विस्थापनम् = 0
    };

    /* Remove processed leaves until an unprocessed right arrow or parent
     * is found, then update entry accordingly */
    यावत् (pending->processed) {
        /* Remove last keysym */
        iter->प्रविष्टिः.sequence_दीर्घता--;
        यदि (ग्रन्थिः->hikid) {
            /* Follow right arrow: replace current pending node */
            pending->processed = false;
            pending->विस्थापनम् = ग्रन्थिः->hikid;
            /* Process child’s left arrow */
            गच्छ ग्रन्थिः_वामः;
        } अन्यथा {
            /* Remove processed node */
            darray_remove_चरमः(iter->pending_nodes);
            यदि (unlikely(darray_रिक्तानि(iter->pending_nodes))) {
                /* No more nodes to process */
                प्रत्यागम रिक्त;
            }
            /* Get parent node */
            pending = &darray_item(iter->pending_nodes,
                                   darray_आकार(iter->pending_nodes) - 1);
            ग्रन्थिः = &darray_item(iter->सारणी->nodes, pending->विस्थापनम्);
        }
    }

    यावत् (1) {
        /* Follow down arrow */
        pending->processed = true;
        iter->प्रविष्टिः.sequence_दीर्घता++;
        iter->प्रविष्टिः.sequence[iter->प्रविष्टिः.sequence_दीर्घता - 1] = ग्रन्थिः->keysym;
        यदि (ग्रन्थिः->is_पर्णम्) {
            /* Leaf: return entry */
            iter->प्रविष्टिः.keysym = ग्रन्थिः->पर्णम्.keysym;
            iter->प्रविष्टिः.utf8 = &darray_item(iter->सारणी->utf8, ग्रन्थिः->पर्णम्.utf8);
            प्रत्यागम &iter->प्रविष्टिः;
        }
        /* Not a leaf: process child node */
        new.विस्थापनम् = ग्रन्थिः->आन्तरिकम्.eqkid;
        darray_append(iter->pending_nodes, new);
        pending = &darray_item(iter->pending_nodes,
                               darray_आकार(iter->pending_nodes) - 1);
ग्रन्थिः_वामः:
        ग्रन्थिः = &darray_item(iter->सारणी->nodes, pending->विस्थापनम्);
        /* Find the next left-most arrow and store intermediate pending nodes */
        यावत् (ग्रन्थिः->lokid) {
            /* Follow left arrow */
            new.विस्थापनम् = ग्रन्थिः->lokid;
            darray_append(iter->pending_nodes, new);
            pending = &darray_item(iter->pending_nodes,
                                   darray_आकार(iter->pending_nodes) - 1);
            ग्रन्थिः = &darray_item(iter->सारणी->nodes, new.विस्थापनम्);
        }
    }
}
