/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <locale.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "messages-codes.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-paths.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "context.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "paths.h"

ਗਿਣਤੀ resolve_name_direction {
    ਖੱਬਾ_ਮੰਜ਼ਿਲ_ਸੱਜਾ,
    ਸੱਜਾ_ਮੰਜ਼ਿਲ_ਖੱਬਾ,
};

ਅਚਲ ਅੱਖਰ *
get_xlocaledir_ਰਸਤਾ(ਬਣਤਰ xkb_context *ctx)
{
    ਅਚਲ ਅੱਖਰ *dir = xkb_context_getenv(ctx, "XLOCALEDIR");
    ਜੇ (!dir)
        dir = XLOCALEDIR;
    ਵਾਪਸ dir;
}

/*
 * Files like compose.dir have the format LEFT: RIGHT.  Lookup @name in
 * such a file and return its matching value, according to @direction.
 * @filename is relative to the xlocaledir.
 */
ਸਥਿਰ ਅੱਖਰ *
resolve_name(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ *filename,
             ਗਿਣਤੀ resolve_name_direction direction, ਅਚਲ ਅੱਖਰ *name)
{
    ਪੂਰਨ ret;
    bool ok;
    ਅਚਲ ਅੱਖਰ *xlocaledir;
    ਅੱਖਰ ਰਸਤਾ[512];
    FILE *file;
    ਅੱਖਰ *string;
    ਮਾਪ_t string_ਮਾਪ;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ;
    ਅਚਲ ਅੱਖਰ *s, *ਖੱਬਾ, *ਸੱਜਾ;
    ਅੱਖਰ *ਮੇਲ;
    ਮਾਪ_t ਖੱਬਾ_len, ਸੱਜਾ_len, name_len;

    xlocaledir = get_xlocaledir_ਰਸਤਾ(ctx);

    ret = snprintf(ਰਸਤਾ, ਆਕਾਰ(ਰਸਤਾ), "%s/%s", xlocaledir, filename);
    ਜੇ (ret < 0 || (ਮਾਪ_t) ret >= ਆਕਾਰ(ਰਸਤਾ))
        ਵਾਪਸ ਨਿਰਾ;

    file = fopen(ਰਸਤਾ, "rb");
    ਜੇ (!file)
        ਵਾਪਸ ਨਿਰਾ;

    ok = map_file(file, &string, &string_ਮਾਪ);
    fclose(file);
    ਜੇ (!ok)
        ਵਾਪਸ ਨਿਰਾ;

    s = string;
    ਸਿਰਾ = string + string_ਮਾਪ;
    name_len = ਲੜੀ_ਲੰਬਾਈ(name);
    ਮੇਲ = ਨਿਰਾ;

    ਜਦੋਂ (s < ਸਿਰਾ) {
        /* Skip spaces. */
        ਜਦੋਂ (s < ਸਿਰਾ && is_space(*s))
            s++;

        /* Skip comments. */
        ਜੇ (s < ਸਿਰਾ && *s == '#') {
            ਜਦੋਂ (s < ਸਿਰਾ && *s != '\n')
                s++;
            ਜਾਰੀ;
        }

        /* Get the left value. */
        ਖੱਬਾ = s;
        ਜਦੋਂ (s < ਸਿਰਾ && !is_space(*s) && *s != ':')
            s++;
        ਖੱਬਾ_len = (ਮਾਪ_t)(s - ਖੱਬਾ);

        /* There's an optional colon between left and right. */
        ਜੇ (s < ਸਿਰਾ && *s == ':')
            s++;

        /* Skip spaces. */
        ਜਦੋਂ (s < ਸਿਰਾ && is_space(*s))
            s++;

        /* Get the right value. */
        ਸੱਜਾ = s;
        ਜਦੋਂ (s < ਸਿਰਾ && !is_space(*s))
            s++;
        ਸੱਜਾ_len = (ਮਾਪ_t)(s - ਸੱਜਾ);

        /* Discard rest of line. */
        ਜਦੋਂ (s < ਸਿਰਾ && *s != '\n')
            s++;

        ਜੇ (direction == ਖੱਬਾ_ਮੰਜ਼ਿਲ_ਸੱਜਾ) {
            ਜੇ (ਖੱਬਾ_len == name_len && memcmp(ਖੱਬਾ, name, ਖੱਬਾ_len) == 0) {
                ਮੇਲ = strndup(ਸੱਜਾ, ਸੱਜਾ_len);
                ਰੋਕੋ;
            }
        } ਨਹੀਂਤਾਂ ਜੇ (direction == ਸੱਜਾ_ਮੰਜ਼ਿਲ_ਖੱਬਾ &&
                   ਸੱਜਾ_len == name_len &&
                   memcmp(ਸੱਜਾ, name, ਸੱਜਾ_len) == 0) {
            ਮੇਲ = strndup(ਖੱਬਾ, ਖੱਬਾ_len);
            ਰੋਕੋ;
        }
    }

    unmap_file(string, string_ਮਾਪ);
    ਵਾਪਸ ਮੇਲ;
}

ਅੱਖਰ *
resolve_locale(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ *locale)
{
    ਅੱਖਰ *alias = resolve_name(ctx, "locale.alias", ਖੱਬਾ_ਮੰਜ਼ਿਲ_ਸੱਜਾ, locale);
    ਵਾਪਸ alias ? alias : strdup(locale);
}

ਅੱਖਰ *
get_xcomposefile_ਰਸਤਾ(ਬਣਤਰ xkb_context *ctx)
{
    ਵਾਪਸ strdup_ਸੁਰੱਖਿਅਤ(xkb_context_getenv(ctx, "XCOMPOSEFILE"));
}

ਅੱਖਰ *
get_xdg_xcompose_file_ਰਸਤਾ(ਬਣਤਰ xkb_context *ctx)
{
    ਅਚਲ ਅੱਖਰ *xdg_config_home;
    ਅਚਲ ਅੱਖਰ *home;

    xdg_config_home = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    ਜੇ (!xdg_config_home || !is_absolute_ਰਸਤਾ(xdg_config_home)) {
        home = xkb_context_getenv(ctx, "HOME");
        ਜੇ (!home)
            ਵਾਪਸ ਨਿਰਾ;
        ਵਾਪਸ asprintf_ਸੁਰੱਖਿਅਤ("%s/.config/XCompose", home);
    }

    ਵਾਪਸ asprintf_ਸੁਰੱਖਿਅਤ("%s/XCompose", xdg_config_home);
}

ਅੱਖਰ *
get_home_xcompose_file_ਰਸਤਾ(ਬਣਤਰ xkb_context *ctx)
{
    ਅਚਲ ਅੱਖਰ *home;

    home = xkb_context_getenv(ctx, "HOME");
    ਜੇ (!home)
        ਵਾਪਸ ਨਿਰਾ;

    ਵਾਪਸ asprintf_ਸੁਰੱਖਿਅਤ("%s/.XCompose", home);
}

ਅੱਖਰ *
get_locale_compose_file_ਰਸਤਾ(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ *locale)
{
    ਅੱਖਰ *resolved;
    ਅੱਖਰ *ਰਸਤਾ;

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
    ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ fallback = "en_US.UTF-8";
    ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ registry = "compose.dir";
    ਜੇ (streq(locale, "C"))
        locale = fallback;

    resolved = resolve_name(ctx, registry, ਸੱਜਾ_ਮੰਜ਼ਿਲ_ਖੱਬਾ, locale);
    ਜੇ (!resolved) {
#ਜੇ HAVE_NEWLOCALE
        /*
         * There is no extension mechanism for X11 Compose locales.
         * Instead of failing, we just use `en_US.UTF-8` as a fallback because
         * it is what most locales use anyway.
         * But we still want to fail if the locale is invalid in the system,
         * so that we can catch missing system locales and typos.
         */
        locale_t loc = newlocale(LC_ALL_MASK, locale, (locale_t) 0);
        ਜੇ (loc == (locale_t) 0) {
            log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                    "No Compose file for locale \"%s\": "
                    "locale is either invalid or not installed\n", locale);
            ਵਾਪਸ ਨਿਰਾ;
        } ਨਹੀਂਤਾਂ {
            /*
             * The locale is legit but have no entry in the X11 Compose registry,
             * so use the `en_US.UTF-8` fallback.
             */
            freelocale(loc);
            resolved = resolve_name(ctx, registry, ਸੱਜਾ_ਮੰਜ਼ਿਲ_ਖੱਬਾ, fallback);
            ਜੇ (!resolved) {
                log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                        "No Compose file for locale \"%s\": "
                        "failed to use fallback \"%s\"\n", locale, fallback);
                ਵਾਪਸ ਨਿਰਾ;
            } ਨਹੀਂਤਾਂ {
                log_warn(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                         "No Compose file for locale \"%s\": "
                         "using locale fallback \"%s\"\n", locale, fallback);
            }
        }
#ਨਹੀਂਤਾਂ
        ਵਾਪਸ ਨਿਰਾ;
#ਜੇ_ਅੰਤ
    }

    ਜੇ (is_absolute_ਰਸਤਾ(resolved)) {
        ਰਸਤਾ = resolved;
    }
    ਨਹੀਂਤਾਂ {
        ਅਚਲ ਅੱਖਰ *xlocaledir = get_xlocaledir_ਰਸਤਾ(ctx);
        ਰਸਤਾ = asprintf_ਸੁਰੱਖਿਅਤ("%s/%s", xlocaledir, resolved);
        ਮੁਕਤ(resolved);
    }

    ਵਾਪਸ ਰਸਤਾ;
}
