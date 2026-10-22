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

ಎಣಿಕೆ resolve_name_direction {
    ಎಡ_ಗಮ್ಯ_ಬಲ,
    ಬಲ_ಗಮ್ಯ_ಎಡ,
};

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
get_xlocaledir_ಮಾರ್ಗ(ರಚನೆ xkb_context *ctx)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dir = xkb_context_getenv(ctx, "XLOCALEDIR");
    ಆದರೆ (!dir)
        dir = XLOCALEDIR;
    ಹಿಂತಿರುಗಿ dir;
}

/*
 * Files like compose.dir have the format LEFT: RIGHT.  Lookup @name in
 * such a file and return its matching value, according to @direction.
 * @filename is relative to the xlocaledir.
 */
ಸ್ಥಿರ ಅಕ್ಷರ *
resolve_name(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename,
             ಎಣಿಕೆ resolve_name_direction direction, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
    ಪೂರ್ಣಾಂಕ ret;
    bool ok;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *xlocaledir;
    ಅಕ್ಷರ ಮಾರ್ಗ[512];
    FILE *file;
    ಅಕ್ಷರ *string;
    ಗಾತ್ರ_t string_ಗಾತ್ರ;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೊನೆ;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, *ಎಡ, *ಬಲ;
    ಅಕ್ಷರ *ಹೊಂದಿಕೆ;
    ಗಾತ್ರ_t ಎಡ_len, ಬಲ_len, name_len;

    xlocaledir = get_xlocaledir_ಮಾರ್ಗ(ctx);

    ret = snprintf(ಮಾರ್ಗ, ಗಾತ್ರ(ಮಾರ್ಗ), "%s/%s", xlocaledir, filename);
    ಆದರೆ (ret < 0 || (ಗಾತ್ರ_t) ret >= ಗಾತ್ರ(ಮಾರ್ಗ))
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    file = fopen(ಮಾರ್ಗ, "rb");
    ಆದರೆ (!file)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ok = map_file(file, &string, &string_ಗಾತ್ರ);
    fclose(file);
    ಆದರೆ (!ok)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    s = string;
    ಕೊನೆ = string + string_ಗಾತ್ರ;
    name_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name);
    ಹೊಂದಿಕೆ = ಖಾಲಿ;

    ಆಗ (s < ಕೊನೆ) {
        /* Skip spaces. */
        ಆಗ (s < ಕೊನೆ && is_space(*s))
            s++;

        /* Skip comments. */
        ಆದರೆ (s < ಕೊನೆ && *s == '#') {
            ಆಗ (s < ಕೊನೆ && *s != '\n')
                s++;
            ಮುಂದುವರಿ;
        }

        /* Get the left value. */
        ಎಡ = s;
        ಆಗ (s < ಕೊನೆ && !is_space(*s) && *s != ':')
            s++;
        ಎಡ_len = (ಗಾತ್ರ_t)(s - ಎಡ);

        /* There's an optional colon between left and right. */
        ಆದರೆ (s < ಕೊನೆ && *s == ':')
            s++;

        /* Skip spaces. */
        ಆಗ (s < ಕೊನೆ && is_space(*s))
            s++;

        /* Get the right value. */
        ಬಲ = s;
        ಆಗ (s < ಕೊನೆ && !is_space(*s))
            s++;
        ಬಲ_len = (ಗಾತ್ರ_t)(s - ಬಲ);

        /* Discard rest of line. */
        ಆಗ (s < ಕೊನೆ && *s != '\n')
            s++;

        ಆದರೆ (direction == ಎಡ_ಗಮ್ಯ_ಬಲ) {
            ಆದರೆ (ಎಡ_len == name_len && memcmp(ಎಡ, name, ಎಡ_len) == 0) {
                ಹೊಂದಿಕೆ = strndup(ಬಲ, ಬಲ_len);
                ನಿಲ್ಲಿಸು;
            }
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (direction == ಬಲ_ಗಮ್ಯ_ಎಡ &&
                   ಬಲ_len == name_len &&
                   memcmp(ಬಲ, name, ಬಲ_len) == 0) {
            ಹೊಂದಿಕೆ = strndup(ಎಡ, ಎಡ_len);
            ನಿಲ್ಲಿಸು;
        }
    }

    unmap_file(string, string_ಗಾತ್ರ);
    ಹಿಂತಿರುಗಿ ಹೊಂದಿಕೆ;
}

ಅಕ್ಷರ *
resolve_locale(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *locale)
{
    ಅಕ್ಷರ *alias = resolve_name(ctx, "locale.alias", ಎಡ_ಗಮ್ಯ_ಬಲ, locale);
    ಹಿಂತಿರುಗಿ alias ? alias : strdup(locale);
}

ಅಕ್ಷರ *
get_xcomposefile_ಮಾರ್ಗ(ರಚನೆ xkb_context *ctx)
{
    ಹಿಂತಿರುಗಿ strdup_ಸುರಕ್ಷಿತ(xkb_context_getenv(ctx, "XCOMPOSEFILE"));
}

ಅಕ್ಷರ *
get_xdg_xcompose_file_ಮಾರ್ಗ(ರಚನೆ xkb_context *ctx)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *xdg_config_home;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *home;

    xdg_config_home = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    ಆದರೆ (!xdg_config_home || !is_absolute_ಮಾರ್ಗ(xdg_config_home)) {
        home = xkb_context_getenv(ctx, "HOME");
        ಆದರೆ (!home)
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        ಹಿಂತಿರುಗಿ asprintf_ಸುರಕ್ಷಿತ("%s/.config/XCompose", home);
    }

    ಹಿಂತಿರುಗಿ asprintf_ಸುರಕ್ಷಿತ("%s/XCompose", xdg_config_home);
}

ಅಕ್ಷರ *
get_home_xcompose_file_ಮಾರ್ಗ(ರಚನೆ xkb_context *ctx)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *home;

    home = xkb_context_getenv(ctx, "HOME");
    ಆದರೆ (!home)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ asprintf_ಸುರಕ್ಷಿತ("%s/.XCompose", home);
}

ಅಕ್ಷರ *
get_locale_compose_file_ಮಾರ್ಗ(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *locale)
{
    ಅಕ್ಷರ *resolved;
    ಅಕ್ಷರ *ಮಾರ್ಗ;

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
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ fallback = "en_US.UTF-8";
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ registry = "compose.dir";
    ಆದರೆ (streq(locale, "C"))
        locale = fallback;

    resolved = resolve_name(ctx, registry, ಬಲ_ಗಮ್ಯ_ಎಡ, locale);
    ಆದರೆ (!resolved) {
#ಆದರೆ HAVE_NEWLOCALE
        /*
         * There is no extension mechanism for X11 Compose locales.
         * Instead of failing, we just use `en_US.UTF-8` as a fallback because
         * it is what most locales use anyway.
         * But we still want to fail if the locale is invalid in the system,
         * so that we can catch missing system locales and typos.
         */
        locale_t loc = newlocale(LC_ALL_MASK, locale, (locale_t) 0);
        ಆದರೆ (loc == (locale_t) 0) {
            log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                    "No Compose file for locale \"%s\": "
                    "locale is either invalid or not installed\n", locale);
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        } ಇಲ್ಲದಿದ್ದರೆ {
            /*
             * The locale is legit but have no entry in the X11 Compose registry,
             * so use the `en_US.UTF-8` fallback.
             */
            freelocale(loc);
            resolved = resolve_name(ctx, registry, ಬಲ_ಗಮ್ಯ_ಎಡ, fallback);
            ಆದರೆ (!resolved) {
                log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                        "No Compose file for locale \"%s\": "
                        "failed to use fallback \"%s\"\n", locale, fallback);
                ಹಿಂತಿರುಗಿ ಖಾಲಿ;
            } ಇಲ್ಲದಿದ್ದರೆ {
                log_warn(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                         "No Compose file for locale \"%s\": "
                         "using locale fallback \"%s\"\n", locale, fallback);
            }
        }
#ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ
    }

    ಆದರೆ (is_absolute_ಮಾರ್ಗ(resolved)) {
        ಮಾರ್ಗ = resolved;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *xlocaledir = get_xlocaledir_ಮಾರ್ಗ(ctx);
        ಮಾರ್ಗ = asprintf_ಸುರಕ್ಷಿತ("%s/%s", xlocaledir, resolved);
        ಬಿಡುಗಡೆ(resolved);
    }

    ಹಿಂತಿರುಗಿ ಮಾರ್ಗ;
}
