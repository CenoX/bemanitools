#include <windows.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bemanitools/eamio.h"
#include "bemanitools/jbio.h"

#include "cconfig/cconfig-hook.h"

#include "hook/iohook.h"
#include "hook/table.h"

#include "hooklib/adapter.h"
#include "hooklib/app.h"
#include "hooklib/rs232.h"
#include "hooklib/setupapi.h"

#include "imports/avs.h"

#include "jbhook3/config-io.h"
#include "jbhook3/gfx.h"

#include "jbhook-util/acio.h"
#include "jbhook-util/eamuse.h"
#include "jbhook-util/p4io.h"

#include "p4ioemu/device.h"
#include "p4ioemu/setupapi.h"

#include "security/id.h"

#include "util/defs.h"
#include "util/log.h"
#include "util/thread.h"

#define JBHOOK3_INFO_HEADER     \
    "jbhook3 for saucer and up" \
    ", build " __DATE__ " " __TIME__ ", gitrev " STRINGIFY(GITREV) "\n"
#define JBHOOK3_CMD_USAGE                 \
    "Usage: launcher.exe -K jbhook3.dll " \
    "<jubeat.dll> [options...]"

static struct jbhook3_config_io jbhook3_config_io;

static bool load_configs()
{
    struct cconfig *config;
    config = cconfig_init();

    jbhook3_config_io_init(config);

    if (!cconfig_hook_config_init(
            config,
            JBHOOK3_INFO_HEADER "\n" JBHOOK3_CMD_USAGE,
            CCONFIG_CMD_USAGE_OUT_DBG)) {
        cconfig_finit(config);
        return false;
    }

    jbhook3_config_io_get(&jbhook3_config_io, config);

    cconfig_finit(config);

    return true;
}

static bool my_dll_entry_init(char *sidcode, struct property_node *param)
{
    bool eam_io_ok;
    bool jb_io_ok;

    eam_io_ok = false;
    jb_io_ok = false;

    log_info("--- Begin jbhook dll_entry_init ---");

    log_info(JBHOOK3_INFO_HEADER);
    log_info("Initializing jbhook...");

    // reload configs again so they get logged through avs as well
    // (so we get a copy of them in the -Y logfile)
    if (!load_configs()) {
        exit(EXIT_FAILURE);
    }

    iohook_push_handler(p4ioemu_dispatch_irp);
    iohook_push_handler(jbhook_util_ac_io_port_dispatch_irp);

    jbhook3_gfx_init();

    if (jbhook3_config_io.windowed) {
        jbhook3_gfx_set_windowed();
    }

    if (jbhook3_config_io.show_cursor) {
        jbhook3_gfx_set_show_cursor();
    }

    if (!jbhook3_config_io.disable_p4ioemu) {
        log_info("Starting up jubeat IO backend");

        jb_io_set_loggers(
            log_impl_misc, log_impl_info, log_impl_warning, log_impl_fatal);

        jb_io_ok =
            jb_io_init(avs_thread_create, avs_thread_join, avs_thread_destroy);

        if (!jb_io_ok) {
            goto fail;
        }

        hook_setupapi_init(&p4ioemu_setupapi_data);
        p4ioemu_init(jbhook_p4io_init());
    }

    if (!jbhook3_config_io.disable_cardemu) {
        log_info("Starting up card reader backend");

        eam_io_set_loggers(
            log_impl_misc, log_impl_info, log_impl_warning, log_impl_fatal);

        eam_io_ok =
            eam_io_init(avs_thread_create, avs_thread_join, avs_thread_destroy);

        if (!eam_io_ok) {
            goto fail;
        }

        rs232_hook_init();
        jbhook_util_ac_io_port_init(L"COM2");
        jbhook_util_ac_io_set_iccb();
    }

    log_info("---  End  jbhook dll_entry_init ---");

    bool ret = app_hook_invoke_init(sidcode, param);

    return ret;

fail:
    if (eam_io_ok) {
        eam_io_fini();
    }

    if (jb_io_ok) {
        jb_io_fini();
    }

    return false;
}

static bool my_dll_entry_main(void)
{
    bool result;

    result = app_hook_invoke_main();

    log_info("Shutting down card reader backend");
    eam_io_fini();

    log_info("Shutting down Jubeat IO backend");
    jb_io_fini();

    if (!jbhook3_config_io.disable_p4ioemu) {
        p4ioemu_fini();
    }

    if (!jbhook3_config_io.disable_cardemu) {
        jbhook_util_ac_io_port_fini();
    }

    return result;
}

BOOL WINAPI DllMain(HMODULE mod, DWORD reason, void *ctx)
{
    if (reason != DLL_PROCESS_ATTACH) {
        return TRUE;
    }

    log_to_external(
        log_body_misc, log_body_info, log_body_warning, log_body_fatal);

    app_hook_init(my_dll_entry_init, my_dll_entry_main);

    if (!jbhook3_config_io.disable_adapteremu) {
        adapter_hook_init();
    }

    jbhook_util_eamuse_hook_init();

    // TODO: MAYBE WE SHOULD ADD EXTERNEL & OVERRIDE IP FROM HERE?
    // if (jbhook3_config_io.use_external_ip) {
    adapter_hook_wan_override();
    // }
    return TRUE;
}
