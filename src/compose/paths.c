/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن <locale.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "messages-codes.h"
#ضمن "utils.h"
#ضمن "util-paths.h"
#ضمن "context.h"
#ضمن "paths.h"

تعداد resolve_name_direction {
    يسار_TO_يمين,
    يمين_TO_يسار,
};

ثابت حرف *
get_xlocaledir_path(بنية xkb_context *ctx)
{
    ثابت حرف *dir = xkb_context_getenv(ctx, "XLOCALEDIR");
    إذا (!dir)
        dir = XLOCALEDIR;
    إرجاع dir;
}

/*
 * Files like compose.dir have the format LEFT: RIGHT.  Lookup @name in
 * such a file and return its matching value, according to @direction.
 * @filename is relative to the xlocaledir.
 */
ساكن حرف *
resolve_name(بنية xkb_context *ctx, ثابت حرف *filename,
             تعداد resolve_name_direction direction, ثابت حرف *name)
{
    صحيح ret;
    bool ok;
    ثابت حرف *xlocaledir;
    حرف path[512];
    FILE *file;
    حرف *string;
    size_t string_size;
    ثابت حرف *end;
    ثابت حرف *s, *يسار, *يمين;
    حرف *match;
    size_t يسار_len, يمين_len, name_len;

    xlocaledir = get_xlocaledir_path(ctx);

    ret = snprintf(path, حجم(path), "%s/%s", xlocaledir, filename);
    إذا (ret < 0 || (size_t) ret >= حجم(path))
        إرجاع عدم;

    file = fopen(path, "rb");
    إذا (!file)
        إرجاع عدم;

    ok = map_file(file, &string, &string_size);
    fclose(file);
    إذا (!ok)
        إرجاع عدم;

    s = string;
    end = string + string_size;
    name_len = strlen(name);
    match = عدم;

    بينما (s < end) {
        /* Skip spaces. */
        بينما (s < end && is_space(*s))
            s++;

        /* Skip comments. */
        إذا (s < end && *s == '#') {
            بينما (s < end && *s != '\n')
                s++;
            استمر;
        }

        /* Get the left value. */
        يسار = s;
        بينما (s < end && !is_space(*s) && *s != ':')
            s++;
        يسار_len = (size_t)(s - يسار);

        /* There's an optional colon between left and right. */
        إذا (s < end && *s == ':')
            s++;

        /* Skip spaces. */
        بينما (s < end && is_space(*s))
            s++;

        /* Get the right value. */
        يمين = s;
        بينما (s < end && !is_space(*s))
            s++;
        يمين_len = (size_t)(s - يمين);

        /* Discard rest of line. */
        بينما (s < end && *s != '\n')
            s++;

        إذا (direction == يسار_TO_يمين) {
            إذا (يسار_len == name_len && memcmp(يسار, name, يسار_len) == 0) {
                match = strndup(يمين, يمين_len);
                اكسر;
            }
        } وإلا إذا (direction == يمين_TO_يسار &&
                   يمين_len == name_len &&
                   memcmp(يمين, name, يمين_len) == 0) {
            match = strndup(يسار, يسار_len);
            اكسر;
        }
    }

    unmap_file(string, string_size);
    إرجاع match;
}

حرف *
resolve_locale(بنية xkb_context *ctx, ثابت حرف *locale)
{
    حرف *alias = resolve_name(ctx, "locale.alias", يسار_TO_يمين, locale);
    إرجاع alias ? alias : strdup(locale);
}

حرف *
get_xcomposefile_path(بنية xkb_context *ctx)
{
    إرجاع strdup_safe(xkb_context_getenv(ctx, "XCOMPOSEFILE"));
}

حرف *
get_xdg_xcompose_file_path(بنية xkb_context *ctx)
{
    ثابت حرف *xdg_config_home;
    ثابت حرف *home;

    xdg_config_home = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    إذا (!xdg_config_home || !is_absolute_path(xdg_config_home)) {
        home = xkb_context_getenv(ctx, "HOME");
        إذا (!home)
            إرجاع عدم;
        إرجاع asprintf_safe("%s/.config/XCompose", home);
    }

    إرجاع asprintf_safe("%s/XCompose", xdg_config_home);
}

حرف *
get_home_xcompose_file_path(بنية xkb_context *ctx)
{
    ثابت حرف *home;

    home = xkb_context_getenv(ctx, "HOME");
    إذا (!home)
        إرجاع عدم;

    إرجاع asprintf_safe("%s/.XCompose", home);
}

حرف *
get_locale_compose_file_path(بنية xkb_context *ctx, ثابت حرف *locale)
{
    حرف *resolved;
    حرف *path;

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
    ساكن ثابت حرف * ثابت fallback = "en_US.UTF-8";
    ساكن ثابت حرف * ثابت registry = "compose.dir";
    إذا (streq(locale, "C"))
        locale = fallback;

    resolved = resolve_name(ctx, registry, يمين_TO_يسار, locale);
    إذا (!resolved) {
#إذا HAVE_NEWLOCALE
        /*
         * There is no extension mechanism for X11 Compose locales.
         * Instead of failing, we just use `en_US.UTF-8` as a fallback because
         * it is what most locales use anyway.
         * But we still want to fail if the locale is invalid in the system,
         * so that we can catch missing system locales and typos.
         */
        locale_t loc = newlocale(LC_ALL_MASK, locale, (locale_t) 0);
        إذا (loc == (locale_t) 0) {
            log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                    "No Compose file for locale \"%s\": "
                    "locale is either invalid or not installed\n", locale);
            إرجاع عدم;
        } وإلا {
            /*
             * The locale is legit but have no entry in the X11 Compose registry,
             * so use the `en_US.UTF-8` fallback.
             */
            freelocale(loc);
            resolved = resolve_name(ctx, registry, يمين_TO_يسار, fallback);
            إذا (!resolved) {
                log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                        "No Compose file for locale \"%s\": "
                        "failed to use fallback \"%s\"\n", locale, fallback);
                إرجاع عدم;
            } وإلا {
                log_warn(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                         "No Compose file for locale \"%s\": "
                         "using locale fallback \"%s\"\n", locale, fallback);
            }
        }
#وإلا
        إرجاع عدم;
#endif
    }

    إذا (is_absolute_path(resolved)) {
        path = resolved;
    }
    وإلا {
        ثابت حرف *xlocaledir = get_xlocaledir_path(ctx);
        path = asprintf_safe("%s/%s", xlocaledir, resolved);
        حرر(resolved);
    }

    إرجاع path;
}
