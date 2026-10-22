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
    якщо (!dir)
        dir = XLOCALEDIR;
    повернути dir;
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
    ціле ret;
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
    якщо (ret < 0 || (size_t) ret >= sizeof(path))
        повернути NULL;

    file = fopen(path, "rb");
    якщо (!file)
        повернути NULL;

    ok = map_file(file, &string, &string_size);
    fclose(file);
    якщо (!ok)
        повернути NULL;

    s = string;
    end = string + string_size;
    name_len = strlen(name);
    match = NULL;

    поки (s < end) {
        /* Skip spaces. */
        поки (s < end && is_space(*s))
            s++;

        /* Skip comments. */
        якщо (s < end && *s == '#') {
            поки (s < end && *s != '\n')
                s++;
            continue;
        }

        /* Get the left value. */
        left = s;
        поки (s < end && !is_space(*s) && *s != ':')
            s++;
        left_len = (size_t)(s - left);

        /* There's an optional colon between left and right. */
        якщо (s < end && *s == ':')
            s++;

        /* Skip spaces. */
        поки (s < end && is_space(*s))
            s++;

        /* Get the right value. */
        right = s;
        поки (s < end && !is_space(*s))
            s++;
        right_len = (size_t)(s - right);

        /* Discard rest of line. */
        поки (s < end && *s != '\n')
            s++;

        якщо (direction == LEFT_TO_RIGHT) {
            якщо (left_len == name_len && memcmp(left, name, left_len) == 0) {
                match = strndup(right, right_len);
                break;
            }
        } інакше якщо (direction == RIGHT_TO_LEFT &&
                   right_len == name_len &&
                   memcmp(right, name, right_len) == 0) {
            match = strndup(left, left_len);
            break;
        }
    }

    unmap_file(string, string_size);
    повернути match;
}

char *
resolve_locale(struct xkb_context *ctx, const char *locale)
{
    char *alias = resolve_name(ctx, "locale.alias", LEFT_TO_RIGHT, locale);
    повернути alias ? alias : strdup(locale);
}

char *
get_xcomposefile_path(struct xkb_context *ctx)
{
    повернути strdup_safe(xkb_context_getenv(ctx, "XCOMPOSEFILE"));
}

char *
get_xdg_xcompose_file_path(struct xkb_context *ctx)
{
    const char *xdg_config_home;
    const char *home;

    xdg_config_home = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    якщо (!xdg_config_home || !is_absolute_path(xdg_config_home)) {
        home = xkb_context_getenv(ctx, "HOME");
        якщо (!home)
            повернути NULL;
        повернути asprintf_safe("%s/.config/XCompose", home);
    }

    повернути asprintf_safe("%s/XCompose", xdg_config_home);
}

char *
get_home_xcompose_file_path(struct xkb_context *ctx)
{
    const char *home;

    home = xkb_context_getenv(ctx, "HOME");
    якщо (!home)
        повернути NULL;

    повернути asprintf_safe("%s/.XCompose", home);
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
    якщо (streq(locale, "C"))
        locale = fallback;

    resolved = resolve_name(ctx, registry, RIGHT_TO_LEFT, locale);
    якщо (!resolved) {
#якщо HAVE_NEWLOCALE
        /*
         * There is no extension mechanism for X11 Compose locales.
         * Instead of failing, we just use `en_US.UTF-8` as a fallback because
         * it is what most locales use anyway.
         * But we still want to fail if the locale is invalid in the system,
         * so that we can catch missing system locales and typos.
         */
        locale_t loc = newlocale(LC_ALL_MASK, locale, (locale_t) 0);
        якщо (loc == (locale_t) 0) {
            log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                    "No Compose file for locale \"%s\": "
                    "locale is either invalid or not installed\n", locale);
            повернути NULL;
        } інакше {
            /*
             * The locale is legit but have no entry in the X11 Compose registry,
             * so use the `en_US.UTF-8` fallback.
             */
            freelocale(loc);
            resolved = resolve_name(ctx, registry, RIGHT_TO_LEFT, fallback);
            якщо (!resolved) {
                log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                        "No Compose file for locale \"%s\": "
                        "failed to use fallback \"%s\"\n", locale, fallback);
                повернути NULL;
            } інакше {
                log_warn(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                         "No Compose file for locale \"%s\": "
                         "using locale fallback \"%s\"\n", locale, fallback);
            }
        }
#інакше
        повернути NULL;
#endif
    }

    якщо (is_absolute_path(resolved)) {
        path = resolved;
    }
    інакше {
        const char *xlocaledir = get_xlocaledir_path(ctx);
        path = asprintf_safe("%s/%s", xlocaledir, resolved);
        free(resolved);
    }

    повернути path;
}
