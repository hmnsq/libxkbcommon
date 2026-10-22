/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل <locale.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "messages-codes.h"
#شامل "utils.h"
#شامل "util-paths.h"
#شامل "context.h"
#شامل "paths.h"

شمار resolve_name_direction {
    بایاں_مقصد_دایاں,
    دایاں_مقصد_بایاں,
};

ثابت حرف *
get_xlocaledir_راستہ(ساخت xkb_context *ctx)
{
    ثابت حرف *dir = xkb_context_getenv(ctx, "XLOCALEDIR");
    اگر (!dir)
        dir = XLOCALEDIR;
    واپس dir;
}

/*
 * Files like compose.dir have the format LEFT: RIGHT.  Lookup @name in
 * such a file and return its matching value, according to @direction.
 * @filename is relative to the xlocaledir.
 */
ساکن حرف *
resolve_name(ساخت xkb_context *ctx, ثابت حرف *filename,
             شمار resolve_name_direction direction, ثابت حرف *name)
{
    صحیح ret;
    bool ok;
    ثابت حرف *xlocaledir;
    حرف راستہ[512];
    FILE *file;
    حرف *string;
    سائز_t string_سائز;
    ثابت حرف *انتہا;
    ثابت حرف *s, *بایاں, *دایاں;
    حرف *مطابقت;
    سائز_t بایاں_len, دایاں_len, name_len;

    xlocaledir = get_xlocaledir_راستہ(ctx);

    ret = snprintf(راستہ, حجم(راستہ), "%s/%s", xlocaledir, filename);
    اگر (ret < 0 || (سائز_t) ret >= حجم(راستہ))
        واپس تہی;

    file = fopen(راستہ, "rb");
    اگر (!file)
        واپس تہی;

    ok = map_file(file, &string, &string_سائز);
    fclose(file);
    اگر (!ok)
        واپس تہی;

    s = string;
    انتہا = string + string_سائز;
    name_len = لڑی_لمبائی(name);
    مطابقت = تہی;

    جب (s < انتہا) {
        /* Skip spaces. */
        جب (s < انتہا && is_space(*s))
            s++;

        /* Skip comments. */
        اگر (s < انتہا && *s == '#') {
            جب (s < انتہا && *s != '\n')
                s++;
            جاری;
        }

        /* Get the left value. */
        بایاں = s;
        جب (s < انتہا && !is_space(*s) && *s != ':')
            s++;
        بایاں_len = (سائز_t)(s - بایاں);

        /* There's an optional colon between left and right. */
        اگر (s < انتہا && *s == ':')
            s++;

        /* Skip spaces. */
        جب (s < انتہا && is_space(*s))
            s++;

        /* Get the right value. */
        دایاں = s;
        جب (s < انتہا && !is_space(*s))
            s++;
        دایاں_len = (سائز_t)(s - دایاں);

        /* Discard rest of line. */
        جب (s < انتہا && *s != '\n')
            s++;

        اگر (direction == بایاں_مقصد_دایاں) {
            اگر (بایاں_len == name_len && memcmp(بایاں, name, بایاں_len) == 0) {
                مطابقت = strndup(دایاں, دایاں_len);
                توڑو;
            }
        } ورنہ اگر (direction == دایاں_مقصد_بایاں &&
                   دایاں_len == name_len &&
                   memcmp(دایاں, name, دایاں_len) == 0) {
            مطابقت = strndup(بایاں, بایاں_len);
            توڑو;
        }
    }

    unmap_file(string, string_سائز);
    واپس مطابقت;
}

حرف *
resolve_locale(ساخت xkb_context *ctx, ثابت حرف *locale)
{
    حرف *alias = resolve_name(ctx, "locale.alias", بایاں_مقصد_دایاں, locale);
    واپس alias ? alias : strdup(locale);
}

حرف *
get_xcomposefile_راستہ(ساخت xkb_context *ctx)
{
    واپس strdup_محفوظ(xkb_context_getenv(ctx, "XCOMPOSEFILE"));
}

حرف *
get_xdg_xcompose_file_راستہ(ساخت xkb_context *ctx)
{
    ثابت حرف *xdg_config_home;
    ثابت حرف *home;

    xdg_config_home = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    اگر (!xdg_config_home || !is_absolute_راستہ(xdg_config_home)) {
        home = xkb_context_getenv(ctx, "HOME");
        اگر (!home)
            واپس تہی;
        واپس asprintf_محفوظ("%s/.config/XCompose", home);
    }

    واپس asprintf_محفوظ("%s/XCompose", xdg_config_home);
}

حرف *
get_home_xcompose_file_راستہ(ساخت xkb_context *ctx)
{
    ثابت حرف *home;

    home = xkb_context_getenv(ctx, "HOME");
    اگر (!home)
        واپس تہی;

    واپس asprintf_محفوظ("%s/.XCompose", home);
}

حرف *
get_locale_compose_file_راستہ(ساخت xkb_context *ctx, ثابت حرف *locale)
{
    حرف *resolved;
    حرف *راستہ;

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
    ساکن ثابت حرف * ثابت fallback = "en_US.UTF-8";
    ساکن ثابت حرف * ثابت registry = "compose.dir";
    اگر (streq(locale, "C"))
        locale = fallback;

    resolved = resolve_name(ctx, registry, دایاں_مقصد_بایاں, locale);
    اگر (!resolved) {
#اگر HAVE_NEWLOCALE
        /*
         * There is no extension mechanism for X11 Compose locales.
         * Instead of failing, we just use `en_US.UTF-8` as a fallback because
         * it is what most locales use anyway.
         * But we still want to fail if the locale is invalid in the system,
         * so that we can catch missing system locales and typos.
         */
        locale_t loc = newlocale(LC_ALL_MASK, locale, (locale_t) 0);
        اگر (loc == (locale_t) 0) {
            log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                    "No Compose file for locale \"%s\": "
                    "locale is either invalid or not installed\n", locale);
            واپس تہی;
        } ورنہ {
            /*
             * The locale is legit but have no entry in the X11 Compose registry,
             * so use the `en_US.UTF-8` fallback.
             */
            freelocale(loc);
            resolved = resolve_name(ctx, registry, دایاں_مقصد_بایاں, fallback);
            اگر (!resolved) {
                log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                        "No Compose file for locale \"%s\": "
                        "failed to use fallback \"%s\"\n", locale, fallback);
                واپس تہی;
            } ورنہ {
                log_warn(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                         "No Compose file for locale \"%s\": "
                         "using locale fallback \"%s\"\n", locale, fallback);
            }
        }
#ورنہ
        واپس تہی;
#اگر_ختم
    }

    اگر (is_absolute_راستہ(resolved)) {
        راستہ = resolved;
    }
    ورنہ {
        ثابت حرف *xlocaledir = get_xlocaledir_راستہ(ctx);
        راستہ = asprintf_محفوظ("%s/%s", xlocaledir, resolved);
        آزاد(resolved);
    }

    واپس راستہ;
}
