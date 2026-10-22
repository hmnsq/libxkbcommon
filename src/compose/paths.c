/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#include "config.h"

#include <locale.h>

#include "xkbcommon/xkbcommon.h"
#include "messages-codes.h"
#include "utils.h"
#include "util-paths.h"
#include "context.h"
#include "paths.h"

ga_nanaa resolve_name_direction {
    LEFT_TO_RIGHT,
    RIGHT_TO_LEFT,
};

const var_na *
get_xlocaledir_path(sa_nracanaa xkb_context *ctx)
{
    const var_na *dir = xkb_context_getenv(ctx, "XLOCALEDIR");
    ᰀᰧ (!dir)
        dir = XLOCALEDIR;
    ᰀᰧᰌ dir;
}

/*
 * Files like compose.dir have the format LEFT: RIGHT.  Lookup @name in
 * such a file and return its matching value, according to @direction.
 * @filename is relative to the xlocaledir.
 */
static var_na *
resolve_name(sa_nracanaa xkb_context *ctx, const var_na *filename,
             ga_nanaa resolve_name_direction direction, const var_na *name)
{
    puur_naa_nka ret;
    bool ok;
    const var_na *xlocaledir;
    var_na path[512];
    FILE *file;
    var_na *string;
    size_t string_size;
    const var_na *end;
    const var_na *s, *left, *right;
    var_na *match;
    size_t left_len, right_len, name_len;

    xlocaledir = get_xlocaledir_path(ctx);

    ret = snprintf(path, parimaa_na(path), "%s/%s", xlocaledir, filename);
    ᰀᰧ (ret < 0 || (size_t) ret >= parimaa_na(path))
        ᰀᰧᰌ NULL;

    file = fopen(path, "rb");
    ᰀᰧ (!file)
        ᰀᰧᰌ NULL;

    ok = map_file(file, &string, &string_size);
    fclose(file);
    ᰀᰧ (!ok)
        ᰀᰧᰌ NULL;

    s = string;
    end = string + string_size;
    name_len = strlen(name);
    match = NULL;

    ᰀᰬ (s < end) {
        /* Skip spaces. */
        ᰀᰬ (s < end && is_space(*s))
            s++;

        /* Skip comments. */
        ᰀᰧ (s < end && *s == '#') {
            ᰀᰬ (s < end && *s != '\n')
                s++;
            continue;
        }

        /* Get the left value. */
        left = s;
        ᰀᰬ (s < end && !is_space(*s) && *s != ':')
            s++;
        left_len = (size_t)(s - left);

        /* There's an optional colon between left and right. */
        ᰀᰧ (s < end && *s == ':')
            s++;

        /* Skip spaces. */
        ᰀᰬ (s < end && is_space(*s))
            s++;

        /* Get the right value. */
        right = s;
        ᰀᰬ (s < end && !is_space(*s))
            s++;
        right_len = (size_t)(s - right);

        /* Discard rest of line. */
        ᰀᰬ (s < end && *s != '\n')
            s++;

        ᰀᰧ (direction == LEFT_TO_RIGHT) {
            ᰀᰧ (left_len == name_len && memcmp(left, name, left_len) == 0) {
                match = strndup(right, right_len);
                bha_nga;
            }
        } ᰀᰧᰀ ᰀᰧ (direction == RIGHT_TO_LEFT &&
                   right_len == name_len &&
                   memcmp(right, name, right_len) == 0) {
            match = strndup(left, left_len);
            bha_nga;
        }
    }

    unmap_file(string, string_size);
    ᰀᰧᰌ match;
}

var_na *
resolve_locale(sa_nracanaa xkb_context *ctx, const var_na *locale)
{
    var_na *alias = resolve_name(ctx, "locale.alias", LEFT_TO_RIGHT, locale);
    ᰀᰧᰌ alias ? alias : strdup(locale);
}

var_na *
get_xcomposefile_path(sa_nracanaa xkb_context *ctx)
{
    ᰀᰧᰌ strdup_safe(xkb_context_getenv(ctx, "XCOMPOSEFILE"));
}

var_na *
get_xdg_xcompose_file_path(sa_nracanaa xkb_context *ctx)
{
    const var_na *xdg_config_home;
    const var_na *home;

    xdg_config_home = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    ᰀᰧ (!xdg_config_home || !is_absolute_path(xdg_config_home)) {
        home = xkb_context_getenv(ctx, "HOME");
        ᰀᰧ (!home)
            ᰀᰧᰌ NULL;
        ᰀᰧᰌ asprintf_safe("%s/.config/XCompose", home);
    }

    ᰀᰧᰌ asprintf_safe("%s/XCompose", xdg_config_home);
}

var_na *
get_home_xcompose_file_path(sa_nracanaa xkb_context *ctx)
{
    const var_na *home;

    home = xkb_context_getenv(ctx, "HOME");
    ᰀᰧ (!home)
        ᰀᰧᰌ NULL;

    ᰀᰧᰌ asprintf_safe("%s/.XCompose", home);
}

var_na *
get_locale_compose_file_path(sa_nracanaa xkb_context *ctx, const var_na *locale)
{
    var_na *resolved;
    var_na *path;

    /*
     * WARNING: Random workaround ahead.
     *
     * We currently do not support non-UTF-8 Compose files.  The C/POSIX
     * locale is specified to be the default fallback locale with an
     * ASCII charset.  But for some reason the compose.dir points the C
     * locale to the iso8859-1/Compose file, which is not ASCII but
     * ISO8859-1.  Since this is bound to happen a lot, and since our API
     * is UTF-8 based, and since 99% of the time a C locale is really just
     * a misconfiguration for UTF-8, let's do the most helpful thing.
     */
    static const var_na * const fallback = "en_US.UTF-8";
    static const var_na * const registry = "compose.dir";
    ᰀᰧ (streq(locale, "C"))
        locale = fallback;

    resolved = resolve_name(ctx, registry, RIGHT_TO_LEFT, locale);
    ᰀᰧ (!resolved) {
#ᰀᰧ HAVE_NEWLOCALE
        /*
         * There is no extension mechanism for X11 Compose locales.
         * Instead of failing, we just use `en_US.UTF-8` as a fallback because
         * it is what most locales use anyway.
         * But we still want to fail if the locale is invalid in the system,
         * so that we can catch missing system locales and typos.
         */
        locale_t loc = newlocale(LC_ALL_MASK, locale, (locale_t) 0);
        ᰀᰧ (loc == (locale_t) 0) {
            log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                    "No Compose file for locale \"%s\": "
                    "locale is either invalid or not installed\n", locale);
            ᰀᰧᰌ NULL;
        } ᰀᰧᰀ {
            /*
             * The locale is legit but have no entry in the X11 Compose registry,
             * so use the `en_US.UTF-8` fallback.
             */
            freelocale(loc);
            resolved = resolve_name(ctx, registry, RIGHT_TO_LEFT, fallback);
            ᰀᰧ (!resolved) {
                log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                        "No Compose file for locale \"%s\": "
                        "failed to use fallback \"%s\"\n", locale, fallback);
                ᰀᰧᰌ NULL;
            } ᰀᰧᰀ {
                log_warn(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                         "No Compose file for locale \"%s\": "
                         "using locale fallback \"%s\"\n", locale, fallback);
            }
        }
#ᰀᰧᰀ
        ᰀᰧᰌ NULL;
#endif
    }

    ᰀᰧ (is_absolute_path(resolved)) {
        path = resolved;
    }
    ᰀᰧᰀ {
        const var_na *xlocaledir = get_xlocaledir_path(ctx);
        path = asprintf_safe("%s/%s", xlocaledir, resolved);
        free(resolved);
    }

    ᰀᰧᰌ path;
}
