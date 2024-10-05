#include "cconfig/cconfig-util.h"

#include "jbhook2/config-io.h"

#include "util/log.h"

#define JBHOOK2_CONFIG_IO_WINDOWED_KEY "windowed"
#define JBHOOK2_CONFIG_IO_VERTICAL_KEY "vertical"
#define JBHOOK2_CONFIG_IO_DISABLE_P3IOEMU_KEY "disable_p3ioemu"
#define JBHOOK2_CONFIG_IO_DISABLE_CARDEMU_KEY "disable_cardemu"
#define JBHOOK2_CONFIG_IO_DISABLE_ADAPTEREMU_KEY "disable_adapteremu"
#define JBHOOK2_CONFIG_IO_SHOW_CURSOR_KEY "show_cursor"
#define JBHOOK2_CONFIG_IO_USE_EXTERNAL_IP_KEY "use_external"
#define JBHOOK2_CONFIG_IO_USE_OVERRIDE_IP_KEY "use_override"
#define JBHOOK2_CONFIG_IO_USE_SPECIFIC_ADAPTER_UUID_KEY \
    "use_specific_adapter_uuid"
#define JBHOOK2_CONFIG_IO_OVERRIDE_IP_KEY "override_ip"

#define JBHOOK2_CONFIG_IO_DEFAULT_WINDOWED_VALUE false
#define JBHOOK2_CONFIG_IO_DEFAULT_VERTICAL_VALUE false
#define JBHOOK2_CONFIG_IO_DEFAULT_DISABLE_P3IOEMU_VALUE false
#define JBHOOK2_CONFIG_IO_DEFAULT_DISABLE_CARDEMU_VALUE false
#define JBHOOK2_CONFIG_IO_DEFAULT_DISABLE_ADAPTEREMU_VALUE false
#define JBHOOK2_CONFIG_IO_DEFAULT_SHOW_CURSOR_VALUE false
#define JBHOOK2_CONFIG_IO_DEFAULT_USE_EXTERNAL_IP_VALUE false
#define JBHOOK2_CONFIG_IO_DEFAULT_USE_OVERRIDE_IP_VALUE false
#define JBHOOK2_CONFIG_IO_DEFAULT_USE_SPECIFIC_ADAPTER_UUID_VALUE ""
#define JBHOOK2_CONFIG_IO_DEFAULT_OVERRIDE_IP_VALUE ""

void jbhook2_config_io_init(struct cconfig *config)
{
    cconfig_util_set_bool(
        config,
        JBHOOK2_CONFIG_IO_WINDOWED_KEY,
        JBHOOK2_CONFIG_IO_DEFAULT_WINDOWED_VALUE,
        "Run in windowed mode");

    cconfig_util_set_bool(
        config,
        JBHOOK2_CONFIG_IO_VERTICAL_KEY,
        JBHOOK2_CONFIG_IO_DEFAULT_VERTICAL_VALUE,
        "Run in vertical mode");

    cconfig_util_set_bool(
        config,
        JBHOOK2_CONFIG_IO_DISABLE_P3IOEMU_KEY,
        JBHOOK2_CONFIG_IO_DEFAULT_DISABLE_P3IOEMU_VALUE,
        "Disable P3 IO emulation");

    cconfig_util_set_bool(
        config,
        JBHOOK2_CONFIG_IO_DISABLE_CARDEMU_KEY,
        JBHOOK2_CONFIG_IO_DEFAULT_DISABLE_CARDEMU_VALUE,
        "Disable card reader emulation");

    cconfig_util_set_bool(
        config,
        JBHOOK2_CONFIG_IO_DISABLE_ADAPTEREMU_KEY,
        JBHOOK2_CONFIG_IO_DEFAULT_DISABLE_ADAPTEREMU_VALUE,
        "Disable adapter emulation");

    cconfig_util_set_bool(
        config,
        JBHOOK2_CONFIG_IO_SHOW_CURSOR_KEY,
        JBHOOK2_CONFIG_IO_DEFAULT_SHOW_CURSOR_VALUE,
        "Show cursor");

    cconfig_util_set_bool(
        config,
        JBHOOK2_CONFIG_IO_USE_EXTERNAL_IP_KEY,
        JBHOOK2_CONFIG_IO_DEFAULT_USE_EXTERNAL_IP_VALUE,
        "Use external IP");

    cconfig_util_set_bool(
        config,
        JBHOOK2_CONFIG_IO_USE_OVERRIDE_IP_KEY,
        JBHOOK2_CONFIG_IO_DEFAULT_USE_OVERRIDE_IP_VALUE,
        "Use override IP");

    cconfig_util_set_str(
        config,
        JBHOOK2_CONFIG_IO_USE_SPECIFIC_ADAPTER_UUID_KEY,
        JBHOOK2_CONFIG_IO_DEFAULT_USE_SPECIFIC_ADAPTER_UUID_VALUE,
        "Use specific adapter UUID");

    cconfig_util_set_str(
        config,
        JBHOOK2_CONFIG_IO_OVERRIDE_IP_KEY,
        JBHOOK2_CONFIG_IO_DEFAULT_OVERRIDE_IP_VALUE,
        "Override IP");
}

void jbhook2_config_io_get(
    struct jbhook2_config_io *config_io, struct cconfig *config)
{
    if (!cconfig_util_get_bool(
            config,
            JBHOOK2_CONFIG_IO_WINDOWED_KEY,
            &config_io->windowed,
            JBHOOK2_CONFIG_IO_DEFAULT_WINDOWED_VALUE)) {
        log_warning(
            "Invalid value for key '%s' specified, fallback "
            "to default '%d'",
            JBHOOK2_CONFIG_IO_WINDOWED_KEY,
            JBHOOK2_CONFIG_IO_DEFAULT_WINDOWED_VALUE);
    }

    if (!cconfig_util_get_bool(
            config,
            JBHOOK2_CONFIG_IO_VERTICAL_KEY,
            &config_io->vertical,
            JBHOOK2_CONFIG_IO_DEFAULT_VERTICAL_VALUE)) {
        log_warning(
            "Invalid value for key '%s' specified, fallback "
            "to default '%d'",
            JBHOOK2_CONFIG_IO_VERTICAL_KEY,
            JBHOOK2_CONFIG_IO_DEFAULT_VERTICAL_VALUE);
    }

    if (!cconfig_util_get_bool(
            config,
            JBHOOK2_CONFIG_IO_DISABLE_P3IOEMU_KEY,
            &config_io->disable_p3ioemu,
            JBHOOK2_CONFIG_IO_DEFAULT_DISABLE_P3IOEMU_VALUE)) {
        log_warning(
            "Invalid value for key '%s' specified, fallback "
            "to default '%d'",
            JBHOOK2_CONFIG_IO_DISABLE_P3IOEMU_KEY,
            JBHOOK2_CONFIG_IO_DEFAULT_DISABLE_P3IOEMU_VALUE);
    }

    if (!cconfig_util_get_bool(
            config,
            JBHOOK2_CONFIG_IO_DISABLE_CARDEMU_KEY,
            &config_io->disable_cardemu,
            JBHOOK2_CONFIG_IO_DEFAULT_DISABLE_CARDEMU_VALUE)) {
        log_warning(
            "Invalid value for key '%s' specified, fallback "
            "to default '%d'",
            JBHOOK2_CONFIG_IO_DISABLE_CARDEMU_KEY,
            JBHOOK2_CONFIG_IO_DEFAULT_DISABLE_CARDEMU_VALUE);
    }

    if (!cconfig_util_get_bool(
            config,
            JBHOOK2_CONFIG_IO_DISABLE_ADAPTEREMU_KEY,
            &config_io->disable_adapteremu,
            JBHOOK2_CONFIG_IO_DEFAULT_DISABLE_ADAPTEREMU_VALUE)) {
        log_warning(
            "Invalid value for key '%s' specified, fallback "
            "to default '%d'",
            JBHOOK2_CONFIG_IO_DISABLE_ADAPTEREMU_KEY,
            JBHOOK2_CONFIG_IO_DEFAULT_DISABLE_ADAPTEREMU_VALUE);
    }
}