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
    کاٻو_منزل_ساڄو,
    ساڄو_منزل_کاٻو,
};

ثابت حرف *
get_xlocaledir_رستو(بنية xkb_context *ctx)
{
    ثابت حرف *dir = xkb_context_getenv(ctx, "XLOCALEDIR");
    إذا (!dir)
        dir = XLOCALEDIR;
    أرجع dir;
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
    حرف رستو[512];
    FILE *file;
    حرف *string;
    سائيز_t string_سائيز;
    ثابت حرف *پڇاڙي;
    ثابت حرف *s, *کاٻو, *ساڄو;
    حرف *ميلاپ;
    سائيز_t کاٻو_len, ساڄو_len, name_len;

    xlocaledir = get_xlocaledir_رستو(ctx);

    ret = snprintf(رستو, حجم(رستو), "%s/%s", xlocaledir, filename);
    إذا (ret < 0 || (سائيز_t) ret >= حجم(رستو))
        أرجع عدم;

    file = fopen(رستو, "rb");
    إذا (!file)
        أرجع عدم;

    ok = map_file(file, &string, &string_سائيز);
    fclose(file);
    إذا (!ok)
        أرجع عدم;

    s = string;
    پڇاڙي = string + string_سائيز;
    name_len = لڙي_ڊيگهه(name);
    ميلاپ = عدم;

    طالما (s < پڇاڙي) {
        /* Skip spaces. */
        طالما (s < پڇاڙي && is_space(*s))
            s++;

        /* Skip comments. */
        إذا (s < پڇاڙي && *s == '#') {
            طالما (s < پڇاڙي && *s != '\n')
                s++;
            استمر;
        }

        /* Get the left value. */
        کاٻو = s;
        طالما (s < پڇاڙي && !is_space(*s) && *s != ':')
            s++;
        کاٻو_len = (سائيز_t)(s - کاٻو);

        /* There's an optional colon between left and right. */
        إذا (s < پڇاڙي && *s == ':')
            s++;

        /* Skip spaces. */
        طالما (s < پڇاڙي && is_space(*s))
            s++;

        /* Get the right value. */
        ساڄو = s;
        طالما (s < پڇاڙي && !is_space(*s))
            s++;
        ساڄو_len = (سائيز_t)(s - ساڄو);

        /* Discard rest of line. */
        طالما (s < پڇاڙي && *s != '\n')
            s++;

        إذا (direction == کاٻو_منزل_ساڄو) {
            إذا (کاٻو_len == name_len && memcmp(کاٻو, name, کاٻو_len) == 0) {
                ميلاپ = strndup(ساڄو, ساڄو_len);
                اكسر;
            }
        } وإلا إذا (direction == ساڄو_منزل_کاٻو &&
                   ساڄو_len == name_len &&
                   memcmp(ساڄو, name, ساڄو_len) == 0) {
            ميلاپ = strndup(کاٻو, کاٻو_len);
            اكسر;
        }
    }

    unmap_file(string, string_سائيز);
    أرجع ميلاپ;
}

حرف *
resolve_locale(بنية xkb_context *ctx, ثابت حرف *locale)
{
    حرف *alias = resolve_name(ctx, "locale.alias", کاٻو_منزل_ساڄو, locale);
    أرجع alias ? alias : strdup(locale);
}

حرف *
get_xcomposefile_رستو(بنية xkb_context *ctx)
{
    أرجع strdup_محفوظ(xkb_context_getenv(ctx, "XCOMPOSEFILE"));
}

حرف *
get_xdg_xcompose_file_رستو(بنية xkb_context *ctx)
{
    ثابت حرف *xdg_config_home;
    ثابت حرف *home;

    xdg_config_home = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    إذا (!xdg_config_home || !is_absolute_رستو(xdg_config_home)) {
        home = xkb_context_getenv(ctx, "HOME");
        إذا (!home)
            أرجع عدم;
        أرجع asprintf_محفوظ("%s/.config/XCompose", home);
    }

    أرجع asprintf_محفوظ("%s/XCompose", xdg_config_home);
}

حرف *
get_home_xcompose_file_رستو(بنية xkb_context *ctx)
{
    ثابت حرف *home;

    home = xkb_context_getenv(ctx, "HOME");
    إذا (!home)
        أرجع عدم;

    أرجع asprintf_محفوظ("%s/.XCompose", home);
}

حرف *
get_locale_compose_file_رستو(بنية xkb_context *ctx, ثابت حرف *locale)
{
    حرف *resolved;
    حرف *رستو;

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

    resolved = resolve_name(ctx, registry, ساڄو_منزل_کاٻو, locale);
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
            أرجع عدم;
        } وإلا {
            /*
             * The locale is legit but have no entry in the X11 Compose registry,
             * so use the `en_US.UTF-8` fallback.
             */
            freelocale(loc);
            resolved = resolve_name(ctx, registry, ساڄو_منزل_کاٻو, fallback);
            إذا (!resolved) {
                log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                        "No Compose file for locale \"%s\": "
                        "failed to use fallback \"%s\"\n", locale, fallback);
                أرجع عدم;
            } وإلا {
                log_warn(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                         "No Compose file for locale \"%s\": "
                         "using locale fallback \"%s\"\n", locale, fallback);
            }
        }
#وإلا
        أرجع عدم;
#جيڪڏهن_پورو
    }

    إذا (is_absolute_رستو(resolved)) {
        رستو = resolved;
    }
    وإلا {
        ثابت حرف *xlocaledir = get_xlocaledir_رستو(ctx);
        رستو = asprintf_محفوظ("%s/%s", xlocaledir, resolved);
        حرر(resolved);
    }

    أرجع رستو;
}
