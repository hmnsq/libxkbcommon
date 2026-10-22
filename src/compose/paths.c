/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <locale.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "messages-codes.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "util-paths.h"
#ಅನ್ತರ್ಭಾವ "context.h"
#ಅನ್ತರ್ಭಾವ "paths.h"

ಗಣನಾ resolve_name_direction {
    ಎಡ_TO_ಬಲ,
    ಬಲ_TO_ಎಡ,
};

ಧ್ರುವ ವರ್ಣ *
get_xlocaledir_path(ಸಂರಚನಾ xkb_context *ctx)
{
    ಧ್ರುವ ವರ್ಣ *dir = xkb_context_getenv(ctx, "XLOCALEDIR");
    ಯದಿ (!dir)
        dir = XLOCALEDIR;
    ಪ್ರತ್ಯಾಗಮ dir;
}

/*
 * Files like compose.dir have the format LEFT: RIGHT.  Lookup @name in
 * such a file and return its matching value, according to @direction.
 * @filename is relative to the xlocaledir.
 */
ಸ್ಥಿರ ವರ್ಣ *
resolve_name(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ವರ್ಣ *filename,
             ಗಣನಾ resolve_name_direction direction, ಧ್ರುವ ವರ್ಣ *name)
{
    ಪೂರ್ಣಾಙ್ಕ ret;
    bool ok;
    ಧ್ರುವ ವರ್ಣ *xlocaledir;
    ವರ್ಣ path[512];
    FILE *file;
    ವರ್ಣ *string;
    size_t string_size;
    ಧ್ರುವ ವರ್ಣ *end;
    ಧ್ರುವ ವರ್ಣ *s, *ಎಡ, *ಬಲ;
    ವರ್ಣ *match;
    size_t ಎಡ_len, ಬಲ_len, name_len;

    xlocaledir = get_xlocaledir_path(ctx);

    ret = snprintf(path, ಪರಿಮಾಣ(path), "%s/%s", xlocaledir, filename);
    ಯದಿ (ret < 0 || (size_t) ret >= ಪರಿಮಾಣ(path))
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    file = fopen(path, "rb");
    ಯದಿ (!file)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ok = map_file(file, &string, &string_size);
    fclose(file);
    ಯದಿ (!ok)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    s = string;
    end = string + string_size;
    name_len = strlen(name);
    match = ರಿಕ್ತ;

    ಯಾವತ್ (s < end) {
        /* Skip spaces. */
        ಯಾವತ್ (s < end && is_space(*s))
            s++;

        /* Skip comments. */
        ಯದಿ (s < end && *s == '#') {
            ಯಾವತ್ (s < end && *s != '\n')
                s++;
            ಅನುವರ್ತ;
        }

        /* Get the left value. */
        ಎಡ = s;
        ಯಾವತ್ (s < end && !is_space(*s) && *s != ':')
            s++;
        ಎಡ_len = (size_t)(s - ಎಡ);

        /* There's an optional colon between left and right. */
        ಯದಿ (s < end && *s == ':')
            s++;

        /* Skip spaces. */
        ಯಾವತ್ (s < end && is_space(*s))
            s++;

        /* Get the right value. */
        ಬಲ = s;
        ಯಾವತ್ (s < end && !is_space(*s))
            s++;
        ಬಲ_len = (size_t)(s - ಬಲ);

        /* Discard rest of line. */
        ಯಾವತ್ (s < end && *s != '\n')
            s++;

        ಯದಿ (direction == ಎಡ_TO_ಬಲ) {
            ಯದಿ (ಎಡ_len == name_len && memcmp(ಎಡ, name, ಎಡ_len) == 0) {
                match = strndup(ಬಲ, ಬಲ_len);
                ಭಙ್ಗ;
            }
        } ಅನ್ಯಥಾ ಯದಿ (direction == ಬಲ_TO_ಎಡ &&
                   ಬಲ_len == name_len &&
                   memcmp(ಬಲ, name, ಬಲ_len) == 0) {
            match = strndup(ಎಡ, ಎಡ_len);
            ಭಙ್ಗ;
        }
    }

    unmap_file(string, string_size);
    ಪ್ರತ್ಯಾಗಮ match;
}

ವರ್ಣ *
resolve_locale(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ವರ್ಣ *locale)
{
    ವರ್ಣ *alias = resolve_name(ctx, "locale.alias", ಎಡ_TO_ಬಲ, locale);
    ಪ್ರತ್ಯಾಗಮ alias ? alias : strdup(locale);
}

ವರ್ಣ *
get_xcomposefile_path(ಸಂರಚನಾ xkb_context *ctx)
{
    ಪ್ರತ್ಯಾಗಮ strdup_safe(xkb_context_getenv(ctx, "XCOMPOSEFILE"));
}

ವರ್ಣ *
get_xdg_xcompose_file_path(ಸಂರಚನಾ xkb_context *ctx)
{
    ಧ್ರುವ ವರ್ಣ *xdg_config_home;
    ಧ್ರುವ ವರ್ಣ *home;

    xdg_config_home = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    ಯದಿ (!xdg_config_home || !is_absolute_path(xdg_config_home)) {
        home = xkb_context_getenv(ctx, "HOME");
        ಯದಿ (!home)
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        ಪ್ರತ್ಯಾಗಮ asprintf_safe("%s/.config/XCompose", home);
    }

    ಪ್ರತ್ಯಾಗಮ asprintf_safe("%s/XCompose", xdg_config_home);
}

ವರ್ಣ *
get_home_xcompose_file_path(ಸಂರಚನಾ xkb_context *ctx)
{
    ಧ್ರುವ ವರ್ಣ *home;

    home = xkb_context_getenv(ctx, "HOME");
    ಯದಿ (!home)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ asprintf_safe("%s/.XCompose", home);
}

ವರ್ಣ *
get_locale_compose_file_path(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ವರ್ಣ *locale)
{
    ವರ್ಣ *resolved;
    ವರ್ಣ *path;

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
    ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ fallback = "en_US.UTF-8";
    ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ registry = "compose.dir";
    ಯದಿ (streq(locale, "C"))
        locale = fallback;

    resolved = resolve_name(ctx, registry, ಬಲ_TO_ಎಡ, locale);
    ಯದಿ (!resolved) {
#ಯದಿ HAVE_NEWLOCALE
        /*
         * There is no extension mechanism for X11 Compose locales.
         * Instead of failing, we just use `en_US.UTF-8` as a fallback because
         * it is what most locales use anyway.
         * But we still want to fail if the locale is invalid in the system,
         * so that we can catch missing system locales and typos.
         */
        locale_t loc = newlocale(LC_ALL_MASK, locale, (locale_t) 0);
        ಯದಿ (loc == (locale_t) 0) {
            log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                    "No Compose file for locale \"%s\": "
                    "locale is either invalid or not installed\n", locale);
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        } ಅನ್ಯಥಾ {
            /*
             * The locale is legit but have no entry in the X11 Compose registry,
             * so use the `en_US.UTF-8` fallback.
             */
            freelocale(loc);
            resolved = resolve_name(ctx, registry, ಬಲ_TO_ಎಡ, fallback);
            ಯದಿ (!resolved) {
                log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                        "No Compose file for locale \"%s\": "
                        "failed to use fallback \"%s\"\n", locale, fallback);
                ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
            } ಅನ್ಯಥಾ {
                log_warn(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                         "No Compose file for locale \"%s\": "
                         "using locale fallback \"%s\"\n", locale, fallback);
            }
        }
#ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
#endif
    }

    ಯದಿ (is_absolute_path(resolved)) {
        path = resolved;
    }
    ಅನ್ಯಥಾ {
        ಧ್ರುವ ವರ್ಣ *xlocaledir = get_xlocaledir_path(ctx);
        path = asprintf_safe("%s/%s", xlocaledir, resolved);
        ಮುಕ್ತ(resolved);
    }

    ಪ್ರತ್ಯಾಗಮ path;
}
