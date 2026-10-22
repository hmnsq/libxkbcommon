/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <locale.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "messages-codes.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "util-paths.h"
#അന്തര്ഭാവ "context.h"
#അന്തര്ഭാവ "paths.h"

എണ്ണൽ resolve_name_direction {
    ഇടത്_TO_വലത്,
    വലത്_TO_ഇടത്,
};

സ്ഥിരം അക്ഷരം *
get_xlocaledir_path(ഘടന xkb_context *ctx)
{
    സ്ഥിരം അക്ഷരം *dir = xkb_context_getenv(ctx, "XLOCALEDIR");
    എങ്കിൽ (!dir)
        dir = XLOCALEDIR;
    തിരികെ dir;
}

/*
 * Files like compose.dir have the format LEFT: RIGHT.  Lookup @name in
 * such a file and return its matching value, according to @direction.
 * @filename is relative to the xlocaledir.
 */
സ്ഥിതം അക്ഷരം *
resolve_name(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം *filename,
             എണ്ണൽ resolve_name_direction direction, സ്ഥിരം അക്ഷരം *name)
{
    പൂർണ്ണസംഖ്യ ret;
    bool ok;
    സ്ഥിരം അക്ഷരം *xlocaledir;
    അക്ഷരം path[512];
    FILE *file;
    അക്ഷരം *string;
    size_t string_size;
    സ്ഥിരം അക്ഷരം *end;
    സ്ഥിരം അക്ഷരം *s, *ഇടത്, *വലത്;
    അക്ഷരം *match;
    size_t ഇടത്_len, വലത്_len, name_len;

    xlocaledir = get_xlocaledir_path(ctx);

    ret = snprintf(path, വലുപ്പം(path), "%s/%s", xlocaledir, filename);
    എങ്കിൽ (ret < 0 || (size_t) ret >= വലുപ്പം(path))
        തിരികെ ശൂന്യ;

    file = fopen(path, "rb");
    എങ്കിൽ (!file)
        തിരികെ ശൂന്യ;

    ok = map_file(file, &string, &string_size);
    fclose(file);
    എങ്കിൽ (!ok)
        തിരികെ ശൂന്യ;

    s = string;
    end = string + string_size;
    name_len = strlen(name);
    match = ശൂന്യ;

    ആകുമ്പോൾ (s < end) {
        /* Skip spaces. */
        ആകുമ്പോൾ (s < end && is_space(*s))
            s++;

        /* Skip comments. */
        എങ്കിൽ (s < end && *s == '#') {
            ആകുമ്പോൾ (s < end && *s != '\n')
                s++;
            തുടരുക;
        }

        /* Get the left value. */
        ഇടത് = s;
        ആകുമ്പോൾ (s < end && !is_space(*s) && *s != ':')
            s++;
        ഇടത്_len = (size_t)(s - ഇടത്);

        /* There's an optional colon between left and right. */
        എങ്കിൽ (s < end && *s == ':')
            s++;

        /* Skip spaces. */
        ആകുമ്പോൾ (s < end && is_space(*s))
            s++;

        /* Get the right value. */
        വലത് = s;
        ആകുമ്പോൾ (s < end && !is_space(*s))
            s++;
        വലത്_len = (size_t)(s - വലത്);

        /* Discard rest of line. */
        ആകുമ്പോൾ (s < end && *s != '\n')
            s++;

        എങ്കിൽ (direction == ഇടത്_TO_വലത്) {
            എങ്കിൽ (ഇടത്_len == name_len && memcmp(ഇടത്, name, ഇടത്_len) == 0) {
                match = strndup(വലത്, വലത്_len);
                നിർത്തുക;
            }
        } അല്ലെങ്കിൽ എങ്കിൽ (direction == വലത്_TO_ഇടത് &&
                   വലത്_len == name_len &&
                   memcmp(വലത്, name, വലത്_len) == 0) {
            match = strndup(ഇടത്, ഇടത്_len);
            നിർത്തുക;
        }
    }

    unmap_file(string, string_size);
    തിരികെ match;
}

അക്ഷരം *
resolve_locale(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം *locale)
{
    അക്ഷരം *alias = resolve_name(ctx, "locale.alias", ഇടത്_TO_വലത്, locale);
    തിരികെ alias ? alias : strdup(locale);
}

അക്ഷരം *
get_xcomposefile_path(ഘടന xkb_context *ctx)
{
    തിരികെ strdup_safe(xkb_context_getenv(ctx, "XCOMPOSEFILE"));
}

അക്ഷരം *
get_xdg_xcompose_file_path(ഘടന xkb_context *ctx)
{
    സ്ഥിരം അക്ഷരം *xdg_config_home;
    സ്ഥിരം അക്ഷരം *home;

    xdg_config_home = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    എങ്കിൽ (!xdg_config_home || !is_absolute_path(xdg_config_home)) {
        home = xkb_context_getenv(ctx, "HOME");
        എങ്കിൽ (!home)
            തിരികെ ശൂന്യ;
        തിരികെ asprintf_safe("%s/.config/XCompose", home);
    }

    തിരികെ asprintf_safe("%s/XCompose", xdg_config_home);
}

അക്ഷരം *
get_home_xcompose_file_path(ഘടന xkb_context *ctx)
{
    സ്ഥിരം അക്ഷരം *home;

    home = xkb_context_getenv(ctx, "HOME");
    എങ്കിൽ (!home)
        തിരികെ ശൂന്യ;

    തിരികെ asprintf_safe("%s/.XCompose", home);
}

അക്ഷരം *
get_locale_compose_file_path(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം *locale)
{
    അക്ഷരം *resolved;
    അക്ഷരം *path;

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
    സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം fallback = "en_US.UTF-8";
    സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം registry = "compose.dir";
    എങ്കിൽ (streq(locale, "C"))
        locale = fallback;

    resolved = resolve_name(ctx, registry, വലത്_TO_ഇടത്, locale);
    എങ്കിൽ (!resolved) {
#എങ്കിൽ HAVE_NEWLOCALE
        /*
         * There is no extension mechanism for X11 Compose locales.
         * Instead of failing, we just use `en_US.UTF-8` as a fallback because
         * it is what most locales use anyway.
         * But we still want to fail if the locale is invalid in the system,
         * so that we can catch missing system locales and typos.
         */
        locale_t loc = newlocale(LC_ALL_MASK, locale, (locale_t) 0);
        എങ്കിൽ (loc == (locale_t) 0) {
            log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                    "No Compose file for locale \"%s\": "
                    "locale is either invalid or not installed\n", locale);
            തിരികെ ശൂന്യ;
        } അല്ലെങ്കിൽ {
            /*
             * The locale is legit but have no entry in the X11 Compose registry,
             * so use the `en_US.UTF-8` fallback.
             */
            freelocale(loc);
            resolved = resolve_name(ctx, registry, വലത്_TO_ഇടത്, fallback);
            എങ്കിൽ (!resolved) {
                log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                        "No Compose file for locale \"%s\": "
                        "failed to use fallback \"%s\"\n", locale, fallback);
                തിരികെ ശൂന്യ;
            } അല്ലെങ്കിൽ {
                log_warn(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                         "No Compose file for locale \"%s\": "
                         "using locale fallback \"%s\"\n", locale, fallback);
            }
        }
#അല്ലെങ്കിൽ
        തിരികെ ശൂന്യ;
#endif
    }

    എങ്കിൽ (is_absolute_path(resolved)) {
        path = resolved;
    }
    അല്ലെങ്കിൽ {
        സ്ഥിരം അക്ഷരം *xlocaledir = get_xlocaledir_path(ctx);
        path = asprintf_safe("%s/%s", xlocaledir, resolved);
        സ്വതന്ത്രം(resolved);
    }

    തിരികെ path;
}
