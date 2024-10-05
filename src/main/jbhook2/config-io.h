#ifndef JBHOOK2_CONFIG_IO_H
#define JBHOOK2_CONFIG_IO_H

#include <windows.h>
#include "cconfig/cconfig.h"

struct jbhook2_config_io {
    bool windowed;
    bool vertical;
    bool disable_p3ioemu;
    bool disable_cardemu;
    bool disable_adapteremu;
    bool show_cursor;
    bool use_external_ip;
    bool use_override_ip;
    char use_specific_adapter_uuid[260];
    char override_ip[16];
};

void jbhook2_config_io_init(struct cconfig *config);

void jbhook2_config_io_get(
    struct jbhook2_config_io *config_io, struct cconfig *config);

#endif