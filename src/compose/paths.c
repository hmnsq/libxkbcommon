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
    بایاں_منزل_دایاں,
    دایاں_منزل_بایاں,
};

ثابت حرف *
get_xlocaledir_رستہ(بنية xkb_context *ctx)
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
    حرف رستہ[512];
    FILE *file;
    حرف *string;
    ناپ_t string_ناپ;
    ثابت حرف *سرا;
    ثابت حرف *s, *بایاں, *دایاں;
    حرف *میل;
    ناپ_t بایاں_len, دایاں_len, name_len;

    xlocaledir = get_xlocaledir_رستہ(ctx);

    ret = snprintf(رستہ, حجم(رستہ), "%s/%s", xlocaledir, filename);
    إذا (ret < 0 || (ناپ_t) ret >= حجم(رستہ))
        أرجع عدم;

    file = fopen(رستہ, "rb");
    إذا (!file)
        أرجع عدم;

    ok = map_file(file, &string, &string_ناپ);
    fclose(file);
    إذا (!ok)
        أرجع عدم;

    s = string;
    سرا = string + string_ناپ;
    name_len = لڑی_لمبائی(name);
    میل = عدم;

    طالما (s < سرا) {
        /* Skip spaces. */
        طالما (s < سرا && is_space(*s))
            s++;

        /* Skip comments. */
        إذا (s < سرا && *s == '#') {
            طالما (s < سرا && *s != '\n')
                s++;
            استمر;
        }

        /* Get the left value. */
        بایاں = s;
        طالما (s < سرا && !is_space(*s) && *s != ':')
            s++;
        بایاں_len = (ناپ_t)(s - بایاں);

        /* There's an optional colon between left and right. */
        إذا (s < سرا && *s == ':')
            s++;

        /* Skip spaces. */
        طالما (s < سرا && is_space(*s))
            s++;

        /* Get the right value. */
        دایاں = s;
        طالما (s < سرا && !is_space(*s))
            s++;
        دایاں_len = (ناپ_t)(s - دایاں);

        /* Discard rest of line. */
        طالما (s < سرا && *s != '\n')
            s++;

        إذا (direction == بایاں_منزل_دایاں) {
            إذا (بایاں_len == name_len && memcmp(بایاں, name, بایاں_len) == 0) {
                میل = strndup(دایاں, دایاں_len);
                اكسر;
            }
        } وإلا إذا (direction == دایاں_منزل_بایاں &&
                   دایاں_len == name_len &&
                   memcmp(دایاں, name, دایاں_len) == 0) {
            میل = strndup(بایاں, بایاں_len);
            اكسر;
        }
    }

    unmap_file(string, string_ناپ);
    أرجع میل;
}

حرف *
resolve_locale(بنية xkb_context *ctx, ثابت حرف *locale)
{
    حرف *alias = resolve_name(ctx, "locale.alias", بایاں_منزل_دایاں, locale);
    أرجع alias ? alias : strdup(locale);
}

حرف *
get_xcomposefile_رستہ(بنية xkb_context *ctx)
{
    أرجع strdup_محفوظ(xkb_context_getenv(ctx, "XCOMPOSEFILE"));
}

حرف *
get_xdg_xcompose_file_رستہ(بنية xkb_context *ctx)
{
    ثابت حرف *xdg_config_home;
    ثابت حرف *home;

    xdg_config_home = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    إذا (!xdg_config_home || !is_absolute_رستہ(xdg_config_home)) {
        home = xkb_context_getenv(ctx, "HOME");
        إذا (!home)
            أرجع عدم;
        أرجع asprintf_محفوظ("%s/.config/XCompose", home);
    }

    أرجع asprintf_محفوظ("%s/XCompose", xdg_config_home);
}

حرف *
get_home_xcompose_file_رستہ(بنية xkb_context *ctx)
{
    ثابت حرف *home;

    home = xkb_context_getenv(ctx, "HOME");
    إذا (!home)
        أرجع عدم;

    أرجع asprintf_محفوظ("%s/.XCompose", home);
}

حرف *
get_locale_compose_file_رستہ(بنية xkb_context *ctx, ثابت حرف *locale)
{
    حرف *resolved;
    حرف *رستہ;

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

    resolved = resolve_name(ctx, registry, دایاں_منزل_بایاں, locale);
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
            resolved = resolve_name(ctx, registry, دایاں_منزل_بایاں, fallback);
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
#جے_انت
    }

    إذا (is_absolute_رستہ(resolved)) {
        رستہ = resolved;
    }
    وإلا {
        ثابت حرف *xlocaledir = get_xlocaledir_رستہ(ctx);
        رستہ = asprintf_محفوظ("%s/%s", xlocaledir, resolved);
        حرر(resolved);
    }

    أرجع رستہ;
}
