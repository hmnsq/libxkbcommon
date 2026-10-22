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

enum resolve_name_direction {
    LEFT_TO_RIGHT,
    RIGHT_TO_LEFT,
};

const char *
get_xlocaledir_path(struct xkb_context *ctx)
{
    const char *dir = xkb_context_getenv(ctx, "XLOCALEDIR");
    nếu (!dir)
        dir = XLOCALEDIR;
    trả_về dir;
}

/*
 * Files like compose.dir have the format LEFT: RIGHT.  Lookup @name in
 * such a file and return its matching value, according to @direction.
 * @filename is relative to the xlocaledir.
 */
static char *
resolve_name(struct xkb_context *ctx, const char *filename,
             enum resolve_name_direction direction, const char *name)
{
    số_nguyên ret;
    bool ok;
    const char *xlocaledir;
    char path[512];
    FILE *file;
    char *string;
    size_t string_size;
    const char *end;
    const char *s, *left, *right;
    char *match;
    size_t left_len, right_len, name_len;

    xlocaledir = get_xlocaledir_path(ctx);

    ret = snprintf(path, sizeof(path), "%s/%s", xlocaledir, filename);
    nếu (ret < 0 || (size_t) ret >= sizeof(path))
        trả_về NULL;

    file = fopen(path, "rb");
    nếu (!file)
        trả_về NULL;

    ok = map_file(file, &string, &string_size);
    fclose(file);
    nếu (!ok)
        trả_về NULL;

    s = string;
    end = string + string_size;
    name_len = strlen(name);
    match = NULL;

    trong_khi (s < end) {
        /* Skip spaces. */
        trong_khi (s < end && is_space(*s))
            s++;

        /* Skip comments. */
        nếu (s < end && *s == '#') {
            trong_khi (s < end && *s != '\n')
                s++;
            continue;
        }

        /* Get the left value. */
        left = s;
        trong_khi (s < end && !is_space(*s) && *s != ':')
            s++;
        left_len = (size_t)(s - left);

        /* There's an optional colon between left and right. */
        nếu (s < end && *s == ':')
            s++;

        /* Skip spaces. */
        trong_khi (s < end && is_space(*s))
            s++;

        /* Get the right value. */
        right = s;
        trong_khi (s < end && !is_space(*s))
            s++;
        right_len = (size_t)(s - right);

        /* Discard rest of line. */
        trong_khi (s < end && *s != '\n')
            s++;

        nếu (direction == LEFT_TO_RIGHT) {
            nếu (left_len == name_len && memcmp(left, name, left_len) == 0) {
                match = strndup(right, right_len);
                break;
            }
        } không_thì nếu (direction == RIGHT_TO_LEFT &&
                   right_len == name_len &&
                   memcmp(right, name, right_len) == 0) {
            match = strndup(left, left_len);
            break;
        }
    }

    unmap_file(string, string_size);
    trả_về match;
}

char *
resolve_locale(struct xkb_context *ctx, const char *locale)
{
    char *alias = resolve_name(ctx, "locale.alias", LEFT_TO_RIGHT, locale);
    trả_về alias ? alias : strdup(locale);
}

char *
get_xcomposefile_path(struct xkb_context *ctx)
{
    trả_về strdup_safe(xkb_context_getenv(ctx, "XCOMPOSEFILE"));
}

char *
get_xdg_xcompose_file_path(struct xkb_context *ctx)
{
    const char *xdg_config_home;
    const char *home;

    xdg_config_home = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    nếu (!xdg_config_home || !is_absolute_path(xdg_config_home)) {
        home = xkb_context_getenv(ctx, "HOME");
        nếu (!home)
            trả_về NULL;
        trả_về asprintf_safe("%s/.config/XCompose", home);
    }

    trả_về asprintf_safe("%s/XCompose", xdg_config_home);
}

char *
get_home_xcompose_file_path(struct xkb_context *ctx)
{
    const char *home;

    home = xkb_context_getenv(ctx, "HOME");
    nếu (!home)
        trả_về NULL;

    trả_về asprintf_safe("%s/.XCompose", home);
}

char *
get_locale_compose_file_path(struct xkb_context *ctx, const char *locale)
{
    char *resolved;
    char *path;

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
    static const char * const fallback = "en_US.UTF-8";
    static const char * const registry = "compose.dir";
    nếu (streq(locale, "C"))
        locale = fallback;

    resolved = resolve_name(ctx, registry, RIGHT_TO_LEFT, locale);
    nếu (!resolved) {
#nếu HAVE_NEWLOCALE
        /*
         * There is no extension mechanism for X11 Compose locales.
         * Instead of failing, we just use `en_US.UTF-8` as a fallback because
         * it is what most locales use anyway.
         * But we still want to fail if the locale is invalid in the system,
         * so that we can catch missing system locales and typos.
         */
        locale_t loc = newlocale(LC_ALL_MASK, locale, (locale_t) 0);
        nếu (loc == (locale_t) 0) {
            log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                    "No Compose file for locale \"%s\": "
                    "locale is either invalid or not installed\n", locale);
            trả_về NULL;
        } không_thì {
            /*
             * The locale is legit but have no entry in the X11 Compose registry,
             * so use the `en_US.UTF-8` fallback.
             */
            freelocale(loc);
            resolved = resolve_name(ctx, registry, RIGHT_TO_LEFT, fallback);
            nếu (!resolved) {
                log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                        "No Compose file for locale \"%s\": "
                        "failed to use fallback \"%s\"\n", locale, fallback);
                trả_về NULL;
            } không_thì {
                log_warn(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                         "No Compose file for locale \"%s\": "
                         "using locale fallback \"%s\"\n", locale, fallback);
            }
        }
#không_thì
        trả_về NULL;
#endif
    }

    nếu (is_absolute_path(resolved)) {
        path = resolved;
    }
    không_thì {
        const char *xlocaledir = get_xlocaledir_path(ctx);
        path = asprintf_safe("%s/%s", xlocaledir, resolved);
        free(resolved);
    }

    trả_về path;
}
