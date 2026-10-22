/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <locale.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "messages-codes.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "util-paths.h"
#అన్తర్భావ "context.h"
#అన్తర్భావ "paths.h"

గణన resolve_name_direction {
    ఎడమ_గమ్యం_కుడి,
    కుడి_గమ్యం_ఎడమ,
};

స్థిరరాశి అక్షరం *
get_xlocaledir_మార్గం(నిర్మాణం xkb_context *ctx)
{
    స్థిరరాశి అక్షరం *dir = xkb_context_getenv(ctx, "XLOCALEDIR");
    ఒకవేళ (!dir)
        dir = XLOCALEDIR;
    తిరిగి dir;
}

/*
 * Files like compose.dir have the format LEFT: RIGHT.  Lookup @name in
 * such a file and return its matching value, according to @direction.
 * @filename is relative to the xlocaledir.
 */
స్థిర అక్షరం *
resolve_name(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం *filename,
             గణన resolve_name_direction direction, స్థిరరాశి అక్షరం *name)
{
    పూర్ణాంకం ret;
    bool ok;
    స్థిరరాశి అక్షరం *xlocaledir;
    అక్షరం మార్గం[512];
    FILE *file;
    అక్షరం *string;
    కొలత_t string_కొలత;
    స్థిరరాశి అక్షరం *చివర;
    స్థిరరాశి అక్షరం *s, *ఎడమ, *కుడి;
    అక్షరం *సరిపోలిక;
    కొలత_t ఎడమ_len, కుడి_len, name_len;

    xlocaledir = get_xlocaledir_మార్గం(ctx);

    ret = snprintf(మార్గం, పరిమాణం(మార్గం), "%s/%s", xlocaledir, filename);
    ఒకవేళ (ret < 0 || (కొలత_t) ret >= పరిమాణం(మార్గం))
        తిరిగి ఖాళీ;

    file = fopen(మార్గం, "rb");
    ఒకవేళ (!file)
        తిరిగి ఖాళీ;

    ok = map_file(file, &string, &string_కొలత);
    fclose(file);
    ఒకవేళ (!ok)
        తిరిగి ఖాళీ;

    s = string;
    చివర = string + string_కొలత;
    name_len = అక్షరశ్రేణి_పొడవు(name);
    సరిపోలిక = ఖాళీ;

    అయితే (s < చివర) {
        /* Skip spaces. */
        అయితే (s < చివర && is_space(*s))
            s++;

        /* Skip comments. */
        ఒకవేళ (s < చివర && *s == '#') {
            అయితే (s < చివర && *s != '\n')
                s++;
            కొనసాగు;
        }

        /* Get the left value. */
        ఎడమ = s;
        అయితే (s < చివర && !is_space(*s) && *s != ':')
            s++;
        ఎడమ_len = (కొలత_t)(s - ఎడమ);

        /* There's an optional colon between left and right. */
        ఒకవేళ (s < చివర && *s == ':')
            s++;

        /* Skip spaces. */
        అయితే (s < చివర && is_space(*s))
            s++;

        /* Get the right value. */
        కుడి = s;
        అయితే (s < చివర && !is_space(*s))
            s++;
        కుడి_len = (కొలత_t)(s - కుడి);

        /* Discard rest of line. */
        అయితే (s < చివర && *s != '\n')
            s++;

        ఒకవేళ (direction == ఎడమ_గమ్యం_కుడి) {
            ఒకవేళ (ఎడమ_len == name_len && memcmp(ఎడమ, name, ఎడమ_len) == 0) {
                సరిపోలిక = strndup(కుడి, కుడి_len);
                ఆపు;
            }
        } లేకపోతే ఒకవేళ (direction == కుడి_గమ్యం_ఎడమ &&
                   కుడి_len == name_len &&
                   memcmp(కుడి, name, కుడి_len) == 0) {
            సరిపోలిక = strndup(ఎడమ, ఎడమ_len);
            ఆపు;
        }
    }

    unmap_file(string, string_కొలత);
    తిరిగి సరిపోలిక;
}

అక్షరం *
resolve_locale(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం *locale)
{
    అక్షరం *alias = resolve_name(ctx, "locale.alias", ఎడమ_గమ్యం_కుడి, locale);
    తిరిగి alias ? alias : strdup(locale);
}

అక్షరం *
get_xcomposefile_మార్గం(నిర్మాణం xkb_context *ctx)
{
    తిరిగి strdup_సురక్షిత(xkb_context_getenv(ctx, "XCOMPOSEFILE"));
}

అక్షరం *
get_xdg_xcompose_file_మార్గం(నిర్మాణం xkb_context *ctx)
{
    స్థిరరాశి అక్షరం *xdg_config_home;
    స్థిరరాశి అక్షరం *home;

    xdg_config_home = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    ఒకవేళ (!xdg_config_home || !is_absolute_మార్గం(xdg_config_home)) {
        home = xkb_context_getenv(ctx, "HOME");
        ఒకవేళ (!home)
            తిరిగి ఖాళీ;
        తిరిగి asprintf_సురక్షిత("%s/.config/XCompose", home);
    }

    తిరిగి asprintf_సురక్షిత("%s/XCompose", xdg_config_home);
}

అక్షరం *
get_home_xcompose_file_మార్గం(నిర్మాణం xkb_context *ctx)
{
    స్థిరరాశి అక్షరం *home;

    home = xkb_context_getenv(ctx, "HOME");
    ఒకవేళ (!home)
        తిరిగి ఖాళీ;

    తిరిగి asprintf_సురక్షిత("%s/.XCompose", home);
}

అక్షరం *
get_locale_compose_file_మార్గం(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం *locale)
{
    అక్షరం *resolved;
    అక్షరం *మార్గం;

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
    స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి fallback = "en_US.UTF-8";
    స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి registry = "compose.dir";
    ఒకవేళ (streq(locale, "C"))
        locale = fallback;

    resolved = resolve_name(ctx, registry, కుడి_గమ్యం_ఎడమ, locale);
    ఒకవేళ (!resolved) {
#ఒకవేళ HAVE_NEWLOCALE
        /*
         * There is no extension mechanism for X11 Compose locales.
         * Instead of failing, we just use `en_US.UTF-8` as a fallback because
         * it is what most locales use anyway.
         * But we still want to fail if the locale is invalid in the system,
         * so that we can catch missing system locales and typos.
         */
        locale_t loc = newlocale(LC_ALL_MASK, locale, (locale_t) 0);
        ఒకవేళ (loc == (locale_t) 0) {
            log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                    "No Compose file for locale \"%s\": "
                    "locale is either invalid or not installed\n", locale);
            తిరిగి ఖాళీ;
        } లేకపోతే {
            /*
             * The locale is legit but have no entry in the X11 Compose registry,
             * so use the `en_US.UTF-8` fallback.
             */
            freelocale(loc);
            resolved = resolve_name(ctx, registry, కుడి_గమ్యం_ఎడమ, fallback);
            ఒకవేళ (!resolved) {
                log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                        "No Compose file for locale \"%s\": "
                        "failed to use fallback \"%s\"\n", locale, fallback);
                తిరిగి ఖాళీ;
            } లేకపోతే {
                log_warn(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                         "No Compose file for locale \"%s\": "
                         "using locale fallback \"%s\"\n", locale, fallback);
            }
        }
#లేకపోతే
        తిరిగి ఖాళీ;
#అయితే_ముగింపు
    }

    ఒకవేళ (is_absolute_మార్గం(resolved)) {
        మార్గం = resolved;
    }
    లేకపోతే {
        స్థిరరాశి అక్షరం *xlocaledir = get_xlocaledir_మార్గం(ctx);
        మార్గం = asprintf_సురక్షిత("%s/%s", xlocaledir, resolved);
        విడుదల(resolved);
    }

    తిరిగి మార్గం;
}
