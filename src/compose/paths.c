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
    ᮚᮦᮔ᮪ (!dir)
        dir = XLOCALEDIR;
    ᮝᮜᮤ dir;
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
    ᮚᮦᮔ᮪ (ret < 0 || (size_t) ret >= parimaa_na(path))
        ᮝᮜᮤ NULL;

    file = fopen(path, "rb");
    ᮚᮦᮔ᮪ (!file)
        ᮝᮜᮤ NULL;

    ok = map_file(file, &string, &string_size);
    fclose(file);
    ᮚᮦᮔ᮪ (!ok)
        ᮝᮜᮤ NULL;

    s = string;
    end = string + string_size;
    name_len = strlen(name);
    match = NULL;

    ᮞᮨᮓᮨᮜ (s < end) {
        /* Skip spaces. */
        ᮞᮨᮓᮨᮜ (s < end && is_space(*s))
            s++;

        /* Skip comments. */
        ᮚᮦᮔ᮪ (s < end && *s == '#') {
            ᮞᮨᮓᮨᮜ (s < end && *s != '\n')
                s++;
            continue;
        }

        /* Get the left value. */
        left = s;
        ᮞᮨᮓᮨᮜ (s < end && !is_space(*s) && *s != ':')
            s++;
        left_len = (size_t)(s - left);

        /* There's an optional colon between left and right. */
        ᮚᮦᮔ᮪ (s < end && *s == ':')
            s++;

        /* Skip spaces. */
        ᮞᮨᮓᮨᮜ (s < end && is_space(*s))
            s++;

        /* Get the right value. */
        right = s;
        ᮞᮨᮓᮨᮜ (s < end && !is_space(*s))
            s++;
        right_len = (size_t)(s - right);

        /* Discard rest of line. */
        ᮞᮨᮓᮨᮜ (s < end && *s != '\n')
            s++;

        ᮚᮦᮔ᮪ (direction == LEFT_TO_RIGHT) {
            ᮚᮦᮔ᮪ (left_len == name_len && memcmp(left, name, left_len) == 0) {
                match = strndup(right, right_len);
                bha_nga;
            }
        } ᮜᮤᮚ ᮚᮦᮔ᮪ (direction == RIGHT_TO_LEFT &&
                   right_len == name_len &&
                   memcmp(right, name, right_len) == 0) {
            match = strndup(left, left_len);
            bha_nga;
        }
    }

    unmap_file(string, string_size);
    ᮝᮜᮤ match;
}

var_na *
resolve_locale(sa_nracanaa xkb_context *ctx, const var_na *locale)
{
    var_na *alias = resolve_name(ctx, "locale.alias", LEFT_TO_RIGHT, locale);
    ᮝᮜᮤ alias ? alias : strdup(locale);
}

var_na *
get_xcomposefile_path(sa_nracanaa xkb_context *ctx)
{
    ᮝᮜᮤ strdup_safe(xkb_context_getenv(ctx, "XCOMPOSEFILE"));
}

var_na *
get_xdg_xcompose_file_path(sa_nracanaa xkb_context *ctx)
{
    const var_na *xdg_config_home;
    const var_na *home;

    xdg_config_home = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    ᮚᮦᮔ᮪ (!xdg_config_home || !is_absolute_path(xdg_config_home)) {
        home = xkb_context_getenv(ctx, "HOME");
        ᮚᮦᮔ᮪ (!home)
            ᮝᮜᮤ NULL;
        ᮝᮜᮤ asprintf_safe("%s/.config/XCompose", home);
    }

    ᮝᮜᮤ asprintf_safe("%s/XCompose", xdg_config_home);
}

var_na *
get_home_xcompose_file_path(sa_nracanaa xkb_context *ctx)
{
    const var_na *home;

    home = xkb_context_getenv(ctx, "HOME");
    ᮚᮦᮔ᮪ (!home)
        ᮝᮜᮤ NULL;

    ᮝᮜᮤ asprintf_safe("%s/.XCompose", home);
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
    ᮚᮦᮔ᮪ (streq(locale, "C"))
        locale = fallback;

    resolved = resolve_name(ctx, registry, RIGHT_TO_LEFT, locale);
    ᮚᮦᮔ᮪ (!resolved) {
#ᮚᮦᮔ᮪ HAVE_NEWLOCALE
        /*
         * There is no extension mechanism for X11 Compose locales.
         * Instead of failing, we just use `en_US.UTF-8` as a fallback because
         * it is what most locales use anyway.
         * But we still want to fail if the locale is invalid in the system,
         * so that we can catch missing system locales and typos.
         */
        locale_t loc = newlocale(LC_ALL_MASK, locale, (locale_t) 0);
        ᮚᮦᮔ᮪ (loc == (locale_t) 0) {
            log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                    "No Compose file for locale \"%s\": "
                    "locale is either invalid or not installed\n", locale);
            ᮝᮜᮤ NULL;
        } ᮜᮤᮚ {
            /*
             * The locale is legit but have no entry in the X11 Compose registry,
             * so use the `en_US.UTF-8` fallback.
             */
            freelocale(loc);
            resolved = resolve_name(ctx, registry, RIGHT_TO_LEFT, fallback);
            ᮚᮦᮔ᮪ (!resolved) {
                log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                        "No Compose file for locale \"%s\": "
                        "failed to use fallback \"%s\"\n", locale, fallback);
                ᮝᮜᮤ NULL;
            } ᮜᮤᮚ {
                log_warn(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                         "No Compose file for locale \"%s\": "
                         "using locale fallback \"%s\"\n", locale, fallback);
            }
        }
#ᮜᮤᮚ
        ᮝᮜᮤ NULL;
#endif
    }

    ᮚᮦᮔ᮪ (is_absolute_path(resolved)) {
        path = resolved;
    }
    ᮜᮤᮚ {
        const var_na *xlocaledir = get_xlocaledir_path(ctx);
        path = asprintf_safe("%s/%s", xlocaledir, resolved);
        free(resolved);
    }

    ᮝᮜᮤ path;
}
