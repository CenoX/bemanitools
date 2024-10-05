#include "cconfig/cconfig-util.h"

#include "jbhook3/config-io.h"

#include "util/log.h"

#define JBHOOK3_CONFIG_IO_WINDOWED_KEY "windowed"
#define JBHOOK3_CONFIG_IO_DISABLE_P4IOEMU_KEY "disable_p4ioemu"
#define JBHOOK3_CONFIG_IO_DISABLE_CARDEMU_KEY "disable_cardemu"
#define JBHOOK3_CONFIG_IO_DISABLE_ADAPTEREMU_KEY "disable_adapteremu"
#define JBHOOK3_CONFIG_IO_SHOW_CURSOR_KEY "show_cursor"
#define JBHOOK3_CONFIG_IO_USE_EXTERNAL_IP_KEY "use_external_ip"
#define JBHOOK3_CONFIG_IO_USE_OVERRIDE_IP_KEY "use_override_ip"
#define JBHOOK3_CONFIG_IO_OVERRIDE_IP_KEY "override_ip"

#define JBHOOK3_CONFIG_IO_DEFAULT_WINDOWED_VALUE false
#define JBHOOK3_CONFIG_IO_DEFAULT_DISABLE_P4IOEMU_VALUE false
#define JBHOOK3_CONFIG_IO_DEFAULT_DISABLE_CARDEMU_VALUE false
#define JBHOOK3_CONFIG_IO_DEFAULT_DISABLE_ADAPTEREMU_VALUE false
#define JBHOOK3_CONFIG_IO_DEFAULT_SHOW_CURSOR_VALUE false
#define JBHOOK3_CONFIG_IO_DEFAULT_USE_EXTERNAL_IP_VALUE false
#define JBHOOK3_CONFIG_IO_DEFAULT_USE_OVERRIDE_IP_VALUE false
#define JBHOOK3_CONFIG_IO_DEFAULT_OVERRIDE_IP_VALUE ""

void jbhook3_config_io_init(struct cconfig *config) {

    cconfig_util_set_bool(
        config,
        JBHOOK3_CONFIG_IO_WINDOWED_KEY,
        JBHOOK3_CONFIG_IO_DEFAULT_WINDOWED_VALUE,
        "Run in windowed mode");

    cconfig_util_set_bool(
        config,
        JBHOOK3_CONFIG_IO_DISABLE_P4IOEMU_KEY,
        JBHOOK3_CONFIG_IO_DEFAULT_DISABLE_P4IOEMU_VALUE,
        "Disable P4 IO emulation");

    cconfig_util_set_bool(
        config,
        JBHOOK3_CONFIG_IO_DISABLE_CARDEMU_KEY,
        JBHOOK3_CONFIG_IO_DEFAULT_DISABLE_CARDEMU_VALUE,
        "Disable card reader emulation");

    cconfig_util_set_bool(
        config,
        JBHOOK3_CONFIG_IO_DISABLE_ADAPTEREMU_KEY,
        JBHOOK3_CONFIG_IO_DEFAULT_DISABLE_ADAPTEREMU_VALUE,
        "Disable adapter emulation");

    cconfig_util_set_bool(
        config,
        JBHOOK3_CONFIG_IO_SHOW_CURSOR_KEY,
        JBHOOK3_CONFIG_IO_DEFAULT_SHOW_CURSOR_VALUE,
        "Show cursor");

    cconfig_util_set_bool(
        config,
        JBHOOK3_CONFIG_IO_USE_EXTERNAL_IP_KEY,
        JBHOOK3_CONFIG_IO_DEFAULT_USE_EXTERNAL_IP_VALUE,
        "Use external IP");

    cconfig_util_set_bool(
        config,
        JBHOOK3_CONFIG_IO_USE_OVERRIDE_IP_KEY,
        JBHOOK3_CONFIG_IO_DEFAULT_USE_OVERRIDE_IP_VALUE,
        "Use override IP");

    cconfig_util_set_str(
        config,
        JBHOOK3_CONFIG_IO_OVERRIDE_IP_KEY,
        JBHOOK3_CONFIG_IO_DEFAULT_OVERRIDE_IP_VALUE,
        "Override IP");
}

void jbhook3_config_io_get(struct jbhook3_config_io *config_io, struct cconfig *config) {

    if (!cconfig_util_get_bool(
            config,
            JBHOOK3_CONFIG_IO_WINDOWED_KEY,
            &config_io->windowed,
            JBHOOK3_CONFIG_IO_DEFAULT_WINDOWED_VALUE)) {
        log_warning(
            "Invalid value for key '%s' specified, fallback "
            "to default '%d'",
            JBHOOK3_CONFIG_IO_WINDOWED_KEY,
            JBHOOK3_CONFIG_IO_DEFAULT_WINDOWED_VALUE);
    }

    if (!cconfig_util_get_bool(
            config,
            JBHOOK3_CONFIG_IO_DISABLE_P4IOEMU_KEY,
            &config_io->disable_p4ioemu,
            JBHOOK3_CONFIG_IO_DEFAULT_DISABLE_P4IOEMU_VALUE)) {
        log_warning(
            "Invalid value for key '%s' specified, fallback "
            "to default '%d'",
            JBHOOK3_CONFIG_IO_DISABLE_P4IOEMU_KEY,
            JBHOOK3_CONFIG_IO_DEFAULT_DISABLE_P4IOEMU_VALUE);
    }

    if (!cconfig_util_get_bool(
            config,
            JBHOOK3_CONFIG_IO_DISABLE_CARDEMU_KEY,
            &config_io->disable_cardemu,
            JBHOOK3_CONFIG_IO_DEFAULT_DISABLE_CARDEMU_VALUE)) {
        log_warning(
            "Invalid value for key '%s' specified, fallback "
            "to default '%d'",
            JBHOOK3_CONFIG_IO_DISABLE_CARDEMU_KEY,
            JBHOOK3_CONFIG_IO_DEFAULT_DISABLE_CARDEMU_VALUE);
    }

    if (!cconfig_util_get_bool(
            config,
            JBHOOK3_CONFIG_IO_DISABLE_ADAPTEREMU_KEY,
            &config_io->disable_adapteremu,
            JBHOOK3_CONFIG_IO_DEFAULT_DISABLE_ADAPTEREMU_VALUE)) {
        log_warning(
            "Invalid value for key '%s' specified, fallback "
            "to default '%d'",
            JBHOOK3_CONFIG_IO_DISABLE_ADAPTEREMU_KEY,
            JBHOOK3_CONFIG_IO_DEFAULT_DISABLE_ADAPTEREMU_VALUE);
    }

    if (!cconfig_util_get_bool(
            config,
            JBHOOK3_CONFIG_IO_SHOW_CURSOR_KEY,
            &config_io->show_cursor,
            JBHOOK3_CONFIG_IO_DEFAULT_SHOW_CURSOR_VALUE)) {
        log_warning(
            "Invalid value for key '%s' specified, fallback "
            "to default '%d'",
            JBHOOK3_CONFIG_IO_SHOW_CURSOR_KEY,
            JBHOOK3_CONFIG_IO_DEFAULT_SHOW_CURSOR_VALUE);
    }

    if (!cconfig_util_get_bool(
            config,
            JBHOOK3_CONFIG_IO_USE_EXTERNAL_IP_KEY,
            &config_io->use_external_ip,
            JBHOOK3_CONFIG_IO_DEFAULT_USE_EXTERNAL_IP_VALUE)) {
        log_warning(
            "Invalid value for key '%s' specified, fallback "
            "to default '%d'",
            JBHOOK3_CONFIG_IO_USE_EXTERNAL_IP_KEY,
            JBHOOK3_CONFIG_IO_DEFAULT_USE_EXTERNAL_IP_VALUE);
    }

    if (!cconfig_util_get_bool(
            config,
            JBHOOK3_CONFIG_IO_USE_OVERRIDE_IP_KEY,
            &config_io->use_override_ip,
            JBHOOK3_CONFIG_IO_DEFAULT_USE_OVERRIDE_IP_VALUE)) {
        log_warning(
            "Invalid value for key '%s' specified, fallback "
            "to default '%d'",
            JBHOOK3_CONFIG_IO_USE_OVERRIDE_IP_KEY,
            JBHOOK3_CONFIG_IO_DEFAULT_USE_OVERRIDE_IP_VALUE);
    }

    if (!cconfig_util_get_str(
            config,
            JBHOOK3_CONFIG_IO_OVERRIDE_IP_KEY,
            config_io->override_ip,
            sizeof(config_io->override_ip),
            JBHOOK3_CONFIG_IO_DEFAULT_OVERRIDE_IP_VALUE)) {
        log_warning(
            "Invalid value for key '%s' specified, fallback "
            "to default '%s'",
            JBHOOK3_CONFIG_IO_OVERRIDE_IP_KEY,
            JBHOOK3_CONFIG_IO_DEFAULT_OVERRIDE_IP_VALUE);
    }

}