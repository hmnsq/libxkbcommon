/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <locale.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "messages-codes.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "util-paths.h"
#અન્તર્ભાવ "context.h"
#અન્તર્ભાવ "paths.h"

ગણના resolve_name_direction {
    ડાબું_TO_જમણું,
    જમણું_TO_ડાબું,
};

અચલ અક્ષર *
get_xlocaledir_path(સંરચના xkb_context *ctx)
{
    અચલ અક્ષર *dir = xkb_context_getenv(ctx, "XLOCALEDIR");
    જો (!dir)
        dir = XLOCALEDIR;
    પાછા dir;
}

/*
 * Files like compose.dir have the format LEFT: RIGHT.  Lookup @name in
 * such a file and return its matching value, according to @direction.
 * @filename is relative to the xlocaledir.
 */
સ્થિર અક્ષર *
resolve_name(સંરચના xkb_context *ctx, અચલ અક્ષર *filename,
             ગણના resolve_name_direction direction, અચલ અક્ષર *name)
{
    પૂર્ણાંક ret;
    bool ok;
    અચલ અક્ષર *xlocaledir;
    અક્ષર path[512];
    FILE *file;
    અક્ષર *string;
    size_t string_size;
    અચલ અક્ષર *end;
    અચલ અક્ષર *s, *ડાબું, *જમણું;
    અક્ષર *match;
    size_t ડાબું_len, જમણું_len, name_len;

    xlocaledir = get_xlocaledir_path(ctx);

    ret = snprintf(path, કદ(path), "%s/%s", xlocaledir, filename);
    જો (ret < 0 || (size_t) ret >= કદ(path))
        પાછા ખાલી;

    file = fopen(path, "rb");
    જો (!file)
        પાછા ખાલી;

    ok = map_file(file, &string, &string_size);
    fclose(file);
    જો (!ok)
        પાછા ખાલી;

    s = string;
    end = string + string_size;
    name_len = strlen(name);
    match = ખાલી;

    જ્યારે (s < end) {
        /* Skip spaces. */
        જ્યારે (s < end && is_space(*s))
            s++;

        /* Skip comments. */
        જો (s < end && *s == '#') {
            જ્યારે (s < end && *s != '\n')
                s++;
            ચાલુ;
        }

        /* Get the left value. */
        ડાબું = s;
        જ્યારે (s < end && !is_space(*s) && *s != ':')
            s++;
        ડાબું_len = (size_t)(s - ડાબું);

        /* There's an optional colon between left and right. */
        જો (s < end && *s == ':')
            s++;

        /* Skip spaces. */
        જ્યારે (s < end && is_space(*s))
            s++;

        /* Get the right value. */
        જમણું = s;
        જ્યારે (s < end && !is_space(*s))
            s++;
        જમણું_len = (size_t)(s - જમણું);

        /* Discard rest of line. */
        જ્યારે (s < end && *s != '\n')
            s++;

        જો (direction == ડાબું_TO_જમણું) {
            જો (ડાબું_len == name_len && memcmp(ડાબું, name, ડાબું_len) == 0) {
                match = strndup(જમણું, જમણું_len);
                તોડો;
            }
        } નહીંતર જો (direction == જમણું_TO_ડાબું &&
                   જમણું_len == name_len &&
                   memcmp(જમણું, name, જમણું_len) == 0) {
            match = strndup(ડાબું, ડાબું_len);
            તોડો;
        }
    }

    unmap_file(string, string_size);
    પાછા match;
}

અક્ષર *
resolve_locale(સંરચના xkb_context *ctx, અચલ અક્ષર *locale)
{
    અક્ષર *alias = resolve_name(ctx, "locale.alias", ડાબું_TO_જમણું, locale);
    પાછા alias ? alias : strdup(locale);
}

અક્ષર *
get_xcomposefile_path(સંરચના xkb_context *ctx)
{
    પાછા strdup_safe(xkb_context_getenv(ctx, "XCOMPOSEFILE"));
}

અક્ષર *
get_xdg_xcompose_file_path(સંરચના xkb_context *ctx)
{
    અચલ અક્ષર *xdg_config_home;
    અચલ અક્ષર *home;

    xdg_config_home = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    જો (!xdg_config_home || !is_absolute_path(xdg_config_home)) {
        home = xkb_context_getenv(ctx, "HOME");
        જો (!home)
            પાછા ખાલી;
        પાછા asprintf_safe("%s/.config/XCompose", home);
    }

    પાછા asprintf_safe("%s/XCompose", xdg_config_home);
}

અક્ષર *
get_home_xcompose_file_path(સંરચના xkb_context *ctx)
{
    અચલ અક્ષર *home;

    home = xkb_context_getenv(ctx, "HOME");
    જો (!home)
        પાછા ખાલી;

    પાછા asprintf_safe("%s/.XCompose", home);
}

અક્ષર *
get_locale_compose_file_path(સંરચના xkb_context *ctx, અચલ અક્ષર *locale)
{
    અક્ષર *resolved;
    અક્ષર *path;

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
    સ્થિર અચલ અક્ષર * અચલ fallback = "en_US.UTF-8";
    સ્થિર અચલ અક્ષર * અચલ registry = "compose.dir";
    જો (streq(locale, "C"))
        locale = fallback;

    resolved = resolve_name(ctx, registry, જમણું_TO_ડાબું, locale);
    જો (!resolved) {
#જો HAVE_NEWLOCALE
        /*
         * There is no extension mechanism for X11 Compose locales.
         * Instead of failing, we just use `en_US.UTF-8` as a fallback because
         * it is what most locales use anyway.
         * But we still want to fail if the locale is invalid in the system,
         * so that we can catch missing system locales and typos.
         */
        locale_t loc = newlocale(LC_ALL_MASK, locale, (locale_t) 0);
        જો (loc == (locale_t) 0) {
            log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                    "No Compose file for locale \"%s\": "
                    "locale is either invalid or not installed\n", locale);
            પાછા ખાલી;
        } નહીંતર {
            /*
             * The locale is legit but have no entry in the X11 Compose registry,
             * so use the `en_US.UTF-8` fallback.
             */
            freelocale(loc);
            resolved = resolve_name(ctx, registry, જમણું_TO_ડાબું, fallback);
            જો (!resolved) {
                log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                        "No Compose file for locale \"%s\": "
                        "failed to use fallback \"%s\"\n", locale, fallback);
                પાછા ખાલી;
            } નહીંતર {
                log_warn(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                         "No Compose file for locale \"%s\": "
                         "using locale fallback \"%s\"\n", locale, fallback);
            }
        }
#નહીંતર
        પાછા ખાલી;
#endif
    }

    જો (is_absolute_path(resolved)) {
        path = resolved;
    }
    નહીંતર {
        અચલ અક્ષર *xlocaledir = get_xlocaledir_path(ctx);
        path = asprintf_safe("%s/%s", xlocaledir, resolved);
        મુક્ત(resolved);
    }

    પાછા path;
}
